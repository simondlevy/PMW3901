#include "PMW3901.hpp"

// Using digital pin 10 for chip select
static PMW3901 flow(10);

void setup() 
{
    Serial.begin(115200);

    if (!flow.begin()) {

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

    // Get motion count since last call
    flow.readMotionCount(&deltaX, &deltaY);

    Serial.print("X: ");
    Serial.print(deltaX);
    Serial.print(", Y: ");
    Serial.print(deltaY);
    Serial.print("\n");

    delay(100);
}
