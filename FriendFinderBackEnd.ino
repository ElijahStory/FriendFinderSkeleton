/*  FriendFinderBackEnd
 *  Author: Jacob Mealey <jacob.mealey@maine.edu>
 *  For: NMD 342
 *  This file is to provide all the under the hood
 *  functionality for friend finder. Do not edit this
 *  file unless you know what you are doing. 
 */
#include <bluefruit.h>

#define MANUFACTURER_ID         0x004C 
#define MAJOR                   0x000A
#define MINOR                   0x0B00

// Timer Interrupt Constants
#define STATE_SCANNING          0
#define STATE_ADVERTISING       1
#define TIMER_INTERVAL_MS      1000  


// Beacon uuid is the same for every project.
uint8_t beaconUuid[16] = 
{ 
  0x14, 0xFF, 0x6F, 0xB5, 0xDF, 0xFB, 0x48, 0xD2, 
  0xB0, 0x60, 0xD0, 0xF5, 0xA7, 0x10, 0x45, 0xE0, 
};



// A valid Beacon packet consists of the following information:
// UUID, Major, Minor, RSSI @ 1M
BLEBeacon beacon(beaconUuid);

uint32_t volatile scanned; 
uint16_t volatile matched_zodiacs;
uint16_t scan_delay;



void setupFriendFinder(uint16_t zodiac, uint16_t del)
{
  Bluefruit.begin();
  Bluefruit.setName("Friend Finder");
  // Arbitrary major and manufacterur used for matching :)
  beacon.setManufacturer(0x004c);
  beacon.setMajorMinor(0xF0F0, 1 << zodiac);
  
  // Setup the advertising packet
  // Set the beacon payload using the BLEBeacon class populated
  // earlier in this example
  Bluefruit.Advertising.setBeacon(beacon);
 
  // char* adv = Bluefruit.Advertising.getData();
 
  // There is no room left for 'Name' in the advertising packet
  // Use the optinal secondary Scan Response packet for 'Name' instead
  Bluefruit.ScanResponse.addName();
  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.filterRssi(-80); // Only run callback if rssi > 80
  // Start advertising
  Bluefruit.Advertising.start();
  scan_delay = del;
  
}


uint16_t scanAndAdvertise(void)
{
  digitalToggle(LED_BUILTIN);
  Serial.println("Stop Scan, Start Adv");
  Bluefruit.Scanner.stop();
  Bluefruit.Advertising.start();
  delay(scan_delay);
  digitalToggle(LED_BUILTIN);
  Serial.println("Stop Adv, Start Scan");
  Bluefruit.Advertising.stop();
  Bluefruit.Scanner.start();
  delay(scan_delay);
  return matched_zodiacs;
}


void scan_callback(ble_gap_evt_adv_report_t *report)
{
  // Our packet length is 30 bytes long, break away
  // if the report isn't the correct size
  if(report->data.len != 30) return;

  uint16_t manufacturer;
  uint8_t  beacon_type;
  uint8_t  beacon_len;
  uint8_t  uuid128[16];
  uint16_t major;
  uint16_t minor;
  uint16_t incoming_zodiac; // This is the one that MATTERS
  int8_t   rssi_at_1m;

  // Extract data from report. 
  manufacturer = (report->data.p_data[6] << 16) | (report->data.p_data[5]);
  beacon_type = report->data.p_data[7];
  
  // MAJOR bytes: 25, 26 MINOR bytes: 27, 28)
  major = (report->data.p_data[25] << 8) | (report->data.p_data[26]);
  minor = (report->data.p_data[27] << 8) | (report->data.p_data[28]);
  // Copy UUID from report (starts at 9th byte)
  memcpy(uuid128, &(report->data.p_data[9]), 16);

  // Print Everything in HEX
  for(int i = 0; i < 16; i++){
    Serial.print(uuid128[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  Serial.print("Manufacturer: ");
  Serial.println(manufacturer, HEX);
  Serial.print("Type: ");
  Serial.println(beacon_type, HEX);
  Serial.print("Major ");
  Serial.println(major, HEX);
  Serial.print("Minor ");
  Serial.println(minor, HEX);

  // The zodiac is stored in the minor value of the packet.
  incoming_zodiac = minor;
  
  // Update scanned!
  // Only one, perhaps we should return a pointer in memory? 
  scanned  |= major << 24 | minor << 16 | manufacturer << 8;
  matched_zodiacs |= incoming_zodiac;
  // For Softdevice v6: after received a report, scanner will be paused
  // We need to call Scanner resume() to continue scanning
  Bluefruit.Scanner.resume();

}
