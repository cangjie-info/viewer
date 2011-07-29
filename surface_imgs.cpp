#include <QDebug>
#include "surface_imgs.h"

SurfaceImgs::SurfaceImgs()
{ 
	//constructor 
}

SurfaceImgs::SurfaceImgs(BoundingBox surfBox)
	: BoundingBox(surfBox)
{
	//constructor
}

void SurfaceImgs::setImageFile(QString path)
{
	imageFile = path;
}

QString SurfaceImgs::getImageFile()
{
	return imageFile;
}

void SurfaceImgs::insertInscr(BoundingBox inscrBox, int index)
{
	if(index < 0 || index > inscrImgsList.count())
	{
		qDebug() << "index oor in SurfaceImgs::insertInscr";
		return;
	}
	inscrImgsList.insert(index, inscrBox);
}

void SurfaceImgs::deleteInscr(int index)
{
	if(index < 0 || index >= inscrImgsList.count())
	{
		qDebug() << "index oor in SurfaceImgs::deleteInscr";
		return;
	}
	inscrImgsList.removeAt(index);
}

void SurfaceImgs::deleteAllInscriptions()
{
	inscrImgsList.clear();
}

int SurfaceImgs::inscriptionCount() const
{
	return inscrImgsList.count();
}

InscriptionImgs SurfaceImgs::inscrAt(int index) const
{
	if(index < 0 || index >= inscrImgsList.count())
	{
		qDebug() << "index oor in SurfaceInscImgs::inscrAt";
		qDebug() << "index = " << index;
		return InscriptionImgs();
	}
	return inscrImgsList.at(index);
}

InscriptionImgs* SurfaceImgs::ptrInscrAt(int index)
{
	if(index < 0 || index >= inscrImgsList.count())
	{
                qDebug() << "index oor in SurfaceImgs::ptrInscrAt";
		return NULL;
	}
	return &inscrImgsList[index];
}

void SurfaceImgs::swap(int i, int j)
{
    //oor checks should be done by caller!!
        inscrImgsList.swap(i, j);
}

void SurfaceImgs::report() const
{
	qDebug() << "**** REPORT FOR SurfaceImgs ****";
	qDebug() << "** Surface Bounding Box **";
	BoundingBox::report();
	qDebug() << "** Inscriptions on surface **";
	qDebug() << "count =" << inscriptionCount();
	int count = inscrImgsList.count();
	for (int i = 0; i < count; i++)
	{
		inscrImgsList[i].report();
	}
	qDebug() << "**** END REPORT FOR SurfaceImgs ****";
}
