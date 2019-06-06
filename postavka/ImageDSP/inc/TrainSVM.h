#ifndef _TRAIN_SVM_H_
#define _TRAIN_SVM_H_

#include <QString>
#include <QVector>
#include <QImage>
#include "DataBase.h"

void trainSVM(const ImgDataBase& dataBase, bool createNewBoWModel);

void crossValidateKFold(const ImgDataBase& dataBase, int K, bool createNewBoWModel);

#endif // _TRAIN_SVM_H_
