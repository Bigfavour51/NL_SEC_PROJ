////////////////////////////////////////////////////////////////////////////////////////////////////
//*FILE NAME:       nl_sec_lib.cpp
//*FILE DESC:       Implementation file for NL_SEC_PROJ.
//*FILE VERSION:    1.1.3b
//*FILE AUTHOR:     Aina Favour Olalekan (Bigfavour)
//*LAST MODIFIED:   Friday, 15 October, 2024
//*LICENSE:         GNU LGPL v2.1
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <Arduino.h>
#include <nl_sec_lib.h>

//ATTRIBUTES:
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

camera_config_t config;
bool takeNewPhoto = true;
bool motionDetect = false;
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { text-align:center; }
    .vert { margin-bottom: 10%; }
    .hori{ margin-bottom: 0%; }
  </style>
</head>
<body>
  <div id="container">
    <h2>NORAHLINKS--SECUREVIEW</h2>
    <p>It might take more than 5 seconds to capture a photo. Keep refreshing!</p>
    <p>
      <button onclick="rotatePhoto();">ROTATE</button>
      <button onclick="capturePhoto()">CAPTURE PHOTO</button>
      <button onclick="location.reload();">REFRESH PAGE</button>
    </p>
    <p id="status">Waiting for action...</p> <!-- Feedback message -->
  </div>
  <div><img src="saved-photo" id="photo" width="70%"></div>
</body>
<script>
  var deg = 0;

  function capturePhoto() {
    var status = document.getElementById("status");
    status.innerHTML = "Taking Photo...";  // Set feedback message when capturing starts

    var xhr = new XMLHttpRequest();
    xhr.open('GET', "/capture", true);

    xhr.onreadystatechange = function() {
      if (xhr.readyState == 4 && xhr.status == 200) {
        status.innerHTML = "Photo Captured!";  // Update message once capture is done
        document.getElementById("photo").src = "saved-photo?time=" + new Date().getTime(); // Refresh the photo
      } else if (xhr.readyState == 4 && xhr.status != 200) {
        status.innerHTML = "Failed to Capture Photo.";
      }
    };
    xhr.send();
  }

  function rotatePhoto() {
    var img = document.getElementById("photo");
    deg += 90;
    if(isOdd(deg/90)){ document.getElementById("container").className = "vert"; }
    else{ document.getElementById("container").className = "hori"; }
    img.style.transform = "rotate(" + deg + "deg)";
  }

  function isOdd(n) { return Math.abs(n % 2) == 1; }
</script>
</html>

)rawliteral";

bool status = false;
bool flashOn = false;
//unsigned long flashTime = 0;


//METHODS:
void capturePhotoSaveSD()
{
  camera_fb_t *fb = NULL;  // Pointer to frame buffer
  bool photoSaved = false; // Flag to check if photo was saved successfully

  //Edit begin:

  //Edit end**

  Serial.println("Capturing photo...");

  // Attempt to take a photo and save to SPIFFS
  while (!photoSaved)
  {
//    while (flashTime < 800)
//    {
//        if(fb == NULL)
//            fb = esp_camera_fb_get(); // Capture photo with the camera
//        digitalWrite(Flashpin, fon);
//    }

    digitalWrite(Flashpin, fon);
    fb = esp_camera_fb_get();
    delay(800);
    digitalWrite(Flashpin, foff);


    if (!fb)
    {
      Serial.println("Camera capture failed");
      return;
    }
    //Save to SPIFFS
//    File file = SPIFFS.open(FILE_PHOTO, FILE_WRITE); // Open file for writing
//    if (!file)
//    {
//      Serial.println("Failed to open file for writing");
//      esp_camera_fb_return(fb); // Return the frame buffer to avoid memory leak
//      return;
//    }
//
//    // Write the image buffer to the file
//    file.write(fb->buf, fb->len);
//    Serial.printf("Saved picture to: %s, size: %u bytes\n", FILE_PHOTO, file.size());
//
//    file.close(); // Close the file

    if(saveToSD(fb))
    {
        Serial.println("Saved photo to sd card.");
    }

    else
    {
        Serial.println("Unable to save to sd card.");
    }

//    esp_camera_fb_return(fb);

    // Check if the photo was successfully saved
//    if (checkPhoto(SPIFFS))
//    {
//      photoSaved = true; // Exit the loop when photo is successfully saved
//      Serial.println("Photo saved to SPI successfully!");
//    }
//
//    else
//    {
//      Serial.println("Failed to save photo, retrying...");
//    }


  }
}

