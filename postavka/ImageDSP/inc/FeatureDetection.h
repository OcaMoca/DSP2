
#ifndef _FEATURE_DETECTION_H_
#define _FEATURE_DETECTION_H_

#include <QtGlobal>
#include "SIFTLib.h"

void SIFTDetect(uchar input[], int xSize, int ySize);

SiftKeypointList GetSIFTFeatures(const uchar input[], int xSize, int ySize);

#endif //  _FEATURE_DETECTION_H_
