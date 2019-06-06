#pragma once
#include <QImage>

typedef struct
{
	QImage* image;
	int labelNumber;
	std::string label;
} DBImage;

typedef std::list<DBImage> ImgDataBase;

