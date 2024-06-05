#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>
#include <Wire.h>
#include <SPI.h>

const byte badPin = 8;
const byte Switch = 7;
bool newGame = true;
bool danger;
volatile bool off = analogRead(7);
int enemyCount = 1;
int playerPosition = 0;
int points = 0;
int difficulty = 0;
int enemyPositions[9] {-1, -1, -1, -1, -1, -1, -1, -1, -1};
AsyncDelay delay_5s;
AsyncDelay delay_1s;
const uint8_t spLEVEL[] PROGMEM = {0x69,0xAB,0xC4,0xB3,0xD8,0x92,0x86,0x2D,0x83,0xEE,0x60,0xCD,0x12,0xD6,0x0C,0x66,0x45,0x2C,0x73,0x58,0x0B,0xA8,0x53,0xD6,0xAC,0x6D,0xE9,0xC0,0x57,0xC5,0xB2,0xAE,0xA1,0xB0,0x49,0x0D,0x7B,0xBD,0x86,0xA2,0x47,0x35,0xE3,0xF5,0xEA,0xB2,0x4B,0x4B,0xCB,0xC7,0xA3,0xCD,0xDE,0x23,0x59,0x1A,0x9A,0x31,0x8B,0xB0,0x54,0x76,0xE3,0xC6,0x26,0x5C,0x2C,0xCC,0x76,0x6B,0x92,0xBC,0x34,0x95,0xC6,0xA3,0xCE,0x74,0xDB,0x85,0x3B,0x8F,0xBA,0x90,0x9C,0x51,0xCC,0xD6,0xEA,0x4C,0x63,0x56,0x30,0x6D,0xA9,0x23,0xCF,0x59,0xD0,0x34,0xB5,0xF9,0x7F};
const uint8_t spOVER[] PROGMEM = {0x63,0x6F,0xC4,0x7A,0x1D,0xB5,0xED,0x61,0x37,0xBB,0x6E,0x75,0x62,0xD9,0x2D,0xEC,0xBF,0x56,0xAD,0x09,0xBA,0x32,0x8C,0x13,0xC7,0xD6,0xED,0x4D,0x85,0x86,0x99,0xE3,0x3E,0xB7,0x29,0x86,0x90,0x2C,0x76,0xDB,0xE6,0x98,0x95,0xBB,0x38,0x4F,0x5B,0x72,0x29,0xB4,0x51,0x6F,0x7D,0xAF,0x47,0xB9,0x73,0x71,0x8C,0x31,0x3F,0xE1,0xC9,0xA9,0x50,0xD6,0xFD,0xBA,0x27,0x57,0xC5,0x6E,0xCD,0xFD,0xFF};
const uint8_t spSTART[] PROGMEM = {0x02,0xF8,0x49,0xCC,0x00,0xBF,0x87,0x3B,0xE0,0xB7,0x60,0x03,0xFC,0x9A,0xAA,0x80,0x3F,0x92,0x11,0x30,0x29,0x9A,0x02,0x86,0x34,0x5F,0x65,0x13,0x69,0xE2,0xDA,0x65,0x35,0x59,0x8F,0x49,0x59,0x97,0xD5,0x65,0x7D,0x29,0xA5,0xDE,0x56,0x97,0xF5,0x85,0x8E,0xE4,0x5D,0x6D,0x0E,0x23,0x39,0xDC,0x79,0xD4,0xA5,0x35,0x75,0x72,0xEF,0x51,0x95,0xE9,0x38,0xE6,0xB9,0x4B,0x5D,0x1A,0x26,0x6B,0x3B,0x46,0xE0,0x14,0xA5,0x2A,0x54,0x03,0x40,0x01,0x43,0xBA,0x31,0x60,0x73,0x35,0x04,0x4E,0x51,0xAA,0x42,0x35,0xFE,0x1F};

void setup() {
  CircuitPlayground.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(Switch), turnOff, CHANGE);
  Serial.begin(9600);
  while(!Serial);
  randomSeed(analogRead(badPin));
  Serial.println("Welcome to lights out beta!");
  Serial.println("You are the green dot, your objective is to avoid the red ones.");
  Serial.println("A yellow dot means both you and a red dot occupy the same space. Watch out!");
  Serial.println("To play use the buttons after the three ticks to move.");
  Serial.println("The left button(D4) moves you counter clockwise.");
  Serial.println("The right button(D5) moves you clockwise");
  Serial.println("You get 5 ticks(5 seconds) before the lights are back on.");
  Serial.println("You gain 100 points each time you move and 1000 per round win!");
  Serial.println("If you get caught in the same spot as a red YOU LOSE!!!");
}

