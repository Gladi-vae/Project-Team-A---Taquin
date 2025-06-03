#ifndef TASKS_H
#define TASKS_H

#include "esp_camera.h"

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

#define LED_PIN 9  // D10 = GPIO9 sur XIAO ESP32S3


void game();

#endif // OLED_DISPLAY_H