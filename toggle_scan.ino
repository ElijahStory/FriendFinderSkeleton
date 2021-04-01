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
  0xE2, 0xC5, 0x6D, 0xB5, 0xDF, 0xFB, 0x48, 0xD2, 
  0xB0, 0x60, 0xD0, 0xF5, 0xA7, 0x10, 0x96, 0xE0, 
};



// A valid Beacon packet consists of the following information:
// UUID, Major, Minor, RSSI @ 1M
BLEBeacon beacon(beaconUuid, 0x0001, 0x0000, -54);
 
void scan_callback(ble_gap_evt_adv_report_t *report)
{
  // does p_data contain the bluetooth UUID?
  Serial.println(report->data.p_data[3]);
  Serial.println(report->data.len);
  Serial.println();

  // Check if advertising contain BleUart service
  if ( Bluefruit.Scanner.checkReportForUuid(report, BLEUART_UUID_SERVICE) )
  {
    Serial.println("                       BLE UART service detected");
  }

  // For Softdevice v6: after received a report, scanner will be paused
  // We need to call Scanner resume() to continue scanning
  Bluefruit.Scanner.resume();

}

void setup() 
{
  Serial.begin(115200);
 
  Serial.println("Bluefruit52 Beacon Example");
 
  Bluefruit.begin();
  Bluefruit.setName("Bluefruit52");
 
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
  delay(500);
  digitalToggle(LED_BUILTIN);
  Serial.println("Stop Adv, Start Scan");
  Bluefruit.Advertising.stop();
  Bluefruit.Scanner.start();
  delay(500);
  
}
