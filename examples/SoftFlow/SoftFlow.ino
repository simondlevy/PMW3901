/* SoftFlow.ino PMW3901_SoftSPI optical flow example
 *
 * Additional library required: 
 *
 *     https://github.com/greiman/DigitalIO) library
 *
 * Copyright (c) 2017 Bitcraze AB, Simon D. Levy
 *
 * MIT License
 */

#include "PMW3901_SoftSPI.h"

// Software SPI pins 
static const uint8_t SOFT_MISO = 4;
static const uint8_t SOFT_MOSI = 8;
static const uint8_t SOFT_SCK  = 9;

// Chip Select
static const uint8_t CS = 10;

static PMW3901_SoftSPI<SOFT_MISO,SOFT_MOSI,SOFT_SCK> flow(CS);

void setup() 
{
  Serial.begin(115200);

  if (!flow.begin()) {
    Serial.println("Initialization of the flow sensor failed");
    while(1) { }
  }
}

void loop() 
{
  int16_t deltaX,deltaY;

  // Get motion count since last call
  flow.readMotionCount(&deltaX, &deltaY);

  Serial.print("X: ");
  Serial.print(deltaX);
  Serial.print(", Y: ");
  Serial.print(deltaY);
  Serial.print("\n");

  delay(100);
}
