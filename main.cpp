#include <QApplication>
#include <QDebug>
#include "viewer.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	Q_INIT_RESOURCE(viewer); //initializes resource file for viewer
							//resource file includes icons etc.
							//should be called from main()
	
	Viewer viewer;
	viewer.show();
/*
//test DbHandler class
	DbHandler dbHandler;
	if(!dbHandler.connect())
	{
		return 1;
	}
	else 
	{
		qDebug() << "successfully connected to db.";
	}

	dbHandler.setCorpus();
	dbHandler.nextSurface();
	dbHandler.nextSurface();
	dbHandler.nextSurface();
	dbHandler.nextSurface();
	dbHandler.previousSurface();
	qDebug() << "position in corpus =" << dbHandler.getPositionInCorpus()
		<< "/" << dbHandler.getCorpusSize();

	SurfaceImgs surf;
	dbHandler.readSurface(surf);
	surf.report();

//test SurfaceImgs class
	BoundingBox surfBox(QPoint(10, 10), QPoint(1000, 1000), 0, false);
	surfBox.setBox(QPoint(333, 333), QPoint(666, 666), 180, true);
	SurfaceImgs mySurface(surfBox);
	mySurface.insertInscr(BoundingBox(QPoint(30, 30), QPoint(70, 70), 30, false), 0);
	InscriptionImgs* ptrInscr = mySurface.ptrInscrAt(0);
	ptrInscr->BoundingBox::toggleNull();
	ptrInscr->insertBox(BoundingBox(QPoint(40, 40), QPoint(50, 50), 0, false), 0);
	ptrInscr->insertBox(BoundingBox(QPoint(55, 55), QPoint(65, 65), 30, false), 0);
	mySurface.report();
*/
	return app.exec(); 
}
