#ifndef _FEATURE_DRAW_H_
#define _FEATURE_DRAW_H_

#include <qglobal.h>

void setPixelValue(uchar img[], int r, int c, int w, int h, int value);

void drawCircle(uchar img[], int r, int c, int cR, int w, int h, int value);

void drawCircle(uchar img[], int r, int c, int cR, int w, int h, int value, float thickness);

void rasterCircle(uchar img[], int r, int c, int radius, int w, int h, int value);

void drawOrientation(uchar img[], int x, int y, float ori, int cR, int w, int h, int value);

void drawKeypoints(uchar img[], int w, int h, ezsift::SiftKeypointList kpt_list, int value);

void drawLine(uchar img[], int w, int h, int r1, int c1, int r2, int c2, int value);

void setPixelValueRGB(uchar img[], int r, int c, int w, int h, int R, int G, int B);

void drawCircleRGB(uchar img[], int r, int c, int cR, int w, int h, int R, int G, int B);

void rasterCircleRGB(uchar img[], int r, int c, int radius, int w, int h, int R, int G, int B);

void drawOrientationRGB(uchar img[], int x, int y, float ori, int cR, int w, int h, int R, int G, int B);

void drawKeypointsRGB(uchar img[], int w, int h, ezsift::SiftKeypointList kpt_list, int R, int G, int B);

void drawLineRGB(uchar img[], int w, int h, int r1, int c1, int r2, int c2, int R, int G, int B);

#endif