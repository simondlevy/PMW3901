#include "pmw3901_arduino.hpp"

// Using digital pin 10 for chip select
static PMW3901_Arduino sensor;

void setup() 
{
    Serial.begin(115200);

    if (!sensor.begin()) {

        while(true) { 
            Serial.println("Initialization of the flow sensor failed");
            delay(500);
        }
    }
}

void loop() 
{
    int16_t deltaX = 0;
    int16_t deltaY = 0;
    bool gotMotion = false;

    sensor.readMotion(&deltaX, &deltaY, &gotMotion); 

    Serial.print("deltaX: ");
    Serial.print(deltaX);
    Serial.print(",\tdeltaY: ");
    Serial.print(deltaY);
    Serial.print(",\tgotMotion: ");
    Serial.println(gotMotion ? "yes" : "no");

    delay(100);
}
