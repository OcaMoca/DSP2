#include "ColorSpaces.h"
#include "kMeans.h"
#include <SIFTLib.h>
#include <list>

using namespace std;
#define CONVERGENCE_THRESHOLD 1e-6

// Predefined list of colors for coloring separate segments
static const int NUM_COLORS = 16;
static const uchar RGBColors[NUM_COLORS][3] = { {255, 0, 0}, {0, 255, 0},{0, 0, 255}, {0, 255, 255}, {255, 255, 0}, {255, 0, 255}, {0, 0, 0}, {255, 255, 255}, { 128, 0, 0 }, { 0, 128, 0 }, { 0, 0, 128 }, { 0, 128, 128 }, { 128, 128, 0 }, { 128, 0, 128 }, { 128, 128, 128 }, {64, 128, 64}};

// Function for kMeans calculation
static vector<vector<int>> kMeans(vector<KMeansPoint> points, int nFeatures, int K);

// Color Based kMeans segmentation
void ColorBasedKMeans(uchar input[], int xSize, int ySize, int K) 
{
	// Calculate feature vector for each point
	vector<KMeansPoint> points;
	for (int i = 0; i < xSize; i++)
	{
		for (int j = 0; j < ySize; j++)
		{
			KMeansPoint newPoint = { i, j};
			for (int k = 0; k < 3; k++)
			{
				newPoint.feature.push_back(input[3 * j * xSize + 3 * i + k]);
			}
			points.push_back(newPoint);
		}
	}
	
	// Invoke kMeans function
	vector<vector<int>> clusters;
	clusters = kMeans(points, 3, K);

	// Paint each segment using predefined colors
	for (int k = 0; k < K; k++)
	{
		for (int index : clusters[k])
		{
			int i = points[index].x;
			int j = points[index].y;
			input[3 * j * xSize + 3 * i] = RGBColors[k][0];
			input[3 * j * xSize + 3 * i + 1] = RGBColors[k][1];
			input[3 * j * xSize + 3 * i + 2] = RGBColors[k][2];
		}
	}
}

void IntensityPlusPositionBasedKMeans(uchar input[], int xSize, int ySize, int K) 
{
	//TO DO
	uchar *Y_buff = new uchar[xSize*ySize];
	char *U_buff = new char[xSize*ySize / 4];
	char *V_buff = new char[xSize*ySize / 4];

	RGBtoYUV420(input, xSize, ySize, Y_buff, U_buff, V_buff);

	vector<KMeansPoint> points;

	vector<double> features;

	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			features.clear();
			features.push_back(Y_buff[j*xSize + i]);
			features.push_back(i * 255 / xSize);
			features.push_back(j * 255 / ySize);
			KMeansPoint newPoint = { i, j };
			newPoint.feature = features;
			points.push_back(newPoint);
		}
	}

	vector<vector<int>> clusters;
	clusters = kMeans(points, 3, K);

	// Paint each segment using predefined colors
	for (int k = 0; k < K; k++)
	{
		for (int index : clusters[k])
		{
			int i = points[index].x;
			int j = points[index].y;
			input[3 * j * xSize + 3 * i] = RGBColors[k][0];
			input[3 * j * xSize + 3 * i + 1] = RGBColors[k][1];
			input[3 * j * xSize + 3 * i + 2] = RGBColors[k][2];
		}
	}
	
}

