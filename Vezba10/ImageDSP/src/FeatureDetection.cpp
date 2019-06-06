#include "FeatureDetection.h"
#include "SIFTLib.h"
#include "ColorSpaces.h"
#include "ImageFilter.h"
#include <list>

using namespace std;

/*******************************************************************************************************************************/
/* Find SIFT keypoints and mark each one with red color*/
/*******************************************************************************************************************************/
void SIFTDetect(uchar input[], int xSize, int ySize)
{
	SiftKeypointList kptList;

	/* Convert input image to YUV420 image */
	/* TO DO*/
	uchar* Y_buff = new uchar[xSize*ySize];
	char* U_buff = new char[xSize*ySize/4];
	char* V_buff = new char[xSize*ySize/4];

	RGBtoYUV420(input, xSize, ySize, Y_buff, U_buff, V_buff);

	/* Perform SIFT transformation  */
	/* TO DO*/

	kptList = calculateSIFT(Y_buff, xSize, ySize);

	/* Mark all keypoints in input image */
	/* TO DO*/
	uchar R = 0, G=0, B=0;
	markSIFTKeypointsRGB(input, xSize, ySize, kptList, R, G, B);

	delete[] Y_buff;
	delete[] U_buff;
	delete[] V_buff;
}

/*******************************************************************************************************************************/
/* Helper function. Splits image feature set in half (used for "two image" input)  
/*******************************************************************************************************************************/
void splitFeatures(const SiftKeypointList& in, int width, SiftKeypointList& leftImageKP, SiftKeypointList& rightImageKP)
{
	for (SiftKeypoint kp : in)
	{
		if (kp.c < width / 2)
			leftImageKP.push_back(kp);
		else
			rightImageKP.push_back(kp);
	}
}

/*******************************************************************************************************************************/
/* Calculate Euclidean between two descriptor vectors
/*******************************************************************************************************************************/
double l2Distance(SiftKeypoint kp1, SiftKeypoint kp2)
{
	/* TO DO */
	float sum = 0;
	for (int i = 0; i < DEGREE_OF_DESCRIPTORS; i++) {
		sum += pow(kp1.descriptors[i] - kp2.descriptors[i], 2);
	}

	return sqrt(sum);
}

/*******************************************************************************************************************************/
/* Go through the first set of keypoints, and for each keypoint try to find the match in the second set
/* One feature matches the other if Euclidean distance between them is lower than threshold. 
/*******************************************************************************************************************************/
void matchFeatures(SiftKeypointList leftImageKP, SiftKeypointList rightImageKP, list<pair<QPoint, QPoint>>& matchPairs, double threshold)
{
	/* TO DO */
	for (SiftKeypoint kl : leftImageKP) {
		for (SiftKeypoint kr : rightImageKP) {
			if (l2Distance(kl, kr) < threshold) {
				matchPairs.push_back(pair<QPoint, QPoint>(QPoint(kl.r, kl.c), QPoint(kr.r, kr.c)));
			}
		}
	}
}

/*******************************************************************************************************************************/
/* Find SIFT keypoints, split the image in half, match the features from one image with those from another
/* one and connect them with green lines
/*******************************************************************************************************************************/
void SIFTDetectPlusMatch(uchar input[], int xSize, int ySize, double threshold)
{
	SiftKeypointList kptList, kptListLeft, kptListRight;

	uchar* Y_buff = new uchar[xSize*ySize];
	char* U_buff = new char[xSize*ySize / 4];
	char* V_buff = new char[xSize*ySize / 4];

	/* Convert input image to YUV420 image */
	/* TO DO */
	RGBtoYUV420(input, xSize, ySize, Y_buff, U_buff, V_buff);

	/* Perform SIFT transformation  */
	/* TO DO */
	kptList = calculateSIFT(Y_buff, xSize, ySize);

	/* Split the features of left and right images in separate lists */
	/* TO DO */
	splitFeatures(kptList, xSize, kptListLeft, kptListRight);

	/* Match the features from two images */
	list<pair<QPoint, QPoint>> matchedDots;
	matchFeatures(kptListLeft, kptListRight, matchedDots, threshold);

	/* Draw a line for each mached feature pair */
	/* TO DO */
	drawMatchedFeaturesLinesRGB(input, xSize, ySize, matchedDots, 0, 255, 0);

	delete[] Y_buff;
	delete[] U_buff;
	delete[] V_buff;

}

/*******************************************************************************************************************************/
/* Harris algorithm for corner detection 
/*******************************************************************************************************************************/
void HarisCornerDetection(uchar input[], int xSize, int ySize, double threshold)
{
	uchar* sobelVertical = new uchar[xSize*ySize];
	uchar* sobelHorizontal = new uchar[xSize*ySize];
	char* U_buff = new char[xSize*ySize / 4];
	char* V_buff = new char[xSize*ySize / 4];

	/* Convert input image to YUV420 image */
	RGBtoYUV420(input, xSize, ySize, sobelVertical, U_buff, V_buff);
	
	/* Create a copy of Y component, since it is needed to calculate derivative in both axis */
	memcpy(sobelHorizontal, sobelVertical, xSize*ySize);

	/* Filter both images with corresponding Sobel operator */
	/* TO DO: */
	double filterCoeff1[] = { -1.0 / 4,0,1.0 / 4,-2.0 / 4,0,2.0 / 4,-1.0 / 4,0,1.0 / 4 };
	double filterCoeff2[] = { -1.0 / 4,-2.0 / 4,-1.0 / 4,0,0,0,1.0 / 4,2.0 / 4,1.0 / 4 };

	convolve2D(sobelVertical, xSize, ySize, filterCoeff1, 3);
	convolve2D(sobelHorizontal, xSize, ySize, filterCoeff2, 3);

	/* For each pixel, calculate the matrix M, then calculate the R factor and place it in new matrix */
	/* Constant alpha is 0.05. */
	/* TO DO: */
	int delta = 1;

	int newXSize = xSize + 2 * delta;
	int newYSize = ySize + 2 * delta;

	uchar* M = new uchar[4];
	double* R = new double[xSize*ySize];

	double alpha = 0.05;

	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			M[0] = sobelHorizontal[j*xSize + i] * sobelHorizontal[j*xSize + i];
			M[1] = sobelHorizontal[j*xSize + i] * sobelVertical[j*xSize + i];
			M[2] = M[1];
			M[3] = sobelVertical[j*xSize + i] * sobelVertical[j*xSize + i];
			R[j*xSize + i] = M[0] * M[3] - M[1] * M[2] - alpha*pow(M[0] + M[3], 2);
		}
	}

	/* For each entry in R matrix, if the value is greater then threshold, check the 3x3 block arround the pixel
	/* and if it is local maximum, colour the entire 3x3 blok in the input image in blue */

	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			if (R[j*xSize + i] > (threshold*10)) {
				input[j*xSize * 3 + i * 3] = 0;
				input[j*xSize * 3 + i * 3 + 1] = 255;
				input[j*xSize * 3 + i * 3 + 2] = 0;
			}
		}
	}

	delete[] sobelVertical;
	delete[] sobelHorizontal;
	delete[] U_buff;
	delete[] V_buff;

}


