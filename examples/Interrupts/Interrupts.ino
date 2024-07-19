
/*
 * Interrupt-based reading for PMW3901 optical flow sensor
 *
 * Copyright 2024 Simon D. Levy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PMW3901 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PMW3901.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "pmw3901.hpp"

#include <SPI.h>

static PMW3901 sensor;

static const uint8_t INTERRUPT_PIN = 9;

static volatile bool gotInterrupt;

static void interruptHandler(void)
{
    gotInterrupt = true;
}

void setup() 
{
    Serial.begin(115200);

    attachInterrupt(
        digitalPinToInterrupt(INTERRUPT_PIN), interruptHandler, FALLING);

    SPI.begin();

    if (!sensor.begin()) {

        while(true) { 
            Serial.println("Initialization of the flow sensor failed");
            delay(500);
        }
    }
}

void loop() 
{
    if (gotInterrupt) {

        gotInterrupt = false;

        int16_t deltaX = 0;
        int16_t deltaY = 0;
        bool gotMotion = false;

        sensor.readMotion(10, deltaX, deltaY, gotMotion); 

        Serial.print("deltaX: ");
        Serial.print(deltaX);
        Serial.print(",\tdeltaY: ");
        Serial.println(deltaY);

    }
}
