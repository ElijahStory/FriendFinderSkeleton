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
// delay between scans in scandAndAdvertise()
uint16_t del = 500; // delay in mS, note that it doesn't work well under 300.

// Friend Finder data is the structure that unpack() writes into.
//
// You will have to decide how the data is arranged the types in the struct 
// should add up to 48 bits
typedef struct friendfinder_data{
  
} friendfinder_t;

friendfinder_t scannedFriendFinder;
friendfinder_t thisFriendFinder;


void setup() 
{
  Serial.begin(115200);
 
  Serial.println("Friend Finder Demo");
  setupFriendFinder(pack(thisFriendFinder), del);
}
 

void loop() 
{
  // Scanned Value is the upper 24 bits of the 32 bit integer, this functions
  // takes about 2 * del to run. 
  uint32_t ScannedValue = scanAndAdvertise();
  // Unpack must take the scanned value and sort it into scannedFriendFinder
  scannedFriendFinder = unpack(ScannedValue);
  
  // if there is a match turn all the lights red.
  if(match(scannedFriendFinder, thisFriendFinder)){
    for(int i = 0; i < RING_SIZE; i++){
      strip.setPixelColor(i, strip.Color(127, 0, 0));
    }
    strip.show();
  }
}

// match() is used to define when this friendfinder device matches with another 
// friendfinder device. by default it returns bool, but depending on your 
// system you could also use an int. 
bool match(friendfinder_t first, friendfinder_t second)
{
  return false;
}

// You must write the packing / unpacking procedure. this is dependent
// on your friendfinder_t definitions
uint64_t pack(friendfinder_t ff)
{
  uint64_t packed;

  return packed;
}


friendfinder_t unpack(uint64_t packed_data)
{
  friendfinder_t ff;
  
  return ff;
}
