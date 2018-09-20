/* flow.ino PMW3901_SoftSPI optical flow example
 *
 * Additional library required: 
 *
 *     https://github.com/greiman/DigitalIO) library
 *
 * Copyright (c) 2017 Bitcraze AB, Simon D. Levy
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "PMW3901_SoftSPI.h"

// Software SPI pins 
static const uint8_t SOFT_MISO = 4;
static const uint8_t SOFT_MOSI = 8;
static const uint8_t SOFT_SCK  = 9;

// Chip Select
static const uint8_t CS = 10;

PMW3901_SoftSPI<SOFT_MISO,SOFT_MOSI,SOFT_SCK> flow(CS);

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
