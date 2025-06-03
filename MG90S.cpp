#include "MG90S.h"

static Servo _servo;
static int _servoGpio = MG90S_DEFAULT_GPIO;

void servo_init() {
    _servo.setPeriodHertz(50);             // fréquence PWM standard pour servos
    _servo.attach(_servoGpio, 500, 2400);  // plage typique pour MG90S
}

void servo_write(int angle) {
    _servo.write(angle);
}

void servo_writeMicroseconds(int us) {
    _servo.writeMicroseconds(us);
}

void servo_detach() {
    _servo.detach();
}


// // TEST SERVO
// void loop() {
//   // Balayage test
//   for (int angle = 0; angle <= 180; angle += 10) {
//     servo_write(angle);
//     delay(300);
//   }

//   for (int angle = 180; angle >= 0; angle -= 10) {
//     servo_write(angle);
//     delay(300);
//   }
// }