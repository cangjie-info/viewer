#include <QDebug>
#include "box_list.h"

BoxList::BoxList()
{
	//constructor
}

void BoxList::insertBox(BoundingBox newBox, int index)
{
	if(index < 0 || index > boxList.count())
	{
		qDebug() << "index out of range in BoxList::insertBox";
		return;
	}
	boxList.insert(index, newBox);
}

void BoxList::deleteBox(int index)
{
	if(index < 0 || index >= boxList.count()) //this also guarantees that the boxList is not empty
	{
		qDebug() << "list empty or index out of range in BoxList::deleteBox";
		return;
	}
	boxList.removeAt(index);
}

void BoxList::toggleNull(int index)
{
	if(index < 0 || index >= boxList.count())
	{
		qDebug() << "list empty or index out of range in BoxList::toggleNull";
		return;
	}
	boxList[index].toggleNull();
}

BoundingBox BoxList::boxAt(int index) const
{
	if(index < 0 || index >= boxList.count())
	{
		qDebug() << "list empty or index oor in BoxList::boxAt";
		return BoundingBox(QPoint(0,0), QPoint(0,0), 0, true);
	}
	return boxList.at(index); //returns value, not modifiable reference
}

bool BoxList::boxNullAt(int index) const
{
	return boxList[index].boxIsNull();
}

int BoxList::boxCount() const
{
	return boxList.count();
}

void BoxList::report() const
{
	qDebug() << "*** REPORT FOR BoxList ***";
	qDebug() << "BoxList contains" << boxList.count() << "boxes: ";
	for(int i = 0; i < boxList.count(); i++)
		boxList[i].report();
	qDebug() << "*** END REPORT FOR BoxList ***";
}
