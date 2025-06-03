#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Définition des dimensions de l'écran
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Adresse I²C du SSD1315
#define OLED_ADDR 0x3C

// Broches I2C personnalisées pour l'ESP32-S3
#define OLED_SDA 5
#define OLED_SCL 6

// Déclaration de l'objet écran (instancié dans oled_display.cpp)
extern Adafruit_SSD1306 display;

// Prototypes des fonctions
void init_screen();
void print_screen(const String &message);

#endif // OLED_DISPLAY_H

