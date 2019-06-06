
#ifndef _K_MEANS_
#define _K_MEANS_

#include <QtGlobal>
#include <vector>

typedef struct
{
	int x;
	int y;
	std::vector<double> feature;
} KMeansPoint;


void ColorBasedKMeans(uchar input[], int xSize, int ySize, int K);

void IntensityPlusPositionBasedKMeans(uchar input[], int xSize, int ySize, int K);

void ColorPlusPositionBasedKMeans(uchar input[], int xSize, int ySize, int K);

void SIFTBasedKMeans(uchar input[], int xSize, int ySize, int K);


#endif //  _K_MEANS_
