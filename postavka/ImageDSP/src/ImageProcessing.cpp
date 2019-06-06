
#include "ImageProcessing.h"
#include "PredictSVM.h"
#include "BoW.h"
#include "svm.h"
#include <QDebug>
#include <QPainter>

static bool modelLoaded = false;
static svm_model* svmModel;
static BagOfWordsModel  bowModel;


void imageProcessingFun(const QString& progName, QImage* const outImgs, const QImage* const inImgs, const QVector<double>& params)
{
	if (!modelLoaded)
	{
		loadBoWModel(bowModel, "model.bow");
		if (bowModel.model.size() == 0)
		{
			return;
		}

		svmModel = svm_load_model("vezba12.model");
		if (svmModel == NULL)
		{
			return;
		}
		modelLoaded = true;
	}

	int X_SIZE = inImgs->width();
	int Y_SIZE = inImgs->height();

	*outImgs = QImage(*inImgs);

	if (progName == "SVM Classification")
	{
		int predicted_label;
		
		//Invoke predict function for Single image 
		predicted_label = predictSingleImage(outImgs->bits(), X_SIZE, Y_SIZE, bowModel, svmModel);

		QPainter p(outImgs);
		p.setPen(QPen(Qt::red));
		p.setFont(QFont("Times", 12, QFont::Bold));
		if(predicted_label == 1)
			p.drawText(outImgs->rect(), Qt::AlignCenter, "Car!");
		else
			p.drawText(outImgs->rect(), Qt::AlignCenter, "Not a car!");
	}
}