void capturePhotoSaveSpiffs()
{
  camera_fb_t *fb = NULL;  // Pointer to frame buffer
  bool photoSaved = false; // Flag to check if photo was saved successfully

  //Edit begin:

  //Edit end**

  Serial.println("Capturing photo...");

  // Attempt to take a photo and save to SPIFFS
  while (!photoSaved)
  {

    takeImg:
    digitalWrite(Flashpin, fon);
    delay(200);
    fb = esp_camera_fb_get();
    digitalWrite(Flashpin, foff);
    delay(200);


    if (!fb)
    {
      Serial.println("Camera capture failed");
      return;
    }
    //Save to SPIFFS
    File file = SPIFFS.open(FILE_PHOTO, FILE_WRITE); // Open file for writing
    if (!file)
    {
      Serial.println("Failed to open file for writing");
      esp_camera_fb_return(fb); // Return the frame buffer to avoid memory leak
      return;
    }

    // Write the image buffer to the file
    file.write(fb->buf, fb->len);
    Serial.printf("Saved picture to: %s, size: %u bytes\n", FILE_PHOTO, file.size());

    file.close(); // Close the file

//    if(saveToSD(fb))
//    {
//        Serial.println("Saved photo to sd card.");
//    }
//
//    else
//    {
//        Serial.println("Unable to save to sd card.");
//    }

    esp_camera_fb_return(fb);

    // Check if the photo was successfully saved
    if (checkPhoto(SPIFFS))
    {
      photoSaved = true; // Exit the loop when photo is successfully saved
      Serial.println("Photo saved to SPI successfully!");
    }

    else
    {
      Serial.println("Failed to save photo, retrying...");
    }


  }
}


bool checkPhoto( fs::FS &fs )
{
  File f_pic = fs.open( FILE_PHOTO );
  unsigned int pic_sz = f_pic.size();
  return ( pic_sz > 100 );
}

void pinTerrupt()
{
    motionDetect = true;
}

bool CheckPIRSensor()
{
    bool state = false;
    if(motionDetect)
    {
        state = true;
        motionDetect = false;
    }
    return state;
}

void connectToWifi(const char* _ssid, const char* _pass)
{
    WiFi.begin(_ssid, _pass);
//    if(WiFi.status() != WL_CONNECTED)
//    {
//        Serial.println("Unable to connect to WiFi");
//    }
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
}

void mountSPIFFS()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        ESP.restart();
    }

    else
    {
        delay(500);
        Serial.println("SPIFFS mounted successfully");
    }
}


void configCam()
{
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.grab_mode = CAMERA_GRAB_LATEST;

    if (psramFound())
    {
        config.frame_size = FRAMESIZE_SVGA;
        config.fb_location = CAMERA_FB_IN_PSRAM;
        config.jpeg_quality = 10;
        config.fb_count = 1;
    }

    else
    {
        config.frame_size = FRAMESIZE_VGA;
        config.jpeg_quality = 10;
        config.fb_count = 1;
    }


    // Camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK)
    {
        Serial.printf("Camera init failed with error 0x%x", err);
        ESP.restart();
    }
}

void startServer()
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest * request)
                                                                {
                                                                    request->send_P(200, "text/html", index_html);
                                                                }
                                                        );

    server.on("/capture", HTTP_GET, [](AsyncWebServerRequest * request)
    {
        takeNewPhoto = true;
        // Simulate photo capture delay
        delay(800); // Optional: simulate the delay of taking a picture

        // Return success response
        if (checkPhoto(SPIFFS))
        {
            request->send(200, "text/plain", "Photo Captured Successfully");
        }
        else
        {
            request->send(500, "text/plain", "Photo Capture Failed");
        }

    });


    server.on("/saved-photo", HTTP_GET, [](AsyncWebServerRequest * request)
    {
        request->send(SPIFFS, FILE_PHOTO, "image/jpg", false);
    });

    // Start server
    server.begin();
}

bool saveToSD(camera_fb_t *_fb)
{
    bool state = false;
//    //Open SD card in write mode
//    fs::FS &fs = SD_MMC;
//    File file = fs.open(FILE_PHOTO, FILE_WRITE);
//    state = (file ? true : false);
//
//    //Save the image by passing the frame buffer to the write method
//    file.write(_fb->buf, _fb->len);
//
//    //Close the file
//    file.close();
//    //Clear the frame buffer so that it is available to use for next photo
//    esp_camera_fb_return(_fb);
    return state;
}

void initSD()
{
    if(SD_MMC.begin())
        Serial.println("SDCard Initialisation Succesful.");
    else
        Serial.println("Failed to initialise SD card.");
}
