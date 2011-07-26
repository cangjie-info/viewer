#include "viewer.h"
// #include "imagelabel.h"
#include <QtGui>
#include <QDebug>
#include <QMessageBox>

Viewer::Viewer()
	: imageLabel(NULL), transWindow(NULL)
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

	//create ImageLabel - for displaying the surface images
	imageLabel = new ImageLabel(this, &surf); //surf is passed as pointer
			//so that imageLabel can manipulate it.

	//create dock and scroll area for transWindow 
	dock = new QDockWidget(this);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, dock);
	transScrollArea = new QScrollArea(this);
	transScrollArea->setMinimumWidth(500);
//	transScrollArea->setBackgroundRole(QPalette::Dark);
	dock->setWidget(transScrollArea);

	createActions();
	createMenus();
	createToolBars();
	imgScrollArea = new QScrollArea(this);
	imgScrollArea->setBackgroundRole(QPalette::Dark); //assigns bground color accroding to theme
	imgScrollArea->setWidget(imageLabel);	//scrollArea holds the imageLabel
	setCentralWidget(imgScrollArea); //makes scrollArea the central widget of the MainWindow (Viewer)
	
	showMaximized();
	advance(); // to move to the first surface and display it
}

void Viewer::newSurf()
{
		db.readSurface(surf, trans);

		locked = true;
		modified = false;

		imageLabel->newSurf();
		//delete old transcription window and create new one
		if(transWindow) 
			delete transWindow;
		transWindow = new TranscriptionWindow(&trans, &surf);
		//connect ImageLabel signals to transcription window slots
		connect(imageLabel, SIGNAL(inscrImgListModified()), transWindow, SLOT(refresh()));
		connect(imageLabel, SIGNAL(inscrImgListModified()), this, SLOT(setModified()));
		//connect Actions to transcription window slots
		connect(toggleCanHaveImageAction, SIGNAL(triggered()), transWindow, SLOT(toggleCanHaveImage()));
		connect(nextTransAction, SIGNAL(triggered()), transWindow, SLOT(nextInscription()));
		connect(prevTransAction, SIGNAL(triggered()), transWindow, SLOT(prevInscription()));
		connect(deleteTransAction, SIGNAL(triggered()), transWindow, SLOT(deleteInscription()));
		connect(insertTransAction, SIGNAL(triggered()), transWindow, SLOT(insertInscription()));
		connect(raiseTransAction, SIGNAL(triggered()), transWindow, SLOT(raiseInscription()));
		connect(lowerTransAction, SIGNAL(triggered()), transWindow, SLOT(lowerInscription()));
		connect(allCanHaveImageAction, SIGNAL(triggered()), transWindow, SLOT(allCanHaveImage()));
		connect(this, SIGNAL(unlockSignal()), transWindow, SLOT(unlock()));
		connect(this, SIGNAL(unlockSignal()), imageLabel, SLOT(unlock()));

		connect(transWindow, SIGNAL(inscrListModified()), this, SLOT(setModified()));

		//install transcription window in scroll area in dock
		transScrollArea->setWidget(transWindow);

		transWindow->show(); //or is it the dock that we need to show()???

		statusUpdate();
}
	
void Viewer::advance()
{
	if(imageLabel->getMode() == ImageLabel::SURFACE) //can only advance to a new surface from SURFACE mode
	{
		//TODO db.writeSurface(surf, trans);
		db.nextSurface(); //stays put if already on last record
		newSurf();
	}
	//else do nothing
}

void Viewer::back()
{
	if(imageLabel->getMode() == ImageLabel::SURFACE)
	{
		//TODO db.writeSurface(surf) //save current state
		db.previousSurface(); //stays put if already on first record
		newSurf();
	}
}

void Viewer::unlock()
{
	//unlock imageLabel and transcription window by emitting unlock signal
	emit unlockSignal();
	locked = false;
	statusUpdate();
}

void Viewer::statusUpdate()
{
	QString statusText;
	statusText += QString(
		"surface: %1, surface type: %2 | zoom = x%3, rotation = %4 | mode = %5 | %6LOCKED%7")
		.arg(trans.getPubId() + trans.getPubNumber())
		.arg(trans.getSurfaceType())
		.arg(imageLabel->getZoom())
		.arg(imageLabel->getRotation())
		.arg(imageLabel->getModeName())
		.arg(locked ? "" : "UN")
		.arg(modified ? " | MODIFIED" : "");
	statusBar()->showMessage(statusText);
}

