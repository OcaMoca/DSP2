#include "PredictSVM.h"
#include <iostream>
#include "BoW.h"
using namespace std;

void predictSVM(const ImgDataBase& dataBase)
{
	/* Load SVM params */
	BagOfWordsModel bowModel;
	cout << "Load Bag of Words model ...";
	loadBoWModel(bowModel, "model.bow");
	if (bowModel.model.size() == 0)
	{
		cout << "ERROR!";
		exit(1);
	}
	cout << "DONE" << endl;

	cout << "Load Bag of Words model ...";
	svm_model* svmModel = svm_load_model("vezba12.model");
	if(svmModel == NULL)
	{
		cout << "ERROR!";
		exit(1);
	}
	cout << "DONE" << endl;

	cout << "Do prediction...";
	int totalHits = 0;
	for (const DBImage& img : dataBase)
	{
		int predictedLabel = predictSingleImage(img.image->bits(), img.image->width(), img.image->height(), bowModel, svmModel);

		if(predictedLabel == img.labelNumber)
			totalHits++;
	}

	cout << "DONE" << endl;

	cout << "Prediction accuracy: " << totalHits*100.0 / dataBase.size() << "%";
	
	svm_free_and_destroy_model(&svmModel);
}


int predictSingleImage(uchar input[], int xSize, int ySize, BagOfWordsModel& bowModel, svm_model* svmModel)
{
	vector<double> feature;
	/*************************************************************************/
	/* TO DO Calculate BoW descriptor for image and put it in feature vector */

	feature = getBoWDescripton(input, xSize, ySize, bowModel);

	/*************************************************************************/

	svm_node* featureVector = new svm_node[bowModel.featureVectorSize + 1];
	for (int j = 0; j < bowModel.featureVectorSize; j++)
	{
		featureVector[j] = { j, feature[j] };
	}
	featureVector[bowModel.featureVectorSize] = { -1, 0.0 };
	/*************************************************************************/
	/* TO DO: Invoke SVM train */
	double res;

	res = svm_predict(svmModel, featureVector);

	/*************************************************************************/
	
	delete[] featureVector;

	return round(res);
}