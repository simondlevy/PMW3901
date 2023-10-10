#pragma once

class NewPMW3901 {

    protected:

        uint8_t _cspin;

        void begin(const uint8_t csPin)
        {
            _cspin = csPin;
        }


};
