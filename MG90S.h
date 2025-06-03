#ifndef MG90S_H
#define MG90S_H

#include <ESP32Servo.h>

//GPIO par défaut utilisé par le servo
#define MG90S_DEFAULT_GPIO 2

void servo_init();
void servo_write(int angle);              // angle en degrés (0 à 180)
void servo_writeMicroseconds(int us);     // impulsion en microsecondes (ex: 1500)
void servo_detach();

#endif
