#include "FeatureDetection.h"
#include "SIFTLib.h"
#include "ColorSpaces.h"
#include "ImageFilter.h"
#include <list>

using namespace std;
using namespace ezsift;

void SIFTDetect(uchar input[], int xSize, int ySize)
{
	SiftKeypointList kptList;

	uchar* Y_buff = new uchar[xSize*ySize];
	char* U_buff = new char[xSize*ySize / 4];
	char* V_buff = new char[xSize*ySize / 4];

	/* Convert input image to YUV420 image */
	RGBtoYUV420(input, xSize, ySize, Y_buff, U_buff, V_buff);

	/* Perform SIFT transformation  */
	kptList = calculateSIFT(Y_buff, xSize, ySize);
	
	/* Mark all keypoints in input image */
	markSIFTKeypointsRGB(input, xSize, ySize, kptList, 255, 0, 0);

	delete[] Y_buff;
	delete[] U_buff;
	delete[] V_buff;
}


SiftKeypointList GetSIFTFeatures(const uchar input[], int xSize, int ySize)
{
	SiftKeypointList kptList;

	uchar* Y_buff = new uchar[xSize*ySize];
	char* U_buff = new char[xSize*ySize];
	char* V_buff = new char[xSize*ySize];

	/* Convert input image to YUV420 image */
	RGBtoYUV444(input, xSize, ySize, Y_buff, U_buff, V_buff);

	delete[] U_buff;
	delete[] V_buff;

	/* Perform SIFT transformation  */
	kptList = calculateSIFT(Y_buff, xSize, ySize);

	delete[] Y_buff;

	return kptList;
}

