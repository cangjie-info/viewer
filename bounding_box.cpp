#include <QDebug>
#include "bounding_box.h"

BoundingBox::BoundingBox(QPoint point1, QPoint point2, int angle, bool null) 
	: QRect(point1, point2), rotation(angle), isNull(null)
{
	//constructor
}

int BoundingBox::getRotation() const
{
	return rotation;
}

void BoundingBox::setNull(bool null)
{
	isNull = null;
}

void BoundingBox::toggleNull()
{
	isNull = !isNull;
}

bool BoundingBox::boxIsNull() const
{
	return isNull;
}

void BoundingBox::report() const
{
	qDebug() << "REPORT FOR BoundingBox";
	qDebug() << "x1 = " << x() << ", y1 = " << y();
	qDebug() << "x2 = " << x()+width()-1 << ", y2 = " << y()+height()-1;
	qDebug() << "rotation = " << getRotation();
	qDebug() << "box is" << (isNull ? "" : "not") << "null.";
}
