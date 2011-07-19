#include "viewer.h"
// #include "imagelabel.h"
#include <QtGui>
#include <QDebug>
#include <QMessageBox>

Viewer::Viewer()
{
	//set up connection with db
	db = DbHandler(); 
	if(db.connect())
		qDebug() << "Connected to db.";
	else
		qDebug() << "Connection to db failed.";
	
	if(db.setCorpus())
		qDebug() << "Corpus set."; 
	else
		qDebug() << "No corpus found.";

	imageLabel = new ImageLabel(this, &surf);
	advance(); // to move to the first surface and display it

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
	scrollArea = new QScrollArea(this);
	scrollArea->setBackgroundRole(QPalette::Dark); //assigns bground color accroding to theme
	scrollArea->setWidget(imageLabel);	//scrollArea holds the imageLabel
	setCentralWidget(scrollArea); //makes scrollArea the central widget of the MainWindow (Viewer)
	showMaximized();
}


/*
int Viewer::graphsFromDb() //return zero if the currentBoxId 
									//has no corresponding inscription
									//otherwise returns the inscriptionId
{
	//called on transition to GRAPH from INSCRIPTION
	//we want all the graphs with the right inscriptionId
	//so determine that first by seeking the one corresponding to
	//currentBoxId
	QSqlQuery inscriptionIdQuery;
	QString inscriptionIdQueryString = QString(
										 		"SELECT id "
												"FROM hdInscriptions "
												"WHERE textSurfaceId=%1 "
												"ORDER BY id;")
												.arg(surfaceIdQuery->value(0).toString());
qDebug() << inscriptionIdQueryString;
	inscriptionIdQuery.exec(inscriptionIdQueryString);
	if(inscriptionIdQuery.seek(imageLabel->getCurrentBoxIndex()))
	{		//there is a corresponding documented inscription
		QSqlQuery getGraphQuery;
		QString getGraphQueryString = QString(
											 "SELECT x1, y1, x2, y2, rotation "
											 "FROM hdInscriptionGraphs "
											 "WHERE inscriptionId = %1 "
											 "ORDER BY graphId;")
										.arg(inscriptionIdQuery.value(0).toString());
qDebug() << getGraphQueryString;
		getGraphQuery.exec(getGraphQueryString);
		boxList.clear();
		bool done = false;
		while(!done)
		{
			if(getGraphQuery.next())
			{
				if(getGraphQuery.isNull(0))
				{
					done = true;
				}
				else
				{
					boxList.append(BoundingBox(QPoint(getGraphQuery.value(0).toInt(),
																 getGraphQuery.value(1).toInt()),
														QPoint(getGraphQuery.value(2).toInt(),
																 getGraphQuery.value(3).toInt()),
																 getGraphQuery.value(4).toInt()));
				}
			}
			else
			{
				done = true;
			}
		}
		return inscriptionIdQuery.value(0).toInt();
	}
	else
	{		//there is no corresponding documented inscription
		return 0;
	}
}


void Viewer::inscriptionsFromDb() 
//called on transition to INSCRIPTIONS from other mode
//surface id is already set
{
	//query: all inscriptions associated with a particular surface id
	QSqlQuery selectInscriptions;
	QString queryString("SELECT id, x1, y1, x2, y2, rotation "
							  "FROM hdInscriptions "
							  "WHERE textSurfaceId=");
	queryString += surfaceIdQuery->value(0).toString();
	queryString += " ORDER BY id;";
qDebug() << queryString;
	selectInscriptions.exec(queryString);
	boxList.clear();
	bool done = false;
	while(!done)
	{
		if(selectInscriptions.next())
		{
			if(selectInscriptions.isNull(1))
			{
				done=true;
			}
			else
			{
				boxList.append(BoundingBox(QPoint(selectInscriptions.value(1).toInt(), 
															 selectInscriptions.value(2).toInt()), 
											   	QPoint(selectInscriptions.value(3).toInt(), 
															 selectInscriptions.value(4).toInt()), 
											   		selectInscriptions.value(5).toInt()));
			}
		}
		else
		{
			done=true;
		}
	}
	//ImageLabel::refresh() sets current box to index=0 
}

void Viewer::graphsToDb()
{	//called on transition from GRAPH to INSCRIPTION
	//we only need to know the current inscription id and the box list
	if(!imageLabel->isLocked())
	{
		//get list of all documented graphs associated with the current inscription id
		QSqlQuery graphIdQuery;
		QString graphIdQueryString;
		graphIdQueryString = QString(
				"SELECT graphId "
				"FROM hdInscriptionGraphs "
				"WHERE inscriptionId = %1 "
				"ORDER BY graphId;")
				.arg(currentInscriptionId);
qDebug() << graphIdQueryString;
		graphIdQuery.exec(graphIdQueryString);
		for(int i=0; i<boxList.size(); i++)
		{
qDebug() << "i = " << i;
qDebug() << "boxList.size() = " << boxList.size();
			if(graphIdQuery.next()) //if we haven't exhausted the docuemented graphs
			{
				//get ith graph id
				int graphId = graphIdQuery.value(0).toInt();
qDebug() << "graphId = " << graphId;
				//then perform update
				QString updateGraphQueryString;
				updateGraphQueryString = QString(
						"UPDATE hdInscriptionGraphs "
						"SET x1=%1, y1=%2, x2=%3, y2=%4, rotation=%5 "
						"WHERE graphId=%6 AND inscriptionId=%7;")
					.arg(boxList.at(i).x())
					.arg(boxList.at(i).y())
					.arg(boxList.at(i).x() + boxList.at(i).width())
					.arg(boxList.at(i).y() + boxList.at(i).height())
					.arg(boxList.at(i).getRotation())
					.arg(graphId)
					.arg(currentInscriptionId);
qDebug() << updateGraphQueryString;
				QSqlQuery updateGraphQuery;
				updateGraphQuery.exec(updateGraphQueryString);
			}
			else //there are no more documented graphs
				//so discard rest of box list
				//we could change this subsequently
					//so that new row are added to ec.inscriptionGraphs instead
			{
				break; //NB!! ***********
			}
		}
		//fill remaining rows with NULL
		//note that this also deals with the case where the box list is empty
		while(graphIdQuery.next())
		{
			QSqlQuery updateGraphs;
			QString queryString;
			queryString = QString(
					"UPDATE hdInscriptionGraphs "
					"SET x1=NULL, y1=NULL, x2=NULL, y2=NULL, rotation=NULL "
					"WHERE graphId=%1 AND inscriptionId=%2;")
				.arg(graphIdQuery.value(0).toInt())
				.arg(currentInscriptionId);
qDebug() << queryString;
			updateGraphs.exec(queryString);
		}
	}
}
void Viewer::inscriptionsToDb()
//called on transition from INSCRIPTIONS from other mode
//we only need to know textSurfaceId and the box list
{
	if(!imageLabel->isLocked()) //if the user has not unlocked for editing
	{									//no point in saving to db
		//get list of all inscriptions associated with current surface id
		QSqlQuery inscriptionIdQuery;
		QString inscriptionIdQueryString;
		inscriptionIdQueryString = QString(
			"SELECT id "
			"FROM hdInscriptions "
			"WHERE textSurfaceId = %1 "
			"ORDER BY id;")
			.arg( surfaceIdQuery->value(0).toString() );
qDebug() << inscriptionIdQueryString;
		inscriptionIdQuery.exec(inscriptionIdQueryString);
		for(int i=0; i<boxList.size(); i++)
		{
			if(inscriptionIdQuery.next()) //if we haven't exhausted the documented inscriptions
			{
				//get ith inscription id
				int inscriptionId = inscriptionIdQuery.value(0).toInt();
				//then perform update
				QString updateQueryString;
				updateQueryString = QString("UPDATE hdInscriptions "
					"SET x1=%1, y1=%2, x2=%3, y2=%4, rotation=%5 "
					"WHERE id=%6;")
					.arg(boxList.at(i).x())
					.arg(boxList.at(i).y())
					.arg(boxList.at(i).x() + boxList.at(i).width())
					.arg(boxList.at(i).y() + boxList.at(i).height())
					.arg(boxList.at(i).getRotation())
					.arg(inscriptionId);
qDebug() << updateQueryString;
				QSqlQuery updateInscriptions;
				updateInscriptions.exec(updateQueryString);
			}
			else //there are no more documented inscriptions
				//so discard rest of box list
				//we could change this subsequently 
					//so that new rows are added to the table instead.
			{
				break; //NB!!! ********
			}
		}
		//fill the remaining rows with NULL
		//note that this also deals with the case where boxList is empty.
		while(inscriptionIdQuery.next())
		{
			QSqlQuery updateInscriptions;
			QString queryString;
			queryString = QString(
				"UPDATE hdInscriptions "
				"SET x1=NULL, y1=NULL, x2=NULL, y2=NULL, rotation=NULL "
				"WHERE id=%1")
				.arg(inscriptionIdQuery.value(0).toInt());
qDebug() << queryString;
			updateInscriptions.exec(queryString);
		}
	}
}

void Viewer::surfaceToDb()
{
	if(!imageLabel->isLocked())  //if the user has not unlocked for editing
	{				 					//no point in saving to db
		QSqlQuery updateQuery;
		QString queryString;
		if(boxList.isEmpty()) //user has deleted old box
		{							//write null values to db
			queryString = QString(
						"UPDATE hdTextSurfaces SET x1=NULL, y1=NULL, x2=NULL, y2=NULL, rotation=NULL WHERE id=");
			queryString += surfaceIdQuery->value(0).toString();
			queryString += ";";
		}
		else // user has replaced the old box with a new one
		{
			queryString = QString("UPDATE hdTextSurfaces SET x1="); //redo with arg for god's sake!!
			queryString += QString::number(boxList.first().x());
			queryString += QString(", y1=");
			queryString += QString::number(boxList.first().y());
			queryString += QString(", x2=");
			queryString += QString::number(boxList.first().x() + boxList.first().width());
			queryString += QString(", y2=");
			queryString += QString::number(boxList.first().y() + boxList.first().height());
			queryString += QString(", rotation=");
			queryString += QString::number(boxList.first().getRotation());
			queryString += QString(" WHERE id=");
			queryString += surfaceIdQuery->value(0).toString();
			queryString += ";";
		}
qDebug() << queryString;
		updateQuery.exec(queryString);
	}
}

void Viewer::surfaceFromDb()
{
	QSqlQuery selectQuery;
	QString queryString("SELECT imageFile, x1, y1, x2, y2, rotation FROM hdTextSurfaces WHERE id = ");
	queryString += surfaceIdQuery->value(0).toString();
	queryString += ";";
qDebug() << queryString;
	selectQuery.exec(queryString);
	if(selectQuery.next())
	{
		boxList.clear();
		if(!selectQuery.isNull(1)) //if the first x-coordinate is null, assume there are no surface coords
												//otherwise retrieve the bounding box
		{
			boxList.append(BoundingBox(QPoint(selectQuery.value(1).toInt(), selectQuery.value(2).toInt()), 
												QPoint(selectQuery.value(3).toInt(), selectQuery.value(4).toInt()), 
												selectQuery.value(5).toInt()));
		}
		QString fileName = selectQuery.value(0).toString();
		fileName.prepend("/home/ads/repository/text_imgs/hd/");
qDebug() << fileName;
		currentImage = QImage(fileName); // load image file from disk.
		imageLabel->refresh();
	}
	else
	{
qDebug() << "Bad query:" << queryString << "\n";
	}
}
*/

