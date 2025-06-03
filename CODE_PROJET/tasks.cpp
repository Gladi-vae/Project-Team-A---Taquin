//Camera
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

int number_help = 3;

uint8_t rgb_image[MAX_HEIGHT][MAX_WIDTH][3];//Image codée en RGB888

//Matrice 3x3 pour stocker les ID des couleurs (chaque position correspond à une zone de l'image)
int colorMatrix[3][3];
void inverserVueArriere(int matrix[3][3]) {
  for (int i = 0; i < 3; i++) {
    // On échange la colonne 0 et la colonne 2 pour chaque ligne
    int temp = matrix[i][0];
    matrix[i][0] = matrix[i][2];
    matrix[i][2] = temp;
  }
}

//Matrice 3x3 pour stocker la configuration Taquin vue de dessus
int Taquin[3][3];

Coord coords[3][3] = {
  { {83, 47}, {145, 40}, {207, 40} },
  { {86, 109}, {151, 107}, {211, 100} },
  { {89, 177}, {142, 172}, {232, 170} }
};

int zoneSize = 15;

//Etat objectif : taquin ordonné classique
  std::vector<uint8_t> goal = {
    1, 2, 3,
    4, 5, 6,
    7, 8, EMPTY
  };

std::vector<uint8_t> flattenMatrix(int matrix[3][3]) {
  std::vector<uint8_t> result;
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      result.push_back(static_cast<uint8_t>(matrix[i][j]));
  return result;
}



void game(){
  if (number_help>0){
    number_help=number_help-1;
    print_screen("DEBUT PHOTO");
    digitalWrite(LED_PIN, HIGH); // Allume la LED
    camera_fb_t *fb = esp_camera_fb_get();
    convertRGB565toRGB888(fb->buf,fb->width,fb->height,rgb_image);
    fillColorMatrix(rgb_image, coords, zoneSize, colorMatrix);

  // Marqueur de début pour synchronisation côté PC
  Serial.write("IMGSTART", 8); // 8 octets pour signaler le début

int width = fb->width;
int height = fb->height;
int numPixels = width * height;
  // Conversion RGB565 → RGB888 et envoi
  for (int i = 0; i < numPixels; i++) {

    //MSB first
    uint8_t msb = fb->buf[i * 2];
    uint8_t lsb = fb->buf[i * 2 + 1];
    uint16_t pixel = (msb << 8) | lsb;

    // Conversion en RGB888
    uint8_t r = ((pixel >> 11) & 0x1F) << 3; // 5 bits -> 8 bits
    uint8_t g = ((pixel >> 5) & 0x3F) << 2;  // 6 bits -> 8 bits
    uint8_t b = (pixel & 0x1F) << 3;         // 5 bits -> 8 bits

    // Envoi du pixel converti
    Serial.write(r);
    Serial.write(g);
    Serial.write(b);
  }

    esp_camera_fb_return(fb);

// Avant inversion
Serial.println("MATRIX_BEFORE");
for (int i = 0; i < 3; i++) {
  for (int j = 0; j < 3; j++) {
    Serial.print(colorMatrix[i][j]);
    if (j < 2) Serial.print(",");
  }
  Serial.println();
}
Serial.println("MATRIX_BEFORE_END");


    //Affiche colorMatrix dans serial ici (avant inversion)
    inverserVueArriere(colorMatrix);

// Après inversion
Serial.println("MATRIX_AFTER");
for (int i = 0; i < 3; i++) {
  for (int j = 0; j < 3; j++) {
    Serial.print(colorMatrix[i][j]);
    if (j < 2) Serial.print(",");
  }
  Serial.println();
}
Serial.println("MATRIX_AFTER_END");


    //Affiche colormatrix dans serial ici (après inversion)
    std::vector<uint8_t> start = flattenMatrix(colorMatrix);
    get_first_move(start, goal);

    delay(2000);
    print_screen("FIN PHOTO");
    digitalWrite(LED_PIN, LOW);  // Éteint la LED

  }
  else{
    print_screen("Plus d'aide!");
  }
}