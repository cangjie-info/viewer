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

	//create TranscriptionWindow as a dock widget
	dock = new QDockWidget(this);
	dock->setAllowedAreas(Qt::TopDockWidgetArea);
	addDockWidget(Qt::TopDockWidgetArea, dock);

	createStatusBar();
	createActions();
	createMenus();
	createToolBars();
	scrollArea = new QScrollArea(this);
	scrollArea->setBackgroundRole(QPalette::Dark); //assigns bground color accroding to theme
	scrollArea->setWidget(imageLabel);	//scrollArea holds the imageLabel
	setCentralWidget(scrollArea); //makes scrollArea the central widget of the MainWindow (Viewer)
	showMaximized();
	advance(); // to move to the first surface and display it
}

void Viewer::advance()
{
	if(imageLabel->getMode() == ImageLabel::SURFACE) //can only advance to a new surface from SURFACE mode
	{
		//TODO db.writeSurface(surf, trans);
		db.nextSurface(); //stays put if already on last record
		db.readSurface(surf, trans);
		imageLabel->newSurf();

		//delete old transcription window and create new one
		if(transWindow) 
			delete transWindow;
		transWindow = new TranscriptionWindow(this, &trans);

		//connect ImageLabel signals to transcription window slots
		connect(imageLabel, SIGNAL(inscrImgListModified()), transWindow, SLOT(refresh()));

		//install transcription window in dock
		dock->setWidget(transWindow);
		transWindow->refresh();
		transWindow->show(); //or is it the dock that we need to show()???
	}
	//else do nothing
}

void Viewer::back()
{
	if(imageLabel->getMode() == ImageLabel::SURFACE)
	{
		//TODO db.writeSurface(surf) //save current state
		db.previousSurface(); //stays put if already on first record
		db.readSurface(surf);
		imageLabel->newSurf();
		if(transWindow) 
			delete transWindow;
		transWindow = new TranscriptionWindow(this, &trans);
		dock->setWidget(transWindow);
		transWindow->refresh();
		transWindow->show(); //or is it the dock that we need to show()???
	}
}

void refreshTransWindow()
{
	transWindow->refresh();
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
		//TODO call to TranscriptionWindow::refresh()
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
