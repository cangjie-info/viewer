#include <QDebug>
#include "inscription_imgs.h"


InscriptionImgs::InscriptionImgs()
{
	//constructor
}

InscriptionImgs::InscriptionImgs(BoundingBox inscrBox)
	: BoundingBox(inscrBox)
{
	//constructor
}

void InscriptionImgs::report() const
{
	qDebug() << "*** REPORT FOR InscriptionImgs ***";
	BoundingBox::report();
	BoxList::report();
	qDebug() << "*** END REPORT FOR Inscription ***";
}

