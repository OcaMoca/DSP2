#include "ImageHistogram.h"
#include "qcustomplot.h"


/*******************************************************************************************************************************/
/* Simple contrast improvement */
/*******************************************************************************************************************************/
void simpleContrastImprovement (const uchar input[], int xSize, int ySize, uchar output[])
{
	/* TODO */
	uchar min = input[0];
	uchar max = input[0];

	for (int j = 0; j < ySize; j++) {
		for (int i = 0; i < xSize; i++) {
			if (input[j*xSize + i] > max) {
				max = input[j*xSize + i];
			}
			if (input[j*xSize + i] < min) {
				min = input[j*xSize + i];
			}
		}
	}

	for (int j = 0; j < ySize; j++) {
		for (int i = 0; i < xSize; i++) {
			output[j*xSize + i] = round((input[j*xSize + i] - min) * 255 / (max - min));
		}
	}

}


/*******************************************************************************************************************************/
/* Calculate image histogram */
/*******************************************************************************************************************************/
void calculateHistogram(const uchar input[], int xSize, int ySize, int histogram[])
{
	/* TODO */
	for (int i = 0; i < 256; i++) {
		histogram[i] = 0;
	}

	for (int j = 0; j < ySize; j++) {
		for (int i = 0; i < xSize; i++) {
			histogram[input[j*xSize + i]]++;
		}
	}

}


/*******************************************************************************************************************************/
/* Equalize image histogram */
/*******************************************************************************************************************************/
void equalizeHistogram(const uchar input[], int xSize, int ySize, uchar output[], uchar L)
{
	/* TODO */
	int histogram[256], cdf[256];
	int sum = 0, cdfmin;

	calculateHistogram(input, xSize, ySize, histogram);
	for (int i = 0; i < 256; i++) {
		sum += histogram[i];
		cdf[i] = sum;
	}

	int k = 0;
	while (cdf[k] == 0) {
		k++;
	}
	cdfmin = cdf[k];

	for (int j = 0; j < ySize; j++) {
		for (int i = 0; i < xSize; i++) {
			output[j*xSize + i] = round((cdf[input[j*xSize + i]] - cdfmin)*L / (xSize*ySize - cdfmin));
		}
	}

}

static uchar clip(double x) {
	if (x < 0) {
		x = 0;
	}
	if (x > 255) {
		x = 255;
	}

	return uchar(x);
}

/*******************************************************************************************************************************/
/* Modify colour saturation */
/*******************************************************************************************************************************/
void modifySaturation(const uchar inputRGB[], const uchar inputY[], int xSize, int ySize, uchar outputRGB[], double S)
{
	/* TODO */
	for (int j = 0; j < ySize; j++) {
		for (int i = 0; i < xSize; i++) {
			outputRGB[j*xSize * 3 + i * 3] = clip(inputRGB[j*xSize * 3 + i * 3] * S + inputY[j*xSize + i] * (1 - S));
			outputRGB[j*xSize * 3 + i * 3 + 1] = clip(inputRGB[j*xSize * 3 + i * 3 + 1] * S + inputY[j*xSize + i] * (1 - S));
			outputRGB[j*xSize * 3 + i * 3 + 2] = clip(inputRGB[j*xSize * 3 + i * 3 + 2] * S + inputY[j*xSize + i] * (1 - S));
		}
	}
}

/*******************************************************************************************************************************/
/* Plot image histogram */
/* Returns QImage containing graph with histogram data. */
/* start and end parameters define first and last value for X axis */
/*******************************************************************************************************************************/
QImage createHistogramPlot(int histogram[], int start, int end)
{
	 QCustomPlot& customPlot = QCustomPlot();
	 
	 int vectorSize = end-start+1;
	 int maxValue = 0;
	 QVector<double> x(vectorSize), y(vectorSize);
	 for (int i=0; i<vectorSize; i++)
	 {
	   x[i] = i + start;  
	   y[i] = histogram[i];
	   maxValue = maxValue>=histogram[i]?maxValue:histogram[i];
	 }
	 
	 // create graph and assign data to it:
	 customPlot.addGraph();
	 customPlot.graph(0)->setData(x, y);
	 // give the axes some labels:
	 customPlot.xAxis->setLabel("x");
	 customPlot.yAxis->setLabel("y");
	 // set axes ranges, so we see all data:
	 customPlot.xAxis->setRange(start, end);
	 customPlot.yAxis->setRange(0, maxValue);
	 return customPlot.toPixmap().toImage();
}