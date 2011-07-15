#include <QDebug>
#include "surface_imgs.h"

SurfaceImgs::SurfaceImgs(BoundingBox surfBox)
	: BoundingBox(surfBox)
{
	//constructor
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

int SurfaceImgs::inscriptionCount() const
{
	return inscrImgsList.count();
}

InscriptionImgs* SurfaceImgs::ptrInscrAt(int index) 
{
	if(index < 0 || index >= inscrImgsList.count())
	{
		qDebug() << "index oor in SurfaceImgs::inscrImgAt";
		return NULL;
	}
	return &inscrImgsList[index];
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