void ColorPlusPositionBasedKMeans(uchar input[], int xSize, int ySize, int K) 
{
	// TO DO
	vector<KMeansPoint> points;

	vector<double> features;

	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			features.clear();
			features.push_back(input[j*xSize*3 + i*3]);
			features.push_back(input[j*xSize * 3 + i * 3 + 1]);
			features.push_back(input[j*xSize * 3 + i * 3 + 2]);
			features.push_back(i * 255 / xSize);
			features.push_back(j * 255 / ySize);
			KMeansPoint newPoint = { i, j };
			newPoint.feature = features;
			points.push_back(newPoint);
		}
	}

	vector<vector<int>> clusters;
	clusters = kMeans(points, 3, K);

	// Paint each segment using predefined colors
	for (int k = 0; k < K; k++)
	{
		for (int index : clusters[k])
		{
			int i = points[index].x;
			int j = points[index].y;
			input[3 * j * xSize + 3 * i] = RGBColors[k][0];
			input[3 * j * xSize + 3 * i + 1] = RGBColors[k][1];
			input[3 * j * xSize + 3 * i + 2] = RGBColors[k][2];
		}
	}
}

void SIFTBasedKMeans(uchar input[], int xSize, int ySize, int K) 
{
	// TO DO
	uchar *Y_buff = new uchar[xSize*ySize];
	char *U_buff = new char[xSize*ySize / 4];
	char *V_buff = new char[xSize*ySize / 4];

	RGBtoYUV420(input, xSize, ySize, Y_buff, U_buff, V_buff);

	SiftKeypointList keyList = calculateSIFT(Y_buff, xSize, ySize);


}

// Calcilate Euclid distance between two points in n-dimensional space
static double getDistance(const vector<double>& a, const vector<double>& b) {
	double temp = 0.0;
	auto it1 = a.begin();
	auto it2 = b.begin();
	for (; (it1 != a.end()) && (it2 != b.end()); it1++, it2++)
	{
		temp += (*it1 - *it2) * (*it1 - *it2);
	}
	return sqrt(temp);
}

// Find nearest cluster for the given point
static double whichIsNearest(const vector<KMeansPoint>& centroids, KMeansPoint pt) {
	double minDistance = 0;
	int minLabel = 0;
	for (int i = 0; i<centroids.size(); i++) {
		double tempDistance = getDistance(centroids[i].feature, pt.feature);
		if (i == 0 || tempDistance < minDistance) {
			minDistance = tempDistance;
			minLabel = i;
		}
	}
	return minLabel;
}

// Calculate kMeans
static vector<vector<int>> kMeans(vector<KMeansPoint> points, int nFeatures, int K) {
	// Total number of points
	int nSamples = points.size();
	
	// Randomize initial centroids
	vector<KMeansPoint> centroids;
	for (int k = 0; k < K; k++) 
	{
		int rand_int = rand() % nSamples;
		centroids.push_back(points[rand_int]);
	}
	
	// Create empty vector for each cluster
	vector<vector<int> > cluster;
	for (int k = 0; k < K; k++) {
		vector<int> vectemp;
		cluster.push_back(vectemp);
	}
	
	// Iteration counter
	int counter = 0;
	
	// Iteratively find better centroids
	while (1) {

		// Clear each cluster
		for (int k = 0; k < K; k++)
		{
			cluster[k].clear();
		}
		
		// Set convergence flag to TRUE
		bool converge = true;
		
		// For every sample, find which cluster it belongs to,
		// By comparing the distance between it and each clusters' centroid.
		// TO DO
		for (int k = 0; k < nSamples; k++) {
			cluster[whichIsNearest(centroids, points[k])].push_back(k);
		}

		// For each cluster, re-calculate its centroid.
		for (int k = 0; k < K; k++) {
			int clusterSize = cluster[k].size();
			
			vector<double> vectemp = vector<double>(nFeatures, 0);
			for (int i = 0; i < clusterSize; i++) {
				for(int j = 0; j < nFeatures; j++)
					vectemp[j] = vectemp[j] + points[cluster[k][i]].feature[j] / (double)clusterSize;
			}
			// If centroid position changed set convergence flag to false
			// TO DO
			if (getDistance(vectemp, centroids[k].feature) > CONVERGENCE_THRESHOLD) {
				converge = false;
			}

			centroids[k].feature = vectemp;

		}

		// If convergence achieved break the loop
		if (converge) 
			break;
		++counter;
	}
	
	// Return cluster vectors
	return cluster;
}


