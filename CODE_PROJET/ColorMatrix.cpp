#include "ColorMatrix.h"
#include <math.h>

// Table de couleurs de référence avec ID
struct ColorRef {
  const char* name;
  uint8_t r;
  uint8_t g;
  uint8_t b;
  int id;
};

static ColorRef colorTable[] = {
  {"Rouge",   220,  38,  38, 1},
  {"Vert",     34, 197,  94, 2},
  {"Bleu",     59, 130, 246, 3},
  {"Jaune",   253, 224,  71, 4},
  {"Orange",  249, 115,  22, 5},
  {"Violet",  147,  51, 234, 6},
  {"Cyan",     34, 211, 238, 7},
  {"Blanc",   255, 255, 255, 8},
  {"Noir",      0,   0,   0, 9}
};



static const int numColors = sizeof(colorTable) / sizeof(ColorRef);

float colorDistance(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2) {
  return sqrt(pow(r1 - r2, 2) + pow(g1 - g2, 2) + pow(b1 - b2, 2));
}

//Trouve l'ID de la couleur la plus proche dans le table à partir d'un objet RGB
int findClosestColor(RGB avgColor) {
  float minDist = 999999.0;
  int closestId = -1;

  for (int i = 0; i < numColors; i++) {
    float dist = colorDistance(avgColor.r, avgColor.g, avgColor.b,
                               colorTable[i].r, colorTable[i].g, colorTable[i].b);
    if (dist < minDist) {
      minDist = dist;
      closestId = colorTable[i].id;
    }
  }
  return closestId;
}

RGB getAverageRGB(uint8_t rgb_image[MAX_HEIGHT][MAX_WIDTH][3], int x, int y, int c) {
  uint32_t sumR = 0, sumG = 0, sumB = 0;
  int count = 0;

  for (int dy = 0; dy < c; dy++) {
    int py = y + dy;
    if (py >= MAX_HEIGHT) continue;

    for (int dx = 0; dx < c; dx++) {
      int px = x + dx;
      if (px >= MAX_WIDTH) continue;

      sumR += rgb_image[py][px][0];
      sumG += rgb_image[py][px][1];
      sumB += rgb_image[py][px][2];
      count++;
    }
  }

  RGB result;
  if (count > 0) {
    result.r = sumR / count;
    result.g = sumG / count;
    result.b = sumB / count;
  } else {
    result.r = 0;
    result.g = 0;
    result.b = 0;
  }
  return result;
}

void fillColorMatrix(uint8_t rgb_image[MAX_HEIGHT][MAX_WIDTH][3], Coord coords[3][3], int zoneSize, int colorMatrix[3][3]) {
  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
      int x = coords[row][col].x; // la structure coords est init dans le .ino
      int y = coords[row][col].y;

      RGB avgColor = getAverageRGB(rgb_image, x, y, zoneSize);
      int colorId = findClosestColor(avgColor);
      colorMatrix[row][col] = colorId;
    }
  }
}

void convertRGB565toRGB888(const uint8_t* buf, int width, int height, uint8_t rgb_image[MAX_HEIGHT][MAX_WIDTH][3]) {
  int numPixels = width * height;
  for (int i = 0; i < numPixels; i++) {
    uint8_t msb = buf[i * 2];
    uint8_t lsb = buf[i * 2 + 1];
    uint16_t pixel = (msb << 8) | lsb;

    rgb_image[i / width][i % width][0] = ((pixel >> 11) & 0x1F) << 3;
    rgb_image[i / width][i % width][1] = ((pixel >> 5) & 0x3F) << 2;
    rgb_image[i / width][i % width][2] = (pixel & 0x1F) << 3;
  }
}

void printColorMatrix(int colorMatrix[3][3]) {
  Serial.println("Contenu de colorMatrix[3][3] :");
  for (int y = 0; y < 3; y++) {
    for (int x = 0; x < 3; x++) {
      Serial.print(colorMatrix[y][x]);
      Serial.print("\t"); // tabulation pour aligner proprement
    }
    Serial.println(); // nouvelle ligne après chaque ligne de la matrice
  }
  Serial.println(); // ligne vide pour séparer
}
