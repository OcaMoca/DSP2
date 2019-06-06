#include "NoiseReduction.h"
#include "ImageFilter.h"

#include <vector>
#include <algorithm>


void performMovingAverage (uchar input[], int xSize, int ySize, int N)
{
	//TO DO
	double *filterCoeff = new double[N*N];
	for (int i = 0; i < N*N; i++) {
		filterCoeff[i] = 1.0 / (N*N);
	}

	convolve2D(input, xSize, ySize, filterCoeff, N);

}

void calculateGaussKernel(double kernel[], int N, double sigma)
{
	double C = 0;
	for(int n = 0; n < N; n++)
    {
        for(int k = 0; k < N; k++)
        {
            kernel[n*N+k] = exp( -((n - N/2)*(n - N/2) + (k - N/2)*(k - N/2)) / (2 * sigma * sigma));
			C += kernel[n*N+k];
		}
	}

	C = 1.0 / C;

	for(int n = 0; n < N; n++)
    {
        for(int k = 0; k < N; k++)
        {
            kernel[n*N+k] *= C;
		}
	}
}

void performGaussFilter (uchar input[], int xSize, int ySize, int N, double sigma)
{
	//TO DO
	double *kernelCoeff = new double[N*N];
	for (int i = 0; i < N*N; i++) {
		kernelCoeff[i] = 0;
	}

	calculateGaussKernel(kernelCoeff, N, sigma);

	convolve2D(input, xSize, ySize, kernelCoeff, N);
}

void performMedianFilter (uchar input[], int xSize, int ySize, int N)
{
	//TO DO

	uchar* extended = new uchar[(xSize + N - 1) * (ySize + N - 1)];

	extendBorders(input, xSize, ySize, extended, N / 2);

	for (int h = 0; h < ySize; h++)
	{
		for (int v = 0; v < xSize; v++)
		{
			double *buffer = new double[N * N];

			for (int k = 0; k <N; k++)
			{
				for (int n = 0; n <N; n++)
				{
					buffer[k*N + n] = extended[(h+k)*(xSize+N-1)+v+n];
				}
			}

			bubble_sort(buffer, N * N);

			input[h*xSize + v] = buffer[(N*N-1)/2];

			delete buffer;
		}
	}

	delete extended;
}

void bubble_sort(double buffer[], int size) {
	int i, j;
	bool swapped;
	for (int i = 0; i < size - 1; i++) {
		swapped = false;
		for (int j = 0; j < size - i - 1; j++) {
			if (buffer[j] > buffer[j + 1]) {
				int tmp = buffer[j];
				buffer[j] = buffer[j + 1];
				buffer[j + 1] = tmp;
				swapped = true;
			}
		}
		if (swapped == false) break;
	}
}