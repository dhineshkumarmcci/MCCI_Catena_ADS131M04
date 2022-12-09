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

#include <Catena.h>
#include <Arduino.h>

/****************************************************************************\
|
|   Manifest constants & typedefs.
|
\****************************************************************************/

using namespace McciCatenaAds131m04;
using namespace McciCatena;

/****************************************************************************\
|
|   Variables.
|
\****************************************************************************/

// the primary object
Catena gCatena;

SPIClass gSPI2(
    Catena::PIN_SPI2_MOSI,
    Catena::PIN_SPI2_MISO,
    Catena::PIN_SPI2_SCK
    );

cADS131M04 gAds;

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
    Serial.println("**** This is an example program to read RAW ADC data using ADS131M04 ****");

    gCatena.begin();
    gSPI2.begin();

    if(! gAds.begin(&gSPI2, D16, D17))
        {
        Serial.println("ADS131M04 begin failed");
        while(1);
        }
    else
        {
        Serial.println("ADS131M04 begin successfully!");

        // Set chip select pins to high
        pinMode(D5, OUTPUT);
        digitalWrite(D5, HIGH);
        }
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
    float voltage;
    float concentration;

    if (gAds.isDataReady())
        {
        voltage = gAds.readVoltage(channel0);
        Serial.print("CH0 Voltage IN    : ");
        Serial.print(voltage, 3);
        Serial.println(" V");
        concentration = gAds.readCO(channel0);
        Serial.print("CO2 concentration : ");
        Serial.print(concentration, 3);
        Serial.println(" ppm");
        Serial.println("");

        voltage = gAds.readVoltage(channel1);
        Serial.print("CH2 Voltage IN    : ");
        Serial.print(voltage, 3);
        Serial.println(" V");
        concentration = gAds.readNO2(channel1);
        Serial.print("NO2 concentration : ");
        Serial.print(concentration, 3);
        Serial.println(" ppm");
        Serial.println("");

        voltage = gAds.readVoltage(channel2);
        Serial.print("CH3 Voltage IN    : ");
        Serial.print(voltage, 3);
        Serial.println(" V");
        concentration = gAds.readO3(channel2);
        Serial.print("O3  concentration : ");
        Serial.print(concentration, 3);
        Serial.println(" ppm");
        Serial.println("");

        voltage = gAds.readVoltage(channel3);
        Serial.print("CH4 Voltage IN    : ");
        Serial.print(voltage, 3);
        Serial.println(" V");
        concentration = gAds.readSO2(channel3);
        Serial.print("SO2  concentration : ");
        Serial.print(concentration, 3);
        Serial.println(" ppm");
        Serial.println("");

        Serial.println("");
        Serial.println("####################################");
        Serial.println("");

        delay(1000);
        }
    }