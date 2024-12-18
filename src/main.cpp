////////////////////////////////////////////////////////////////////////////////////////////////////
//*FILE NAME:       main.cpp
//*FILE DESC:       Main source file for NL_SEC_PROJ.
//*FILE VERSION:    1.1.3
//*FILE AUTHOR:     Aina Favour Olalekan (Bigfavour)
//*LAST MODIFIED:   Friday, 15 October, 2024
//*LICENSE:         GNU LGPL v2.1
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Arduino.h"
#include <nl_sec_lib.h>

elapsedMillis flashTime;

const char* ssid = "Norahlinks";
const char* password = "Norahlinks";

void setup()
{
    Serial.begin(115200);
    pinMode(PIRpin, INPUT);
    pinMode(Flashpin, OUTPUT);
    attachInterrupt(PIRpin, pinTerrupt, RISING);

    for(int i = 0; i < 3; ++i)
    {
        digitalWrite(Flashpin, fon);
        delay(400);
        digitalWrite(Flashpin, foff);
        delay(400);
    }


    // Connect to Wi-Fi
    connectToWifi(ssid, password);

    mountSPIFFS();



    // Print ESP32 Local IP Address
    Serial.print("IP Address: http://");
    Serial.println(WiFi.localIP());

    // Turn-off the 'brownout detector'
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

    configCam();

    startServer();

    //Init SD Card:
//    initSD();

    digitalWrite(Flashpin, fon);
    delay(800);
    digitalWrite(Flashpin, foff);

}

void loop()
{
    if (CheckPIRSensor())
    {
        Serial.println("Motion detected");
        capturePhotoSaveSpiffs();

    }

    else
    {
        Serial.println("Motion not detected");
        digitalWrite(Flashpin, foff);
    }
    delay(400);
}

