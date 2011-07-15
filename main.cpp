#include <QApplication>
#include <QDebug>
#include "bounding_box.h"
#include "box_list.h"
#include "inscription_imgs.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
/*
//test BoundingBox class

	BoundingBox myBox(QPoint(10, 10), QPoint(20, 20), 45, false);
	myBox.report();
	qDebug() << "testing member function getRotation(): returns" << myBox.getRotation();
	qDebug() << "testing member function setNull() ...";
	myBox.setNull(true);
	qDebug() << "testing member function boxIsNull(): returns" << myBox.boxIsNull();
	myBox.report();

//test BoxList class

	BoxList myBoxList;
	qDebug() << "testing insertBox(...)";
	for (int i=0; i<3; i++)
		myBoxList.insertBox(myBox, 0);
	myBoxList.report();
	qDebug() << "testing deleteBox(...)";
	myBoxList.deleteBox(3);
	myBoxList.deleteBox(2);
	myBoxList.deleteBox(0);
	myBoxList.report();
	qDebug() << "testing toggleNull(...)";
	myBoxList.toggleNull(0);
	qDebug() << "testing boxAt(...)";
	myBoxList.boxAt(0).report();
	qDebug() << "testing boxNullAt(...)";
	qDebug() << "Box at 0 is" << (myBoxList.boxNullAt(0) ? "" : "not") << "null";
	qDebug() << "testing boxCount(): number of boxes = " << myBoxList.boxCount();

*/

//test InscripitonImgs class

	BoundingBox myBox(QPoint(10, 10), QPoint(20, 20), 45, false);
	BoundingBox graphBox(QPoint(100, 100), QPoint(200, 200), 0, false);
	InscriptionImgs myInscription(myBox);
	for(int i=0; i<3; i++)
		myInscription.insertBox(graphBox, 0);
	myInscription.report();

/*

	InscriptionGraph graph1(myBox, false, 1, 1);
	InscriptionGraph graph2(myBox, true, 1, 1);
	InscriptionGraph graph3(myBox, false, 0, 0);

	Inscription myInscr(BoundingBox(QPoint(0,0), QPoint(0,0), 0), false);
	for(int i = 0; i<5; i++)
	{
		myInscr.insertGraph(graph2, 0);
	}
	myInscr.report(); //there should be 5 graphs.

	for(int j = 0; j<5; j++)
	{
		myInscr.insertGraphBox(myBox, false, 0);
	}
	
	myInscr.report(); //there should be 5 graphs.

	for(int k = 0; k<5; k++)
	{
		myInscr.insertGraphBox(myBox, false, 0);
	}
	myInscr.report(); //there should now be 10 graphs.

	for(int l=0; l<11; l++)
	{
		myInscr.deleteGraphBox(0);
	}
	myInscr.report(); // there should be 5 graphs all with null boxes

	for(int m=0; m<10; m++)
	{
		myInscr.deleteGraph(0);
	}
	myInscr.report(); //graphList should be empty
*/

	return app.exec(); 
}