void Viewer::setModified()
{
	modified = true;
	statusUpdate();
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
	advanceAction->setShortcut(tr("a"));
	connect(advanceAction, SIGNAL(triggered()), this, SLOT(advance()));

	backAction = new QAction(tr("&Back"), this);
	backAction->setShortcut(tr("b"));
	connect(backAction, SIGNAL(triggered()), this, SLOT(back()));

	modeDownAction = new QAction(tr("Mode Down"), this);
	modeDownAction->setShortcut(tr("Space"));
	connect(modeDownAction, SIGNAL(triggered()), imageLabel, SLOT(modeDown()));
	connect(modeDownAction, SIGNAL(triggered()), this, SLOT(statusUpdate()));

	modeUpAction = new QAction(tr("Mode Up"), this);
	modeUpAction->setShortcut(tr("Escape"));
	connect(modeUpAction, SIGNAL(triggered()), imageLabel, SLOT(modeUp()));
	connect(modeUpAction, SIGNAL(triggered()), this, SLOT(statusUpdate()));

	unlockAction = new QAction(tr("&Unlock"), this);
	unlockAction->setShortcut(tr("U"));
	connect(unlockAction, SIGNAL(triggered()), this, SLOT(unlock()));

	boxForwardAction = new QAction(tr("Box &Forward"), this);
	boxForwardAction->setShortcut(tr("]"));
	connect(boxForwardAction, SIGNAL(triggered()), imageLabel, SLOT(advanceCurrentBoxIndex()));

	boxBackAction = new QAction(tr("Box &Backward"), this);
	boxBackAction->setShortcut(tr("["));
	connect(boxBackAction, SIGNAL(triggered()), imageLabel, SLOT(reverseCurrentBoxIndex()));

	deleteCurrentBoxAction = new QAction(tr("&Delete box"), this); 
		//TODO call to TranscriptionWindow::refresh()
	deleteCurrentBoxAction->setShortcut(tr("Backspace"));
	connect(deleteCurrentBoxAction, SIGNAL(triggered()), imageLabel, SLOT(deleteCurrentBox()));

	zoomInAction = new QAction(tr("&Zoom In"), this);
	zoomInAction->setShortcut(tr("+"));
	connect(zoomInAction, SIGNAL(triggered()), imageLabel, SLOT(zoomIn()));
	connect(zoomInAction, SIGNAL(triggered()), this, SLOT(statusUpdate()));

	zoomOutAction = new QAction(tr("Zoom &Out"), this);
	zoomOutAction->setShortcut(tr("-"));
	connect(zoomOutAction, SIGNAL(triggered()), imageLabel, SLOT(zoomOut()));
	connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(statusUpdate()));

	zoomRestoreAction = new QAction(tr("&Restore"), this);
	zoomRestoreAction->setShortcut(tr("0"));
	connect(zoomRestoreAction, SIGNAL(triggered()), imageLabel, SLOT(zoomRestore()));
	connect(zoomRestoreAction, SIGNAL(triggered()), this, SLOT(statusUpdate()));

	rotateClockwiseAction = new QAction(tr("&Clockwise 5 deg"), this);
	rotateClockwiseAction->setShortcut(tr(">"));
	connect(rotateClockwiseAction, SIGNAL(triggered()), imageLabel, SLOT(rotateClockwise()));
	connect(rotateClockwiseAction, SIGNAL(triggered()), this, SLOT(statusUpdate()));

	rotateAntiClockwiseAction = new QAction(tr("&Anticlockwise 5 deg"), this);
	rotateAntiClockwiseAction->setShortcut(tr("<"));
	connect(rotateAntiClockwiseAction, SIGNAL(triggered()), imageLabel, SLOT(rotateAntiClockwise()));
	connect(rotateAntiClockwiseAction, SIGNAL(triggered()), this, SLOT(statusUpdate()));

	rotateRestoreAction = new QAction(tr("&Restore"), this);
	rotateRestoreAction->setShortcut(tr("^"));
	connect(rotateRestoreAction, SIGNAL(triggered()), imageLabel, SLOT(rotateRestore()));
	connect(rotateRestoreAction, SIGNAL(triggered()), this, SLOT(statusUpdate()));

	toggleIndexNumbersAction = new QAction(tr("&Toggle Index Numbers"), this);
	toggleIndexNumbersAction->setShortcut(tr("i"));
	connect(toggleIndexNumbersAction, SIGNAL(triggered()), imageLabel, SLOT(toggleIndexNumbers()));

	toggleCanHaveImageAction = new QAction(tr("Toggle &No Image"), this);
	toggleCanHaveImageAction->setShortcut(tr("n"));
	//connect must follow creation of transWindow (et seq.)

	nextTransAction = new QAction(tr("Next transcription"), this);
	nextTransAction->setShortcut(Qt::Key_Down + Qt::ShiftModifier);
	//ditto

	prevTransAction = new QAction(tr("Previous transcription"), this);
	prevTransAction->setShortcut(Qt::Key_Up + Qt::ShiftModifier);
	//ditto

	deleteTransAction = new QAction(tr("Delete transcription"), this);
	deleteTransAction->setShortcut(Qt::ShiftModifier + Qt::Key_Backspace);
	//ditto

	insertTransAction = new QAction(tr("Insert transcription"), this);
	insertTransAction->setShortcut(Qt::Key_Return);
	//ditto

	raiseTransAction = new QAction(tr("Raise current transcription index"), this);
	raiseTransAction->setShortcut(Qt::Key_Down + Qt::ControlModifier);
	//ditto

	lowerTransAction = new QAction(tr("Lower current transcription index"), this);
	lowerTransAction->setShortcut(Qt::Key_Up + Qt::ControlModifier);
	//ditto

	allCanHaveImageAction = new QAction(tr("All can have images"), this);
	allCanHaveImageAction->setShortcut(tr("Ctrl+n"));
	//ditto
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
	editMenu->addAction(toggleCanHaveImageAction);
	editMenu->addAction(nextTransAction);
	editMenu->addAction(prevTransAction);
	editMenu->addAction(deleteTransAction);
	editMenu->addAction(insertTransAction);
	editMenu->addAction(raiseTransAction);
	editMenu->addAction(lowerTransAction);
	editMenu->addAction(allCanHaveImageAction);

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
/*
DELETE?
void Viewer::createStatusBar()
{
	QFont statusBarFont = (QString("HuaDong"));
	statusBar()->setFont(statusBarFont);
	statusBar()->showMessage(tr("This is the status bar"));
}

*/
