#include <QtCore/QCoreApplication>
#include "TrainSVM.h"
#include "PredictSVM.h"
#include <string>
#include <iostream>
#include <fstream>
#include <DataBase.h>

using namespace std;

/***********************************************************/
/*                     PARAMS                              */
/***********************************************************/
// Image database path and name
static const string databasePath = "img_database/";
static const string dataBaseName = "carsDataBase.txt";

// Number of database subsets K for K-fold cross validaton
static const int K = 5;

// Create new BoW in training phase
static bool createNewBoWModel = false;

/***********************************************************/

typedef enum
{
	SVM_TRAIN,
	SVM_PREDICT,
	SVM_CROSSVALIDATE
} ProgramOpt_t;

ImgDataBase imgDataBase;

int main(int argc, char *argv[])
{
	ProgramOpt_t opt = SVM_TRAIN;
	if (argc == 2)
	{
		opt = ProgramOpt_t(stoi(argv[1]));
	}

	string line;
	ifstream dbFile(databasePath + dataBaseName);
	if (dbFile.is_open())
	{
		cout << "Database opened succesefully!" << endl;
		cout << "Reading images... ";
		int linenum = 1;
		while (getline(dbFile, line, '|'))
		{
			// Load image name and label 
			DBImage img;
			img.label = line;
			img.labelNumber = stoi(line);
			getline(dbFile, line);
	
			// Open image and align dimension to be dividable by 8
			QImage input = QImage((databasePath + line).c_str());
			if (input.width() % 8 != 0)
				input = input.scaledToWidth(input.width() + (8 - input.width() % 8));
			
			img.image = new QImage(input.convertToFormat(QImage::Format_RGB888));
			
			if (img.image == NULL)
			{
				cout << "Error reading database file at line" << linenum << ": "<< line << endl;
				break;
			}
			imgDataBase.push_back(img);
			linenum++;
		}
		cout << "DONE! " << endl;
		dbFile.close();
	}
	else
	{
		cout << "Unable to open database file";
		return -1;
	}
	

	switch (opt)
	{
		case SVM_TRAIN:
			trainSVM(imgDataBase, createNewBoWModel);
			break;
		case SVM_CROSSVALIDATE:
			crossValidateKFold(imgDataBase, K, createNewBoWModel);
			break;
		case SVM_PREDICT:
			predictSVM(imgDataBase);
			break;
	}

	return 0;
}
