#include <bluefruit.h>
 
// Beacon uses the Manufacturer Specific Data field in the advertising
// packet, which means you must provide a valid Manufacturer ID. Update
// the field below to an appropriate value. For a list of valid IDs see:
// https://www.bluetooth.com/specifications/assigned-numbers/company-identifiers
// 0x004C is Apple (for example)
#define MANUFACTURER_ID   0x004C 
 
// AirLocate UUID: E2C56DB5-DFFB-48D2-B060-D0F5A71096E0
uint8_t beaconUuid[16] = 
{ 
  0x14, 0xFF, 0x6F, 0xB5, 0xDF, 0xFB, 0x48, 0xD2, 
  0xB0, 0x60, 0xD0, 0xF5, 0xA7, 0x10, 0x45, 0xE0, 
};



// A valid Beacon packet consists of the following information:
// UUID, Major, Minor, RSSI @ 1M
BLEBeacon beacon(beaconUuid, 0x000A, 0x0000, -54);
 
void scan_callback(ble_gap_evt_adv_report_t *report)
{

  uint16_t manufacturer;
  
  uint8_t  beacon_type;
  uint8_t  beacon_len;

  uint8_t  uuid128[16];
  uint16_t major;
  uint16_t minor;
  int8_t   rssi_at_1m;
  // This section is straight up WRONG
  manufacturer = (report->data.p_data[3] << 16) | (report->data.p_data[2]);
  beacon_type = report->data.p_data[4];
  // MAJOR bytes: 22, 23 MINOR bytes: 24, 25)
  major = (report->data.p_data[22] << 16) | (report->data.p_data[23]);
  minor = (report->data.p_data[24] << 16) | (report->data.p_data[25]);
  
  Serial.print("man: ");
  Serial.println(manufacturer, HEX);
  Serial.print("Type: ");
  Serial.println(beacon_type, HEX);
  Serial.print("Major ");
  Serial.println(major, HEX);
  Serial.print("Minor ");
  Serial.println(minor, HEX);
  memcpy(uuid128, &(report->data.p_data[5]), 16);
  for(int i = 0; i < 16; i++){
    Serial.print(uuid128[i], HEX);
    Serial.print(" ");
  }
  // does p_data contain the bluetooth UUID?
//  for(int i = 0; i < report->data.len; i++){
//    Serial.print(report->data.p_data[i]);
//    Serial.print(" ");
//  }
  Serial.println();

  // For Softdevice v6: after received a report, scanner will be paused
  // We need to call Scanner resume() to continue scanning
  Bluefruit.Scanner.resume();

}

void setup() 
{
  Serial.begin(115200);
 
  Serial.println("Friend Finder Demo");
 
  Bluefruit.begin();
  Bluefruit.setName("Friend Finder");
 
  // Manufacturer ID is required for Manufacturer Specific Data
  beacon.setManufacturer(MANUFACTURER_ID);
 
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
}
 

void loop() 
{
  // Toggle both LEDs every second
  digitalToggle(LED_BUILTIN);
  Serial.println("Stop Scan, Start Adv");
  Bluefruit.Scanner.stop();
  Bluefruit.Advertising.start();
  delay(1000);
  digitalToggle(LED_BUILTIN);
  Serial.println("Stop Adv, Start Scan");
  Bluefruit.Advertising.stop();
  Bluefruit.Scanner.start();
  delay(1000);
  
}
