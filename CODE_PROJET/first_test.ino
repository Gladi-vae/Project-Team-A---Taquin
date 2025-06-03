//-------IMPORT------------------------------
#include "esp_camera.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <math.h>

//Camera


//ColorMatrix
#include "ColorMatrix.h"

//Ecran
#include "oled_display.h"

//CODE MORSE
#include "morse.h"

//RESOLUTION TAQUIN
#include <Arduino.h>
#include "TaquinSolver.h"

//Servomoteur
#include "MG90S.h"

//Task
#include "tasks.h"

//-------------------------DEFINE---------------------------------------
//Camera

#define CAMERA_MODEL_XIAO_ESP32S3 // Has PSRAM



//LED + BOUTON

#define LED_PIN 9  // D10 = GPIO9 sur XIAO ESP32S3


//------------------------VARIABLES-------------------------------------------
//Camera
bool camera_sign = false;          // Check camera status
#include "camera_pins.h"
//Résolution Taquin

// uint8_t rgb_image[MAX_HEIGHT][MAX_WIDTH][3];//Image codée en RGB888

// //Matrice 3x3 pour stocker les ID des couleurs (chaque position correspond à une zone de l'image)
// int colorMatrix[3][3];

// //Matrice 3x3 pour stocker la configuration Taquin vue de dessus
// int Taquin[3][3];

// Coord coords[3][3] = {
//   { {0, 0}, {80, 0}, {160, 0} },
//   { {0, 80}, {80, 80}, {160, 80} },
//   { {0, 160}, {80, 160}, {160, 160} }
// };

int compteur = 0;               // Compteur d'appuis
bool boutonPrecedent = HIGH;


volatile bool buttonPressed = false;
volatile unsigned long firstPressTime = 0;
volatile int pressCount = 0;

const unsigned long debounceDelay = 150;
const unsigned long doublePressWindow = 500;

volatile int interruptCount = 0;

void IRAM_ATTR handleButtonInterrupt() {
  static unsigned long lastInterruptTime = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastInterruptTime > debounceDelay) {
    interruptCount++;
    Serial.printf("Interrupt #%d\n", interruptCount); // Attention: Serial dans ISR n'est pas conseillé mais utile pour debug rapide
    if (pressCount == 0) {
      firstPressTime = currentTime;
    }
    pressCount++;
    buttonPressed = true;

    lastInterruptTime = currentTime;
  }
}



//----------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  delay(100);  // suffisant pour laisser le temps à l'USB de s'initialiser


  camera_config_t config;
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
  config.frame_size = FRAMESIZE_UXGA;
  //config.pixel_format = PIXFORMAT_JPEG; // for streaming
  config.pixel_format = PIXFORMAT_RGB565;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;
  
  //if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
//   if(config.pixel_format == PIXFORMAT_JPEG){
//     if(psramFound()){
//       config.jpeg_quality = 10;
//       config.fb_count = 2;
//       config.grab_mode = CAMERA_GRAB_LATEST;
//     } else {
//       // Limit the frame size when PSRAM is not available
//       config.frame_size = FRAMESIZE_SVGA;
//       config.fb_location = CAMERA_FB_IN_DRAM;
//     }
//   } else {
//     // Best option for face detection/recognition
//     config.frame_size = FRAMESIZE_240X240;
// #if CONFIG_IDF_TARGET_ESP32S3
//     config.fb_count = 2;
// #endif
//   }

// NEW CODE 1
if (config.pixel_format == PIXFORMAT_JPEG) {
  if (psramFound()) {
    config.jpeg_quality = 10;
    config.fb_count = 2;
    config.grab_mode = CAMERA_GRAB_LATEST;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.fb_location = CAMERA_FB_IN_DRAM;
  }
} else {
  // Configuration pour les formats RAW comme RGB565
  config.fb_count = psramFound() ? 2 : 1;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.frame_size = FRAMESIZE_QVGA; // Choisis une résolution raisonnable
  config.fb_location = CAMERA_FB_IN_PSRAM;
}
//END NEW CODE 1


  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  
  camera_sign = true; // Camera initialization check passes

  Serial.println("Puzzle board check PoC");

  // setup ECRAN OLED
  Wire.begin(); // nécessaire pour écran
  init_screen();

  // setup LED (base du transistor)
  pinMode(LED_PIN, OUTPUT);


  // // Bouton en entrée avec pull-up
  //pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButtonInterrupt, FALLING);

  // Servomoteur
  servo_init();

  // Morse
  initMorse("ABC");

}