void Viewer::advance()
{
	if(imageLabel->getMode() == ImageLabel::SURFACE) //can only advance to a new surface from SURFACE mode
	{
		//TODO db.writeSurface(surf);
		db.nextSurface();
		db.readSurface(surf);
		imageLabel->newSurf();
	}
	//else do nothing
}

void Viewer::back()
{
	if(imageLabel->getMode() == ImageLabel::SURFACE)
	{
		//TODO db.writeSurface(surf) //save current state
		db.previousSurface();
		db.readSurface(surf);
		imageLabel->newSurf();
	}
}

void Viewer::createActions()
{

//	saveThumbnailsAction = new QAction(tr("&Save thumbnails"), this);
//	saveThumbnailsAction->setShortcut(tr("Ctrl+S"));
//	connect(saveThumbnailsAction, SIGNAL(triggered()), imageLabel, SLOT(saveThumbnails()));

	exitAction = new QAction(QIcon(":/icons/window-close.png"), tr("E&xit"), this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	advanceAction = new QAction(tr("&Advance"), this);
	advanceAction->setShortcut(tr("Ctrl+a"));
	connect(advanceAction, SIGNAL(triggered()), this, SLOT(advance()));

	backAction = new QAction(tr("&Back"), this);
	backAction->setShortcut(tr("Ctrl+b"));
	connect(backAction, SIGNAL(triggered()), this, SLOT(back()));

	modeDownAction = new QAction(tr("Mode Down"), this);
	modeDownAction->setShortcut(tr("Ctrl+Space"));
	connect(modeDownAction, SIGNAL(triggered()), imageLabel, SLOT(modeDown()));

	modeUpAction = new QAction(tr("Mode Up"), this);
	modeUpAction->setShortcut(tr("Escape"));
	connect(modeUpAction, SIGNAL(triggered()), imageLabel, SLOT(modeUp()));

	unlockAction = new QAction(tr("&Unlock"), this);
	unlockAction->setShortcut(tr("Ctrl+U"));
	connect(unlockAction, SIGNAL(triggered()), imageLabel, SLOT(unlock()));

	boxForwardAction = new QAction(tr("Box &Forward"), this);
	boxForwardAction->setShortcut(tr("]"));
	connect(boxForwardAction, SIGNAL(triggered()), imageLabel, SLOT(advanceCurrentBoxIndex()));

	boxBackAction = new QAction(tr("Box &Backward"), this);
	boxBackAction->setShortcut(tr("["));
	connect(boxBackAction, SIGNAL(triggered()), imageLabel, SLOT(reverseCurrentBoxIndex()));

	deleteCurrentBoxAction = new QAction(tr("&Delete box"), this);
	deleteCurrentBoxAction->setShortcut(tr("Backspace"));
	connect(deleteCurrentBoxAction, SIGNAL(triggered()), imageLabel, SLOT(deleteCurrentBox()));

	zoomInAction = new QAction(tr("&Zoom In"), this);
	zoomInAction->setShortcut(tr("Ctrl++"));
	connect(zoomInAction, SIGNAL(triggered()), imageLabel, SLOT(zoomIn()));

	zoomOutAction = new QAction(tr("Zoom &Out"), this);
	zoomOutAction->setShortcut(tr("Ctrl+-"));
	connect(zoomOutAction, SIGNAL(triggered()), imageLabel, SLOT(zoomOut()));

	zoomRestoreAction = new QAction(tr("&Restore"), this);
	zoomRestoreAction->setShortcut(tr("Ctrl+0"));
	connect(zoomRestoreAction, SIGNAL(triggered()), imageLabel, SLOT(zoomRestore()));

	rotateClockwiseAction = new QAction(tr("&Clockwise 5 deg"), this);
	rotateClockwiseAction->setShortcut(tr("Ctrl+>"));
	connect(rotateClockwiseAction, SIGNAL(triggered()), imageLabel, SLOT(rotateClockwise()));

	rotateAntiClockwiseAction = new QAction(tr("&Anticlockwise 5 deg"), this);
	rotateAntiClockwiseAction->setShortcut(tr("Ctrl+<"));
	connect(rotateAntiClockwiseAction, SIGNAL(triggered()), imageLabel, SLOT(rotateAntiClockwise()));

	rotateRestoreAction = new QAction(tr("&Restore"), this);
	rotateRestoreAction->setShortcut(tr("Ctrl+^"));
	connect(rotateRestoreAction, SIGNAL(triggered()), imageLabel, SLOT(rotateRestore()));

	toggleIndexNumbersAction = new QAction(tr("&Toggle Index Numbers"), this);
	toggleIndexNumbersAction->setShortcut(tr("Ctrl+I"));
	connect(toggleIndexNumbersAction, SIGNAL(triggered()), imageLabel, SLOT(toggleIndexNumbers()));
}

void Viewer::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(exitAction);
//	fileMenu->addAction(saveThumbnailsAction);

	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(boxForwardAction);
	editMenu->addAction(boxBackAction);
	editMenu->addAction(deleteCurrentBoxAction);
	editMenu->addAction(advanceAction);
	editMenu->addAction(unlockAction);
	editMenu->addAction(backAction);
	editMenu->addAction(modeUpAction);
	editMenu->addAction(modeDownAction);

	zoomMenu = menuBar()->addMenu(tr("&Zoom"));
	zoomMenu->addAction(zoomInAction);
	zoomMenu->addAction(zoomOutAction);
	zoomMenu->addAction(zoomRestoreAction);

	rotateMenu = menuBar()->addMenu(tr("&Rotate"));
	rotateMenu->addAction(rotateClockwiseAction);
	rotateMenu->addAction(rotateAntiClockwiseAction);
	rotateMenu->addAction(rotateRestoreAction);

	viewMenu = menuBar()->addMenu(tr("&View"));
	viewMenu->addAction(toggleIndexNumbersAction);
}

void Viewer::createToolBars()
{
	toolBar = addToolBar(tr("Tool Bar"));
	toolBar->addAction(exitAction);
	toolBar->addAction(zoomInAction);
	toolBar->addAction(zoomOutAction);
	toolBar->addAction(rotateClockwiseAction);
	toolBar->addAction(rotateAntiClockwiseAction);
}

void Viewer::createStatusBar()
{
	QFont statusBarFont = (QString("HuaDong"));
	statusBar()->setFont(statusBarFont);
	statusBar()->showMessage(tr("This is the status bar"));
}