void loop()
{
  if(newGame)
  {
    Serial.println("Press and hold both buttons to play!");
    while(off)
    while (!(digitalRead(4) && digitalRead(5)))
    {
      CircuitPlayground.setPixelColor(2, 13, 0, 0);
      CircuitPlayground.setPixelColor(7, 13, 0, 0);
      delay(250 - 25 * difficulty);
      CircuitPlayground.clearPixels();
      delay(250 - 25 * difficulty);
    }
    CircuitPlayground.speaker.say(spLEVEL);
    CircuitPlayground.speaker.say(spSTART);
    newGame = false;
  }
  for (int i = 0; i < enemyCount; i++)
  {
    enemyPositions[i] = random(10);
  }
  for (int i : enemyPositions)
  {
    if (playerPosition == i)
    {
      danger = true;
    }
  }
  for (int i = 0; i < 4; i++)
  {
    if (danger)
    {
      for (int i : enemyPositions)
      {
        CircuitPlayground.setPixelColor(i, 13, 0, 0);
      }
      CircuitPlayground.setPixelColor(playerPosition, 13, 13, 0);
    }
    else
    {
      for (int i : enemyPositions)
      {
        CircuitPlayground.setPixelColor(i, 13, 0, 0);
      }
      CircuitPlayground.setPixelColor(playerPosition, 0, 13, 0);
    }
    delay(500 - 50 * difficulty);
    CircuitPlayground.playTone(220, 100 - 10 * difficulty, false);
    delay(500 - 50 * difficulty);
    CircuitPlayground.clearPixels();
  }
  danger = false;
  delay_5s.start(5000 - 500 * difficulty, AsyncDelay::MILLIS);
  delay_1s.start(1000 - 100 * difficulty, AsyncDelay::MILLIS);
  while(!(delay_5s.isExpired()))
  {
    if (digitalRead(4) && !off)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      playerPosition++;
      if (playerPosition > 9)
      {
        playerPosition = 0;
      }
      points += 100;
      Serial.print("Player moved to position ");
      Serial.println(playerPosition);
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);
    }
    else if (digitalRead(5) && !off)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      playerPosition--;
      if (playerPosition < 0)
      {
        playerPosition = 9;
      }
      points += 100;
      Serial.print("Player moved to position ");
      Serial.println(playerPosition);
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);
    }
    if (delay_1s.isExpired())
    {
      CircuitPlayground.playTone(110, 100 - 10 * difficulty, false);
      delay_1s.repeat();
    }
  }
  for (int i : enemyPositions)
  {
    if (playerPosition == i)
    {
      danger = true;
    }
  }
  if (danger || off)
  {
    points = 0;
    enemyCount = 1;
    newGame = true;
    for (int i : enemyPositions)
    {
      CircuitPlayground.setPixelColor(i, 13, 0, 0);
    }
    for (int i = 0; i < 10; i++)
    {
      enemyPositions[i] = -1;
    }
    CircuitPlayground.setPixelColor(playerPosition, 13, 13, 0);
    CircuitPlayground.playTone(110, 250 - 25 * difficulty);
    CircuitPlayground.setPixelColor(playerPosition, 0, 0, 0);
    delay(250);
    CircuitPlayground.setPixelColor(playerPosition, 13, 13, 0);
    CircuitPlayground.playTone(110, 250 - 25 * difficulty);
    CircuitPlayground.setPixelColor(playerPosition, 0, 0, 0);
    delay(250);
    CircuitPlayground.setPixelColor(playerPosition, 13, 13, 0);
    CircuitPlayground.playTone(110, 250 - 25 * difficulty);
    CircuitPlayground.setPixelColor(playerPosition, 0, 0, 0);
    delay(250);
    Serial.println("GAME OVER!");
    CircuitPlayground.speaker.say(spLEVEL);
    CircuitPlayground.speaker.say(spOVER);
  }
  else
  {
    points += 1000;
    for (int i : enemyPositions)
    {
      CircuitPlayground.setPixelColor(i, 13, 0, 0);
    }
    CircuitPlayground.setPixelColor(playerPosition, 0, 13, 0);
    Serial.print("You have ");
    Serial.print(points);
    Serial.println(" Points!");
    CircuitPlayground.playTone(220, 250 - 25 * difficulty);
    CircuitPlayground.playTone(246.94, 250 - 25 * difficulty);
    CircuitPlayground.playTone(261.63, 500 - 50 * difficulty);
    if (enemyCount < 9)
    {
      enemyCount++;
    }
    else
    {
      difficulty++;
    }
  }
  danger = false;
  CircuitPlayground.clearPixels();
}
void turnOff()
{
  off = analogRead(7);
}
