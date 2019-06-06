#include "ImageInterpolation.h"
#include <math.h>

int clip(int x, int maks) {
	if (x < 0) {
		return 0;
	}
	if (x > maks) {
		return maks;
	}

	return x;
}

void sampleAndHold(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
	/* TO DO */
	double Fi = (double)newXSize / xSize;
	double Fj = (double)newYSize / ySize;

	for (int i = 0; i < newXSize; i++) {
		for (int j = 0; j < newYSize; j++) {
			int i1 = clip((i - Fi / 2) / Fi + 1,xSize);
			int j1 = clip((j - Fj / 2) / Fj + 1,ySize);

			for (int k = 0; k < 3; k++) {
				output[j*newXSize*3 + i*3+k] = input[j1*xSize*3 + i1*3+k];
			}

		}
	}

}

void bilinearInterpolate(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
	/* TO DO */
	double Fi = (double)newXSize / xSize;
	double Fj = (double)newYSize / ySize;

	for (int i = 0; i < newXSize; i++) {
		for (int j = 0; j < newYSize; j++) {
			
			double a = (double)i / Fi - floor((double)i / Fi);
			double b = (double)j / Fj - floor((double)j / Fj);

			int i1 = floor(i / Fi);
			int j1 = floor(j / Fj);

			for (int k = 0; k < 3; k++) {
				uchar pixel00 = input[j1*xSize * 3 + i1 * 3 + k];
				uchar pixel01 = input[j1*xSize * 3 + (i1+1) * 3 + k];
				uchar pixel10 = input[(j1+1)*xSize * 3 + i1 * 3 + k];
				uchar pixel11 = input[(j1+1)*xSize * 3 + (i1+1) * 3 + k];

				output[j*newXSize * 3 + i * 3 + k] = (1 - a)*(1 - b)*pixel00 + (1 - a)*b*pixel10 + a*(1 - b)*pixel01 + a*b*pixel11;
			}

		}
	}

}

void imageRotate(const uchar input[], int xSize, int ySize, uchar output[], int m, int n, double angle)
{
	/* TO DO */

	double angle1 = angle / 180 * 3.14159;

	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			int i1 = i*cos(angle1) - j*sin(angle1) - m*cos(angle1) + n*sin(angle1) + m;
			int j1 = j*cos(angle1) + i*sin(angle1) - m*sin(angle1) - n*cos(angle1) + n;

			for (int k = 0; k < 3; k++) {
				output[j*xSize * 3 + i * 3 + k] = input[j1*xSize * 3 + i1 * 3 + k];
			}

		}
	}
}

void imageRotateBilinear(const uchar input[], int xSize, int ySize, uchar output[], int m, int n, double angle)
{
	/* TO DO */
}


