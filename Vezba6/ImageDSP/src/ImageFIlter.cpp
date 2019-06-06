#include "ImageFilter.h"


void convolve2D (uchar Y_buff[], int xSize, int ySize, double filterCoeff[], int N)
{
	//TO DO
	int delta = (N - 1) / 2;
	
	int newXSize = xSize + 2 * delta;
	int newYSize = ySize + 2 * delta;

	uchar* newY_buff = new uchar[newXSize*newYSize];

	extendBorders(Y_buff, xSize, ySize, newY_buff, delta);

	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			double accum = 0;
			for (int m = 0; m < N; m++) {
				for (int n = 0; n < N; n++) {
					accum += newY_buff[(j + n)*newXSize + i + m] * filterCoeff[(N - n)*N - m - 1];
				}
			}

			if (accum > 255.0)
				Y_buff[j*xSize + i] = 255;
			else if (accum < 0.0)
				Y_buff[j*xSize + i] = 0;
			else
				Y_buff[j*xSize + i] = floor(accum + 0.5);
		}
	}
}

void extendBorders(uchar input[], int xSize, int ySize, uchar output[], int delta)
{
	//TO DO
	int newXSize = xSize + 2 * delta;
	int newYSize = ySize + 2 * delta; 

	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			output[(j + delta)*newXSize + i + delta] = input[j*xSize + i];
		}
	}

	for (int i = 0; i < delta; i++) {
		memcpy(&output[i*newXSize + delta], &input[0], xSize);
		memcpy(&output[(ySize + delta + i)*newXSize + delta], &input[(ySize - 1)*xSize], xSize);
	}

	for (int i = 0; i < newYSize; i++) {
		memset(&output[i*newXSize], output[i*newXSize + delta], delta);
		memset(&output[i*newXSize + delta + xSize], output[i*newXSize + xSize + delta - 1], delta);
	}
}
	
void performNFFilter (uchar input[], int xSize, int ySize)
{
	double filterCoeff[] = { 1.0 / 9,1.0 / 9,1.0 / 9,1.0 / 9 ,1.0 / 9 ,1.0 / 9 ,1.0 / 9 ,1.0 / 9 ,1.0 / 9 };
	convolve2D(input, xSize, ySize, filterCoeff, 3);
	//TO DO
}

void performVFFilter (uchar input[], int xSize, int ySize)
{
	double filterCoeff[] = { 0,-1,0,-1,4,-1,0,-1,0 };
	convolve2D(input, xSize, ySize, filterCoeff, 3);
	//TO DO
}

void performSuccessiveNFFilter (uchar input[], int xSize, int ySize, int stages)
{
	//TO DO
	for (int i = 0; i < stages; i++) {
		performNFFilter(input, xSize, ySize);
	}
}

void performSobelEdgeDetection(uchar input[], int xSize, int ySize, uchar threshold)
{
	//TO DO
	uchar* copyInput = new uchar[xSize*ySize];

	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			copyInput[j*xSize + i] = input[j*xSize + i];
		}
	}

	double filterCoeff1[] = { -1.0 / 4,0,1.0 / 4,-2.0 / 4,0,2.0 / 4,-1.0 / 4,0,1.0 / 4 };
	double filterCoeff2[] = { -1.0 / 4,-2.0 / 4,-1.0 / 4,0,0,0,1.0 / 4,2.0 / 4,1.0 / 4 };

	convolve2D(input, xSize, ySize, filterCoeff1, 3);
	convolve2D(copyInput, xSize, ySize, filterCoeff2, 3);

	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			input[j*xSize + i] = (sqrtf(input[j*xSize+i]* input[j*xSize + i]+ copyInput[j*xSize + i]* copyInput[j*xSize + i])>=threshold) ? 255 : 0;
		}
	}

}

void performNFplusSobelEdgeDetection(uchar input[], int xSize, int ySize, int stages, uchar threshold)
{
	//TO DO
	performSuccessiveNFFilter(input, xSize, ySize, stages);
	performSobelEdgeDetection(input, xSize, ySize, threshold);
}
