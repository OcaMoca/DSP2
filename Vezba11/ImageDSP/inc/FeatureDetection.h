
#ifndef _FEATURE_DETECTION_H_
#define _FEATURE_DETECTION_H_

#include <QtGlobal>

void SIFTDetect(uchar input[], int xSize, int ySize);

void SIFTDetectPlusMatch(uchar input[], int xSize, int ySize, double threshold);

void HarisCornerDetection(uchar input[], int xSize, int ySize, double threshold);

#endif //  _FEATURE_DETECTION_H_
