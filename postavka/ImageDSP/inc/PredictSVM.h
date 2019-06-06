#pragma once
#include <QImage>
#include "svm.h"
#include "DataBase.h"
#include "BoW.h"


void predictSVM(const ImgDataBase& dataBase);

int predictSingleImage(uchar input[], int xSize, int ySize, BagOfWordsModel& bowModel, svm_model* svmModel);