//// LOOP TEST AFFICHAGE IMAGE SUR PYTHON
// void loop() {
//   camera_fb_t *fb = esp_camera_fb_get();
//   if (!fb) {
//     Serial.println("Camera capture failed");
//     return;
//   }

//   int width = fb->width;
//   int height = fb->height;
//   int numPixels = width * height;

//   // Marqueur de début pour synchronisation côté PC
//   Serial.write("IMGSTART", 8); // 8 octets pour signaler le début

//   // Conversion RGB565 → RGB888 et envoi
//   for (int i = 0; i < numPixels; i++) {

//     //MSB first
//     uint8_t msb = fb->buf[i * 2];
//     uint8_t lsb = fb->buf[i * 2 + 1];
//     uint16_t pixel = (msb << 8) | lsb;

//     // Conversion en RGB888
//     uint8_t r = ((pixel >> 11) & 0x1F) << 3; // 5 bits -> 8 bits
//     uint8_t g = ((pixel >> 5) & 0x3F) << 2;  // 6 bits -> 8 bits
//     uint8_t b = (pixel & 0x1F) << 3;         // 5 bits -> 8 bits

//     // Envoi du pixel converti
//     Serial.write(r);
//     Serial.write(g);
//     Serial.write(b);
//   }

//   esp_camera_fb_return(fb);

//   delay(3000); // Pause entre deux captures
// }


// void loop() {
//   camera_fb_t *fb = esp_camera_fb_get();
//   if (!fb) {
//     Serial.println("Camera capture failed");
//     return;
//   }

//   int width = fb->width;
//   int height = fb->height;
//   int numPixels = width * height;

//   //DEBUT CODE TAQUIN
//   convertRGB565toRGB888(fb->buf,fb->width,fb->height,rgb_image);

//   // Analyse des couleurs : remplissage de la matrice colorMatrix
//   int zoneSize = 20; // ou adapte selon ta grille réelle
//   fillColorMatrix(rgb_image, coords, zoneSize, colorMatrix);
//   printColorMatrix(colorMatrix);

//   esp_camera_fb_return(fb);

//   delay(3000); // Pause entre deux captures
// }

// //LOOP TEST LED CAMERA
// void loop() {
//   // Serial.println("LED ON");
//   // digitalWrite(GPIO_TEST, HIGH); // active la base → transistor ON → LED ON
//   // delay(10000);

//   digitalWrite(2, HIGH); // Met GPIO1 à HIGH (3.3V)
//   Serial.println("LE GPIO 2 EST HIGH HIGH HIGH");
//   delay(10000);

//   // Serial.println("LED OFF");
//   // digitalWrite(LED_CONTROL_GPIO, LOW);  // coupe la base → transistor OFF → LED OFF
//   // delay(1000);
// }



// void loop() {
//   digitalWrite(LED_PIN, HIGH); // Allume la LED
//   delay(6000);                 // Attendre 1 seconde
//   digitalWrite(LED_PIN, LOW);  // Éteint la LED
//   delay(6000);                 // Attendre 1 seconde
// }


//TEST INTERRUPTION


void loop() {
  if (buttonPressed) {
    unsigned long elapsed = millis() - firstPressTime;


    // 2. Double press détecté dans le délai
    if (pressCount == 2 && elapsed <= doublePressWindow) {
      Serial.println("Double pressed");
      Serial.println(pressCount);

      while (digitalRead(BUTTON_PIN) == LOW) {
        delay(10);
      }

      // Petit délai pour éviter un rebond résiduel
      delay(50);

      handleMorseInput();

      pressCount = 0;
      buttonPressed = false;
    }

    // 3. Appui simple, on attend la fenêtre de double press
    else if (pressCount == 1 && elapsed > doublePressWindow) {
      Serial.println("Single pressed");
      Serial.println(pressCount);
      game();

      pressCount = 0;
      buttonPressed = false;
    }
  }
}
