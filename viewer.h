#ifndef VIEWER_H
#define VIEWER_H

//Main window of viewer application
//Has menu bars and handles actions
//Has a scroll area as its main widget for viewing the image
//Can call DbHandler class to read and write the surfaces, 
//inscriptions and inscriptionGraphs tables of ec.

#include <QMainWindow>
#include <QList>
//DELETE #include <QSqlQuery>
#include "surface.h"

class QAction;
class QMenu;
class QScrollArea;
class ImageLabel;
//DELETE class DbHandler; //included already
//TODO class Surface;

class Viewer : public QMainWindow
{
	Q_OBJECT //necessary for use of signals and slots

public:
	Viewer();	//constructor
			//creates DbHandler instance for communicating with ec
			//intializes variables 
			//requests DbHandler to query surfaces
			//and sets up GUI

	enum Mode {SURFACE, INSCRIPTION, GRAPH}; //used to control the behavior of viewer
	Mode getMode(); //return current mode

/* TODO

private slots:
	void advance(); //if safe to do so, 
			//requests DbHandler to advance to next surface
			//reads new currentSurface
			//and sets image in imageLabel.
	void back();
	void modeUp();
	void modeDown();

private:
//functions to set up UI
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();

//actions
	QAction* exitAction; //quit application

	QAction* advanceAction; //increment the row in the db if in SURFACE mode
	QAction* backAction; 	//decrement the row in the db if in SURFACE mode
									//each saves the SURFACE to the db
	// TO DO 
	QAction* saveAction; //save current state to db
	QAction* discardAction; //discard changes and reload
	// END TO DO

	QAction* modeDownAction; //SURFACE > INSCRIPTION or INSCRIPTION > GRAPH
	QAction* modeUpAction; //GRAPH > INSCRIPTION or INSCRIPTION > SURFACE
									//each saves either the inscription list 
									//or the graph list to the db
			//for all of these operations, only existing rows in the db can be editied
			//no rows can be added or deleted

	QAction* unlockAction; //allows editing of bounding box list, which is 
						//read-only otherwise

	QAction* saveThumbnailsAction; //save all current thumbnails to disk
	QAction* zoomInAction; 	//image * 1+1/3
	QAction* zoomOutAction;	//image * 3/4 
	QAction* zoomRestoreAction;	//restore original size
	QAction* rotateClockwiseAction; // +5 degrees
	QAction* rotateAntiClockwiseAction; // -5 degrees
	QAction* rotateRestoreAction; // resent rotation to 0
	QAction* toggleIndexNumbersAction; //switches index numbers for bounding boxes on and off
	QAction* boxForwardAction; //advances box index
	QAction* boxBackAction; //box back by one
	QAction* deleteCurrentBoxAction; //deletes current boudning box
	//TODO QAction* setCorpus; //sets WHERE clause for querying surfaces table

//menus
	QMenu* fileMenu;
	QMenu* editMenu;
	QMenu* zoomMenu;
	QMenu* rotateMenu;
	QMenu* viewMenu;

//toolbars
	QToolBar* toolBar;

//private functions
	void setMode(Mode); //sets mode and performs related stuff
							//like setting maxListLength in ImageViewer
	void unlock(); //unlocks image for editing.
	void surfaceToDb(); //writes current state of text surface to db
	void surfaceFromDb(); //loads a new surface from db
	void inscriptionsToDb();
	void inscriptionsFromDb();
	void graphsToDb();
	int graphsFromDb();
//private data members

	PUT THIS IN DbHandler...  QSqlQuery* surfaceIdQuery; //keeps track of the current row in textSurfaces
*/	
	QImage originalImage; //the image loaded from repository
				//stored while mode = GRAPH or INSCRIPTION
	QImage surfaceImage; //image cropped and rotated to surface
				//stored while mode = GRAPH
	QImage currentImage; //the image (original, surface or inscription depending on mode) 
								//currently visible in the viewer.
	ImageLabel* imageLabel; //widget that holds the image being viewed
	QScrollArea* scrollArea; //provides scroll bars for large imageLabel
/*
	QList<BoundingBox> boxList;
	QList<BoundingBox> surfaceList; //used to store boxList on modeDown from SURFACE
						//	retrieved at modeUp from INSCTIPTION.
	QList<BoundingBox> inscriptionList; //use to store boxList on modeDown
						//from INSCRIPTION, retrieved at modeUp from Graph;
*/
	Surface currentSurface; //complete representation of inscribed surface
	int currentInscriptionIndex; // undefined in SURFACE mode
				// = 0 on entering INSCRIPTIONS mode, unless there are no inscritpions
							// in which case = -1
				// stores current inscription index while in GRAPHS mode
	int currentGraphIndex; // undefined in SURFACE and INSCRIPTIONS modes
				// = 0 on entry to GRAPHS mode, unless there are no GRAPHS in which 
				// case = -1
	Mode mode; 	//used to control the behavior of the viewer
					//moves between SURFACE, INSCRIPTION, and GRAPH
	bool surfaceModified; //TRUE if currentSurface has been modifed but not saved.
	DbHandler db; //handles all SQL queries and other interaction with ec db
}; 

#endif
