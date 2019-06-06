
#include "TrainSVM.h"
#include "FeatureDetection.h"
#include "ColorSpaces.h"
#include "kMeans.h"
#include <QDebug>
#include "DataBase.h"
#include "BoW.h"
#include "svm.h"
#include <iostream>

using namespace std;

static void setDefaultParams(svm_parameter& param);
const int histogramSize = 500;

void trainSVM(const ImgDataBase& dataBase, bool createNewBoWModel)
{
	svm_problem svmProblemSet;
	svm_parameter svmParam;

	/* Create problem set */
	svmProblemSet.l = dataBase.size();
	svmProblemSet.y = new double[svmProblemSet.l];
	svmProblemSet.x = new svm_node* [svmProblemSet.l];

	/* Load SVM params */
	setDefaultParams(svmParam);
	BagOfWordsModel bowModel;
	if (createNewBoWModel)
	{
		cout << "Making Bag of Words model ...";
		bowModel = createBoWModel(dataBase, histogramSize);
		cout << "DONE" << endl;
	}
	else
	{
		cout << "Load Bag of Words model ...";
		loadBoWModel(bowModel, "model.bow");
		cout << "DONE" << endl;
	}

	cout << "Calculating features for SVM...";
	int i = 0;
	for (const DBImage& img : dataBase)
	{
		vector<double> feature;
		/*************************************************************************/
		/* TO DO Calculate BoW descriptor for image and put it in feature vector */
		
		feature = getBoWDescripton(img.image->bits(), img.image->width(), img.image->height(), bowModel);

		/*************************************************************************/

		svmProblemSet.y[i] = img.labelNumber;
		svmProblemSet.x[i] = new svm_node[bowModel.featureVectorSize+1];
		
		for (int j = 0; j < bowModel.featureVectorSize; j++)
		{
			svmProblemSet.x[i][j] = { j, feature[j] };
		}

		svmProblemSet.x[i][bowModel.featureVectorSize] = { -1, 0.0 };
		i++;
	}

	cout << "DONE" << endl;

	cout << "Training SVM model ...";
	
	svm_model* model = NULL;
	
	/*************************************************************************/
	/* TO DO: Invoke SVM train */

	model = svm_train(&svmProblemSet, &svmParam);

	/*************************************************************************/
	cout << "DONE" << endl;

	if(svm_save_model("vezba12.model", model))
	{
		cout <<"ERROR: can't save model to file";
	}
	
	svm_free_and_destroy_model(&model);
}

void crossValidateKFold(const ImgDataBase& dataBase, int K, bool createNewBoWModel)
{
	svm_problem svmProblemSet;
	svm_parameter svmParam;

	/* Create problem set */
	svmProblemSet.l = dataBase.size();
	svmProblemSet.y = new double[svmProblemSet.l];
	svmProblemSet.x = new svm_node*[svmProblemSet.l];

	/* Load SVM params */
	setDefaultParams(svmParam);
	BagOfWordsModel bowModel;
	if (createNewBoWModel)
	{
		cout << "Making Bag of Words model ...";
		bowModel = createBoWModel(dataBase, histogramSize);
		cout << "DONE" << endl;
	}
	else
	{
		cout << "Load Bag of Words model ...";
		loadBoWModel(bowModel, "model.bow");
		cout << "DONE" << endl;
	}

	cout << "Calculating features for SVM...";	
	int i = 0;
	for (const DBImage& img : dataBase)
	{
		vector<double> feature = getBoWDescripton(img.image->bits(), img.image->width(), img.image->height(), bowModel);

		svmProblemSet.y[i] = img.labelNumber;
		svmProblemSet.x[i] = new svm_node[bowModel.featureVectorSize + 1];
		for (int j = 0; j < bowModel.featureVectorSize; j++)
		{
			svmProblemSet.x[i][j] = { j, feature[j] };
		}
		svmProblemSet.x[i][bowModel.featureVectorSize] = { -1, 0.0 };
		i++;
	}
	cout << "DONE" << endl;

	cout << "Perform cross-validation, k = " << K << "...";

	int total_correct = 0;
	double *target = new double[svmProblemSet.l];

	svm_cross_validation(&svmProblemSet, &svmParam, K, target);
	
	cout << "DONE" << endl;

	for (int i = 0; i<svmProblemSet.l; i++)
		if (target[i] == svmProblemSet.y[i])
			++total_correct;
	cout << "Cross Validation Accuracy = " << 100.0*total_correct / svmProblemSet.l << endl;
	
	free(target);
}


static void setDefaultParams(svm_parameter& param)
{
	// default values
	param.svm_type = ONE_CLASS;
	param.kernel_type = RBF;
	param.degree = 3;
	param.gamma = 1.0 / histogramSize;
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	param.C = 0.5;
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;
}


