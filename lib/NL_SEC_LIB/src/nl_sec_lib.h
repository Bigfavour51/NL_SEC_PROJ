////////////////////////////////////////////////////////////////////////////////////////////////////
//*FILE NAME:       nl_sec_lib.h
//*FILE DESC:       Header file for NL_SEC_PROJ.
//*FILE VERSION:    1.1.3b
//*FILE AUTHOR:     Aina Favour Olalekan (Bigfavour)
//*LAST MODIFIED:   Friday, 15 October, 2024
//*LICENSE:         GNU LGPL v2.1
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef NL__SEC__
#define NL__SEC__

#include <Arduino.h>
#include <SPIFFS.h>
#include <FS.h>
#include <elapsedMillis.h>

#include "WiFi.h"
#include "esp_camera.h"
#include "esp_timer.h"
#include "img_converters.h"
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"
#include <ESPAsyncWebServer.h>
#include <StringArray.h>
#include "SD_MMC.h"

// Photo File Name to save in SPIFFS
#define FILE_PHOTO "/photo.jpg"


// definitions for GPIO pins
#define PIRpin 13
#define Flashpin 4


// OV2640 camera module pins (CAMERA_MODEL_AI_THINKER)
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define fon HIGH
#define foff LOW

//METHODS:

void capturePhotoSaveSD();
void capturePhotoSaveSpiffs();
bool CheckPIRSensor();
bool checkPhoto( fs::FS &fs );
void initSD();
bool saveToSD(camera_fb_t*);
void pinTerrupt();

void connectToWifi(const char*, const char*);
void mountSPIFFS();
void configCam();
void startServer();

//ATTRIBUTES:
extern const char index_html[] PROGMEM;
extern bool status, motionDetect;
extern elapsedMillis flashTime;
extern bool flashOn;
extern AsyncWebServer server;
extern bool takeNewPhoto;
extern camera_config_t config;

extern const char* ssid;
extern const char* password;

#endif
