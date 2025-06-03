#ifndef COLORMATRIX_H
#define COLORMATRIX_H

#include <stdint.h>
#include <Arduino.h>


// Définitions configurables
#define MAX_WIDTH  320
#define MAX_HEIGHT 240

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} RGB;

typedef struct {
  int x;
  int y;
} Coord;

// Prototypes des fonctions
float colorDistance(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2);

int findClosestColor(RGB avgColor);

RGB getAverageRGB(uint8_t rgb_image[MAX_HEIGHT][MAX_WIDTH][3], int x, int y, int c);

void fillColorMatrix(uint8_t rgb_image[MAX_HEIGHT][MAX_WIDTH][3], Coord coords[3][3], int zoneSize, int colorMatrix[3][3]);

void convertRGB565toRGB888(const uint8_t* buf, int width, int height, uint8_t rgb_image[MAX_HEIGHT][MAX_WIDTH][3]);

void printColorMatrix(int colorMatrix[3][3]);

#endif // COLORMATRIX_H
