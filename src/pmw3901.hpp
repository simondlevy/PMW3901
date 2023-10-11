#pragma once

#include <Arduino.h>

#if defined(ARDUINO)
#include <SPI.h>
#endif

static void spi_begin(void)
{
#if defined(ARDUINO)
    SPI.begin();
#else
    spiBegin();
#endif
}

static void spi_begin_transaction(void)
{
#if defined(ARDUINO)
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE3));
#else
    spiBeginTransaction(SPI_BAUDRATE_2MHZ);
#endif

}

static void spi_end_transaction(void)
{
#if defined(ARDUINO)
    SPI.endTransaction();
#else
    spiEndTransaction();
#endif
}

static void spi_write_value(uint8_t value)
{
#if defined(ARDUINO)
    SPI.transfer(&value, 1);
#else
    spiExchange(1, &value, &value);
#endif
}

static void spi_read_buffer(void * buffer, const size_t size)
{
#if defined(ARDUINO)
    SPI.transfer(buffer, size);
#else
    spiExchange(size, (uint8_t *)buffer, (uint8_t *)buffer);
#endif
}


//////////////////////////////////////////////////////////////////////

class PMW3901 {

    public:

        bool begin(const uint8_t csPin=10)
        {
            _cs_pin = csPin;

            spi_begin();

            pinMode(_cs_pin, OUTPUT);

            digitalWrite(_cs_pin, HIGH);
            delay(1);
            digitalWrite(_cs_pin, LOW);
            delay(1);
            digitalWrite(_cs_pin, HIGH);
            delay(1);

            uint8_t chipId = registerRead(0x00);
            uint8_t invChipId = registerRead(0x5f);

            if (chipId == 0x49 && invChipId == 0xB6) {

                registerWrite(0x3a, 0x5a);
                delay(5);

                registerRead(0x02);
                registerRead(0x03);
                registerRead(0x04);
                registerRead(0x05);
                registerRead(0x06);
                delay(1);

                initRegisters();

                return true;
            }

            return false;
        }

        void readMotion(int16_t * deltaX, int16_t * deltaY, bool * gotMotion)
        {
            uint8_t address = 0x16;

            spi_begin_transaction();

            digitalWrite(_cs_pin,LOW);
            delayMicroseconds(50);

            spi_write_value(address);

            delayMicroseconds(50);

            spi_read_buffer(&_motion_burst, sizeof(motionBurst_t));

            delayMicroseconds(50);
            digitalWrite(_cs_pin, HIGH);

            spi_end_transaction();

            delayMicroseconds(50);

            *deltaX = _motion_burst.deltaX;
            *deltaY = _motion_burst.deltaY;
            *gotMotion = (_motion_burst.motion == 0xB0);
        }

    private:

        typedef struct motionBurst_s {

            uint8_t motion;

            uint8_t observation;

            int16_t deltaX;
            int16_t deltaY;

        } __attribute__((packed)) motionBurst_t;

        uint8_t _cs_pin;

        motionBurst_t _motion_burst;

        void registerWrite(uint8_t reg, uint8_t value)
        {
            reg |= 0x80u;

            spi_begin_transaction();

            digitalWrite(_cs_pin, LOW);

            delayMicroseconds(50);

            spi_write_value(reg);
            spi_write_value(value);

            delayMicroseconds(50);
            digitalWrite(_cs_pin, HIGH);

            spi_end_transaction();

            delayMicroseconds(200);
        }

        uint8_t registerRead(uint8_t reg)
        {
            reg &= ~0x80u;

            spi_begin_transaction();

            digitalWrite(_cs_pin, LOW);

            delayMicroseconds(50);

            spi_write_value(reg);

            delayMicroseconds(500);

            uint8_t value = 0;
            spi_read_buffer(&value, 1);

            delayMicroseconds(50);
            digitalWrite(_cs_pin, HIGH);
            delayMicroseconds(200);

            spi_end_transaction();

            delayMicroseconds(200);

            return value;
        }

        void initRegisters(void)
        {
            registerWrite(0x7F, 0x00);
            registerWrite(0x61, 0xAD);
            registerWrite(0x7F, 0x03);
            registerWrite(0x40, 0x00);
            registerWrite(0x7F, 0x05);
            registerWrite(0x41, 0xB3);
            registerWrite(0x43, 0xF1);
            registerWrite(0x45, 0x14);
            registerWrite(0x5B, 0x32);
            registerWrite(0x5F, 0x34);
            registerWrite(0x7B, 0x08);
            registerWrite(0x7F, 0x06);
            registerWrite(0x44, 0x1B);
            registerWrite(0x40, 0xBF);
            registerWrite(0x4E, 0x3F);
            registerWrite(0x7F, 0x08);
            registerWrite(0x65, 0x20);
            registerWrite(0x6A, 0x18);
            registerWrite(0x7F, 0x09);
            registerWrite(0x4F, 0xAF);
            registerWrite(0x5F, 0x40);
            registerWrite(0x48, 0x80);
            registerWrite(0x49, 0x80);
            registerWrite(0x57, 0x77);
            registerWrite(0x60, 0x78);
            registerWrite(0x61, 0x78);
            registerWrite(0x62, 0x08);
            registerWrite(0x63, 0x50);
            registerWrite(0x7F, 0x0A);
            registerWrite(0x45, 0x60);
            registerWrite(0x7F, 0x00);
            registerWrite(0x4D, 0x11);
            registerWrite(0x55, 0x80);
            registerWrite(0x74, 0x1F);
            registerWrite(0x75, 0x1F);
            registerWrite(0x4A, 0x78);
            registerWrite(0x4B, 0x78);
            registerWrite(0x44, 0x08);
            registerWrite(0x45, 0x50);
            registerWrite(0x64, 0xFF);
            registerWrite(0x65, 0x1F);
            registerWrite(0x7F, 0x14);
            registerWrite(0x65, 0x67);
            registerWrite(0x66, 0x08);
            registerWrite(0x63, 0x70);
            registerWrite(0x7F, 0x15);
            registerWrite(0x48, 0x48);
            registerWrite(0x7F, 0x07);
            registerWrite(0x41, 0x0D);
            registerWrite(0x43, 0x14);
            registerWrite(0x4B, 0x0E);
            registerWrite(0x45, 0x0F);
            registerWrite(0x44, 0x42);
            registerWrite(0x4C, 0x80);
            registerWrite(0x7F, 0x10);
            registerWrite(0x5B, 0x02);
            registerWrite(0x7F, 0x07);
            registerWrite(0x40, 0x41);
            registerWrite(0x70, 0x00);

            delay(10);

            registerWrite(0x32, 0x44);
            registerWrite(0x7F, 0x07);
            registerWrite(0x40, 0x40);
            registerWrite(0x7F, 0x06);
            registerWrite(0x62, 0xF0);
            registerWrite(0x63, 0x00);
            registerWrite(0x7F, 0x0D);
            registerWrite(0x48, 0xC0);
            registerWrite(0x6F, 0xD5);
            registerWrite(0x7F, 0x00);
            registerWrite(0x5B, 0xA0);
            registerWrite(0x4E, 0xA8);
            registerWrite(0x5A, 0x50);
            registerWrite(0x40, 0x80);

            registerWrite(0x7F, 0x00);
            registerWrite(0x5A, 0x10);
            registerWrite(0x54, 0x00);
        }

};
