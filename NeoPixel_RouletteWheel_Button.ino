// Reference: https://forums.adafruit.com/viewtopic.php?t=43375
#include <Adafruit_NeoPixel.h>


#define BUTTON_PIN 2
#define LED_PIN 7
#define NUM_PIXELS 16
#define BRIGHTNESS 90


//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

uint32_t colors[] = { strip.Color(BRIGHTNESS, 0, 0), strip.Color(0, BRIGHTNESS, 0), strip.Color(0, 0, BRIGHTNESS), strip.Color(BRIGHTNESS, BRIGHTNESS, 0) };
int numColors = sizeof(colors) / sizeof(colors[0]);
int position;
volatile int winner = 0;
volatile bool running = false;


void setup() {
  randomSeed(analogRead(0));

  strip.begin();
  colorWipe(0, 0);
  strip.show();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPressed, FALLING);
}


void loop() {
  strip.setPixelColor(winner, 0);
  strip.show();
  delay(500);
  strip.setPixelColor(winner, colors[winner % numColors]);
  strip.show();
  delay(500);
}


void buttonPressed() {
  if (running) return;
  running = true;
  long randNumber = random(NUM_PIXELS);
  int direction = random(2);  // 0: Clockwise, 1: Counterclockwise
  roulette(direction, randNumber);
  running = false;
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

  winner = position;
  if (direction) {
    winner++;
    if (winner >= NUM_PIXELS) winner = 0;
  } else {
    winner--;
    if (winner < 0) winner = NUM_PIXELS - 1;
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
