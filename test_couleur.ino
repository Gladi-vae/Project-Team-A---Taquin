#include <Wire.h>
#include "DFRobot_TCS34725.h"

#define I2C_ADDRESS 0x29  // Adresse du SEN0212

DFRobot_TCS34725 tcs(&Wire, I2C_ADDRESS, TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_4X);

void setup() {
    Serial.begin(9600);
    Wire.begin();

    Serial.println("🔍 Initialisation du capteur SEN0212...");

    if (!tcs.begin()) {
        Serial.println("❌ Capteur non détecté. Vérifiez les connexions !");
        while (1);  // Stoppe le programme si le capteur n'est pas détecté
    }

    Serial.println("✅ Capteur détecté !");
}

void loop() {
    // Variables pour stocker les couleurs
    uint16_t r, g, b, c;

    // Lire les valeurs du capteur
    tcs.getRGBC(&r, &g, &b, &c);

    // Normalisation des valeurs sur 0-255
    float scale = 255.0 / c;
    uint8_t red = constrain(r * scale, 0, 255);
    uint8_t green = constrain(g * scale, 0, 255);
    uint8_t blue = constrain(b * scale, 0, 255);

    // Affichage des valeurs
    Serial.print("🔴 R: "); Serial.print(red);
    Serial.print(" 🟢 G: "); Serial.print(green);
    Serial.print(" 🔵 B: "); Serial.println(blue);
    
    delay(500);
}

