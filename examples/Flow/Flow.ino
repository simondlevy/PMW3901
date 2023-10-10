#include "pmw3901.hpp"

// Using digital pin 10 for chip select
static PMW3901 sensor(10);

static PMW3901::motionBurst_t motion;

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

    sensor.readMotion(&motion); 

    Serial.print("X: ");
    Serial.print(motion.deltaX);
    Serial.print(", Y: ");
    Serial.print(motion.deltaY);
    Serial.print("\n");

    delay(100);
}
