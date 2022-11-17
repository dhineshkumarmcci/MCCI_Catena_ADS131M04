/*

Module: ads131m04_simple.ino

Function:
    Simple example for MCCI Catena ADS131M04 library.

Copyright and License:
    See accompanying LICENSE file for copyright and license information.

Author:
    Pranau R, MCCI Corporation   November 2022

*/

#include <MCCI_Catena_ADS131M04.h>

#include <Arduino.h>

/****************************************************************************\
|
|   Manifest constants & typedefs.
|
\****************************************************************************/

using namespace McciCatenaAds131m04;

/****************************************************************************\
|
|   Variables.
|
\****************************************************************************/

SPIClass gSPI2(
    Catena::PIN_SPI2_MOSI,
    Catena::PIN_SPI2_MISO,
    Catena::PIN_SPI2_SCK
    );

cADS131M04 ads = cADS131M04(D5, D12, &gSPI2);

/****************************************************************************\
|
|   Code.
|
\****************************************************************************/

/*
Name:   setup()
Function:
        Initilize ADS131M04 for ADC conversion.
Definition:
        void setup(
            void
            );
Description:
        To initiate a serial connection between board and display and to initiate the connectivity of ADS131M04 four channel ADC.
Returns:
        No explicit result.
*/

void setup()
    {
    gSPI2.begin();

    ads.begin();

    Serial.println("Initialised! Reading all Registers:");

    // Set chip select pins to high
    pinMode(D5, OUTPUT);
    digitalWrite(D5, HIGH);
    }

/*
Name:   loop()
Function:
        To get raw ADC readings and to display it in serial monitor.
Definition:
        void loop (
            void
            );
Returns:
        No explicit result.
*/

void loop()
    {
    uint8_t channel0 = 0;
    uint8_t channel1 = 1;
    uint8_t channel2 = 2;
    uint8_t channel3 = 3;
    float FSR = 2.4;
    float BITS = 16777215;

    Serial.print("CH0(HEX)          : ");
    Serial.println(ads.readSingleChannel(channel0), HEX);
    float code = ads.readSingleChannel(channel0);
    Serial.print("CODE(DEC)         : ");
    Serial.println(code);

    Serial.print("CH1(HEX)          : ");
    Serial.println(ads.readSingleChannel(channel1), HEX);
    float code = ads.readSingleChannel(channel1);
    Serial.print("CODE(DEC)         : ");
    Serial.println(code);

    Serial.print("CH2(HEX)          : ");
    Serial.println(ads.readSingleChannel(channel2), HEX);
    float code = ads.readSingleChannel(channel2);
    Serial.print("CODE(DEC)         : ");
    Serial.println(code);

    Serial.print("CH3(HEX)          : ");
    Serial.println(ads.readSingleChannel(channel3), HEX);
    float code = ads.readSingleChannel(channel3);
    Serial.print("CODE(DEC)         : ");
    Serial.println(code);

    Serial.println("");
    Serial.println("####################################");
    Serial.println("");
    }
