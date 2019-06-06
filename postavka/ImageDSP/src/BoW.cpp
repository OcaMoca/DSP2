#include "BoW.h"
#include "kMeans.h"
#include "FeatureDetection.h"
#include <QImage>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

BagOfWordsModel createBoWModel(const ImgDataBase& dataBase, int K)
{
	vector<vector<double>> uncategorizedFeatures;
	for (DBImage inImg : dataBase)
	{
		cout << ".";
		SiftKeypointList features = GetSIFTFeatures(inImg.image->bits(), inImg.image->width(), inImg.image->height());
		for (SiftKeypoint& kp : features)
		{
			vector<double> descriptor(kp.descriptors, kp.descriptors + DEGREE_OF_DESCRIPTORS);
			uncategorizedFeatures.push_back(descriptor);
		}
	}
	cout << endl << "Total number of features: " << uncategorizedFeatures.size() << endl;
	
	cout << "Invoke kMeans...";
	vector<vector<double>> BoWData = kMeansCentroids(uncategorizedFeatures, DEGREE_OF_DESCRIPTORS, K);
	cout << "DONE" << endl;

	BagOfWordsModel model;
	model.model = BoWData;
	model.numOfClusters = K;
	model.featureVectorSize = DEGREE_OF_DESCRIPTORS;
	
	dumpBoWModel(model, "model.bow");

	return model;
}

int findBoWIndex(const BagOfWordsModel& model, vector<double> feature)
{
	return whichIsNearest(model.model, feature);
}

vector<double> getBoWDescripton(uchar input[], int xSize, int ySize, const BagOfWordsModel& model)
{
	vector<double> histogram(model.numOfClusters, 0);
	
	/* TO DO: Calculate SIFT features for input image */
	SiftKeypointList features = GetSIFTFeatures(input, xSize, ySize);
	
	/* TO DO: Create histogram of features based on model */
	for (SiftKeypoint &keys : features) {
		vector<double> descriptor(keys.descriptors, keys.descriptors + DEGREE_OF_DESCRIPTORS);
		histogram[whichIsNearest(model.model, descriptor)]++;
	}

	/* Scale histogram to [-1.0, 1.0]*/
	double sum = 0;
	for (double i : histogram) {
		sum += i;
	}

	for (int i = 0; i < model.numOfClusters; i++) {
		histogram[i] /= sum;
	}

	return histogram;
}


/*******************************************************************/
/*              Load and store BoW Model from/to file              */
/*******************************************************************/
void dumpBoWModel(const BagOfWordsModel& bowModel, string filename)
{
	ofstream outFile(filename.c_str(), std::ios::out);
	outFile << bowModel.numOfClusters << endl;
	outFile << bowModel.featureVectorSize << endl;

	outFile.precision(17);

	for (const vector<double>& feature : bowModel.model)
	{
		for (const double& value : feature)
		{
			outFile << value;
			outFile << " ";
		}
		outFile << endl;
	}
}

void loadBoWModel(BagOfWordsModel& bowModel, string filename)
{
	std::ifstream inFile(filename.c_str(), std::ios::in);

	inFile >> bowModel.numOfClusters;
	inFile >> bowModel.featureVectorSize;

	bowModel.model.clear();

	for (int i = 0; i < bowModel.numOfClusters; i++)
	{
		vector<double> feature(bowModel.featureVectorSize);
		for (int j = 0; j < bowModel.featureVectorSize; j++)
		{
			inFile >> feature[j];
		}
		bowModel.model.push_back(feature);
	}
}
/*******************************************************************/