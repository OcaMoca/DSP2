#pragma once
#include <vector>
#include <Qimage>
#include "DataBase.h"
#include <string>

typedef struct
{
	std::vector<std::vector<double>> model;
	int numOfClusters;
	int featureVectorSize;
} BagOfWordsModel;


BagOfWordsModel createBoWModel(const ImgDataBase& dataBase, int K);

std::vector<double> getBoWDescripton(uchar input[], int xSize, int ySize, const BagOfWordsModel& model);

void dumpBoWModel(const BagOfWordsModel& model, std::string filename);

void loadBoWModel(BagOfWordsModel& model, std::string filename);
