#ifndef VIEWER_H
#define VIEWER_H

//Main window of viewer application
//Has menu bars and handles actions
//Has a scroll area as its main widget
//with an image label for viewing the image
//Creates an instance of DbHandler to read and write the surfaces, 
//inscriptions and inscriptionGraphs tables of ec.

#include <QMainWindow>
#include <QScrollArea>
#include <QDockWidget>
#include <QDialog>
#include "surface_imgs.h"
#include "db_handler.h"
#include "image_label.h"
#include "surface_transcription.h"
#include "transcription_window.h"
#include "config_handler.h"

class Viewer : public QMainWindow
{
    Q_OBJECT //necessary for use of signals and slots
public:
    Viewer();	//constructor
    //sets DbHandler instance for communicating with ec
    //requests DbHandler to query surfaces
    //sets up GUI
    //advances to first surface
public slots:
    void statusUpdate(); //write status bar text. TODO: make private?
    void setModified(); //sets modified to true, connect to signals in imageLabel and TranscriptionWindow
signals:
    void unlockSignal();
private slots:
    void advance(); //if in SURFACE mode
    //requests DbHandler to advance to next surface
    //reads new surface
    //and sets image in imageLabel.
    void back();
    //TODO advance(int jump), back(int jump), advanceTenPc(), moveLast(), etc.
    void unlock();
    void editTranscription(); //opens dialog to edit currnet transcription
    void toggleFullScreen();
    void save(); //writes surface to db
    void discardChanges(); //discards all changes and reloads surface from db

private:
    void newSurf(); //reads surface from db
    //sets up transcriptions window in scroll area
    //connects up signals and slots
    //updates status bar
    //functions to set up UI
    void closeEvent(QCloseEvent *event);
    void createActions();
    void createMenus();
    //actions
    QAction* exitAction; //quit application
    QAction* advanceAction; //increment the row in the db if in SURFACE mode
    QAction* backAction; 	//decrement the row in the db if in SURFACE mode
    QAction* unlockAction; //allows editing of surface
    QAction* toggleFullScreenAction;
    QAction* saveAction; //save current state to db
    QAction* discardChangesAction; //discard changes and reload from db

    //actions related to imgs
    QAction* modeDownAction; //SURFACE > INSCRIPTION or INSCRIPTION > GRAPH
    QAction* modeUpAction; //GRAPH > INSCRIPTION or INSCRIPTION > SURFACE
    //TODO	QAction* saveThumbnailsAction; //save all current thumbnails to disk
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
    QAction* toggleCanHaveImageAction; //toggles canHaveImage variable of currnet inscription
    QAction* raiseBoxAction; //swaps current bbox with bbox of next higher index
    //and increments current box index
    QAction* lowerBoxAction; //swaps current bbox with bbox of next lower index
    //and decrements current box index

    //actions related to transcriptions
    QAction* nextTransAction; //increments current transcription
    QAction* prevTransAction; //decrements current transcription
    QAction* deleteTransAction; //deletes current transcription
    QAction* insertTransAction; //inserts transctption before current
    QAction* raiseTransAction; //raises index number of current transcription
    QAction* lowerTransAction; //lowers index number of current transcription
    QAction* allCanHaveImageAction; //sets canHaveImage to true for all transcriptions
    QAction* editTranscriptionAction; //opens dialog window for editing a single insciption
    QAction* copyTransAction; //duplicates current inscription transcription and
        //inserts it at current index
            //TODO QAction* setCorpus; //sets WHERE clause for querying surfaces table
            //menus
            QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* zoomMenu;
    QMenu* rotateMenu;
    QMenu* viewMenu;

    //private data members
    ConfigHandler* config; //handles interaction with configuration file
    ImageLabel* imageLabel; //widget that holds and manipulates the image being viewed
    DbHandler db; //handles all SQL queries and other interaction with ec db
        //TODO: reimplement as namespace
    QScrollArea* imgScrollArea; //provides scroll bars for large imageLabel
    QScrollArea* transScrollArea; //provides scroll bars for the transcriptions list
    SurfaceImgs surf; //complete representation of inscribed surface
    //passed as reference to imageLabel.
    SurfaceTranscription trans; //stores complete transcription and markup for one surface
    //consists of data for surface, plus a list of inscriptionTrans, each of which
    //consists of a list of graphTrans.
    TranscriptionWindow* transWindow; 	//widget for displaying and editing
    //sets of transcriptions.
    QDockWidget* dock;// dock window for the TranscriptinWindow.
    bool locked; //prevents any modification TODO: differnet kinds of locks, e.g. that allo
    //addition but not deletion...
    bool modified; //true if either surf or trans has been modified since last save
}; 

#endif
