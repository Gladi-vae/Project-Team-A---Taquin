#include "oled_display.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Définition des dimensions de l'écran OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Adresse I²C du SSD1315 (souvent 0x3C ou 0x3D)
#define OLED_ADDR 0x3C

// GPIOs utilisés pour I2C (selon ton branchement)
#define OLED_SDA 5
#define OLED_SCL 6

// Création de l'objet écran avec l'I2C personnalisé
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

void init_screen() {
  // Initialiser le bus I2C avec les bons GPIOs
  Wire.begin(OLED_SDA, OLED_SCL);

  // Initialiser l'écran
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("Erreur init écran OLED"));
    while (1); // Boucle infinie si erreur
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Ecran OK"));
  display.display();
  delay(1000);
  display.clearDisplay();
}

void print_screen(const String &message) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(message);
  display.display();
}



// //TEST INO
// #include "oled_display.h"

// void setup() {
//   Serial.begin(115200);
//   init_screen();
//   print_screen("Pret");
// }

// void loop() {
//   // Exemple d'affichage toutes les 3s
//   print_screen("Salut !");
//   delay(3000);
//   print_screen("Test");
//   delay(3000);
// }
