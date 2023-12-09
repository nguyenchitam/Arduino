// Reference: https://forums.adafruit.com/viewtopic.php?t=43375
#include <Adafruit_NeoPixel.h>


#define PIN 6
#define NUM_PIXELS 16
#define BRIGHTNESS 50


//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint32_t colors[] = { strip.Color(BRIGHTNESS, 0, 0), strip.Color(0, BRIGHTNESS, 0), strip.Color(0, 0, BRIGHTNESS), strip.Color(BRIGHTNESS, BRIGHTNESS, 0) };
int numColors = sizeof(colors) / sizeof(colors[0]);
int position;


void setup() {
  randomSeed(analogRead(0));

  strip.begin();
  colorWipe(0, 0);
  strip.show();
}


void loop() {
  long randNumber = random(NUM_PIXELS);
  int direction = random(2);  // 0: Clockwise, 1: Counterclockwise
  int winner = roulette(direction, randNumber);
  blinkWinner(winner, 3);
  delay(3000);
}


int roulette(int direction, long randNumber) {
  float wait = 4.0;

  // Full speed spins
  for (int i = 0; i < NUM_PIXELS; i++) {
    advanceOne(direction, wait);
  }
  // Slow spins & stop
  for (int i = 0; i < (NUM_PIXELS * 6) + randNumber; i++) {
    advanceOne(direction, wait);
    wait = (wait * 1.05);
  }
  delay(100);

  int winner = position;
  if (direction) {
    winner++;
    if (winner >= NUM_PIXELS) winner = 0;
  } else {
    winner--;
    if (winner < 0) winner = NUM_PIXELS - 1;
  }

  return winner;
}


void blinkWinner(int winner, int blinkTimes) {
  uint32_t win = strip.getPixelColor(winner);

  for (int i = 0; i < blinkTimes; i++) {
    strip.setPixelColor(winner, 0);
    strip.show();
    delay(500);
    strip.setPixelColor(winner, win);
    strip.show();
    delay(500);
  }
}


void advanceOne(int direction, float wait) {
  colorWipe(0, 0);

  strip.setPixelColor(position, colors[position % numColors]);

  strip.show();
  delay(wait);

  if (direction) {
    position--;
    if (position < 0) position = NUM_PIXELS - 1;
  } else {
    position++;
    if (position >= NUM_PIXELS) position = 0;
  }
}


void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
