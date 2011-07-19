#ifndef VIEWER_H
#define VIEWER_H

//Main window of viewer application
//Has menu bars and handles actions
//Has a scroll area as its main widget for viewing the image
//Can call DbHandler class to read and write the surfaces, 
//inscriptions and inscriptionGraphs tables of ec.

#include <QMainWindow>
#include <QScrollArea>
#include "surface_imgs.h"
#include "db_handler.h"
#include "image_label.h"

// class QAction;
// class QMenu;
// class QScrollArea;
// class ImageLabel;

class Viewer : public QMainWindow
{
	Q_OBJECT //necessary for use of signals and slots
public:
	Viewer();	//constructor
			//creates DbHandler instance for communicating with ec
			//requests DbHandler to query surfaces
			//and sets up GUI
private slots:
	void advance(); //if in SURFACE mode
		//requests DbHandler to advance to next surface
		//reads new surface
		//and sets image in imageLabel.
	void back();
private:
//functions to set up UI
	void createActions();
	void createMenus();
	void createStatusBar();
	void createToolBars();
//actions
	QAction* exitAction; //quit application
	QAction* advanceAction; //increment the row in the db if in SURFACE mode
	QAction* backAction; 	//decrement the row in the db if in SURFACE mode
									//each saves the SURFACE to the db
	// TO DO 
//	QAction* saveAction; //save current state to db
//	QAction* discardAction; //discard changes and reload
	// END TO DO

	QAction* modeDownAction; //SURFACE > INSCRIPTION or INSCRIPTION > GRAPH
	QAction* modeUpAction; //GRAPH > INSCRIPTION or INSCRIPTION > SURFACE
									//each saves either the inscription list 
									//or the graph list to the db
			//for all of these operations, only existing rows in the db can be editied
			//no rows can be added or deleted
	QAction* unlockAction; //allows editing of bounding box list, which is 
						//read-only otherwise
//	QAction* saveThumbnailsAction; //save all current thumbnails to disk
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
//private data members
	ImageLabel* imageLabel; //widget that holds the image being viewed
	DbHandler db; //handles all SQL queries and other interaction with ec db
	QScrollArea* scrollArea; //provides scroll bars for large imageLabel
	SurfaceImgs surf; //complete representation of inscribed surface
			//passed as reference to imageLabel.
}; 

#endif
