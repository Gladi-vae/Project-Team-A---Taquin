#ifndef MORSE_H
#define MORSE_H

#include <Arduino.h>
#include "oled_display.h"

#define BUTTON_PIN 4  // à adapter

// Prototypes
void handleMorseInput();
void initMorse(const String &secret);

#endif
