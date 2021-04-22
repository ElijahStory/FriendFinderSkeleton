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

// ZODIAC DEFS
#define ARIES            0
#define TAURUS           1
#define GEMINI           2
#define CANCER           3
#define LEO              4
#define VIRGO            5
#define LIBRA            6
#define SCORPIO          7
#define SAGITTARIUS      8
#define CAPRICORN        9
#define AQUARIUS         10
#define PISCES           11

uint8_t compatibilities[5];
uint8_t compat_count = 0;
uint8_t zodiac_compatibility[12][12] = 
{
  {50, 38, 83, 42, 97, 63, 85, 50, 93, 47, 78, 67},
  {38, 65, 33, 97, 73, 90, 65, 88, 30, 98, 58, 85},
  {83, 33, 60, 65, 88, 68, 93, 28, 60, 68, 85, 53},
  {42, 97, 65, 75, 35, 90, 43, 94, 53, 83, 27, 98},
  {97, 73, 88, 35, 45, 35, 97, 58, 93, 35, 68, 38},
  {63, 90, 68, 90, 35, 65, 68, 88, 48, 95, 30, 88},
  {85, 65, 93, 43, 97, 68, 75, 35, 73, 55, 90, 88},
  {50, 88, 28, 94, 58, 88, 35, 85, 28, 95, 73, 97},
  {93, 30, 60, 53, 93, 48, 73, 28, 45, 60, 90, 93},
  {47, 98, 68, 83, 35, 95, 55, 95, 60, 75, 68, 88},
  {78, 58, 85, 27, 68, 30, 90, 73, 90, 68, 45, 45},
  {67, 85, 53, 98, 38, 88, 88, 97, 63, 88, 45, 60}
};

// Some friend finder settings. upper 48 bits are using for 
// matching
// delay between scans in scandAndAdvertise()
uint16_t del = 500; // delay in mS, note that it doesn't work well under 300.


uint8_t myZodiac = LIBRA;

uint16_t scanned_zodiac;

void setup() 
{
  Serial.begin(115200);
  Serial.println("Friend Finder Demo");
  setupFriendFinder(myZodiac, del);
}
 

void loop()  
{
  Serial.print("LOOP");
  delay(100);
  // Scanned Value is the upper 24 bits of the 32 bit integer, this functions
  // takes about 2 * del to run. 
  scanned_zodiac = scanAndAdvertise();
  match();
  // if there is a match turn all the lights red.
  for(int i = 0; i < 5; i++){
    int currentZodiacMatch = compatibilities[i];
    
    for(int j = 0; j < currentZodiacMatch / 10; j++){  
      for(int k = 0; k < RING_SIZE; k++){
        strip.setPixelColor(k, strip.Color(127, 0, 0));
      }
      delay(100);
      for(int k = 0; k < RING_SIZE; k++){
        strip.setPixelColor(k, strip.Color(0, 0, 0));
      }
    }
    
    strip.show();
  }
}


uint8_t match()
{
  for(int i = 0; i < 12; i++){    
    delay(100);
    if((scanned_zodiac >> i) & 1UL){
      compatibilities[compat_count] =  zodiac_compatibility[myZodiac][i];
      compat_count++;
    }
  } 
}
