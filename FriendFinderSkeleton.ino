/* FriendFinderSkeleton
 *  Author: Jacob Mealey <jacob.mealey@maine.edu>
 *  For: NMD 342
 *  The purpose of this file is to act as a skeleton
 *  for the Friend Finder Project. 
 */

#include <bluefruit.h>
#include <Adafruit_NeoPixel.h>
// NEOPIXEL ring definitions 
#define RING_SIZE 24
#define RING_PIN 3
Adafruit_NeoPixel strip(RING_SIZE, RING_PIN, NEO_RGB + NEO_KHZ800);

// Some friend finder settings. upper 48 bits are using for 
// matching
uint64_t friendfinder_settings = 0x1111222233330000;
// delay between scans in scandAndAdvertise()
uint16_t del = 500; 

// Friend Finder data is the structure
// that unpack() writes into.
//
// You will have to decide how the data is arranged
// the types in the struct should add up to 48 bits
typedef struct friendfinder_data{
  
} friendfinder_t;

friendfinder_t scannedFriendFinder;


void setup() 
{
  Serial.begin(115200);
 
  Serial.println("Friend Finder Demo");
  setupFriendFinder(friendfinder_settings, del);
}
 

void loop() 
{
  // Scanned Value is the upper 24 bits of the 32 bit integer
  uint32_t ScannedValue = scanAndAdvertise();
  // Unpack must take the scanned value and sort it into scannedFriendFinder
  unpack(ScannedValue);
  
  // if there is a match turn all the lights red.
  if(match()){
    for(int i = 0; i < RING_SIZE; i++){
      strip.setPixelColor(i, strip.Color(127, 0, 0));
    }
    strip.show();
  }
}

// match() is used to define when this friendfinder device
// matches with another friendfinder device. by default
// it returns bool, but depending on your system you could 
// also use an int. 
bool match(){
  
}

// You must write the unpacking procedure. this is dependent
// on your friendfinder_t definitions
void unpack(uint32_t packed_data){
  return;
}
