#ifndef EDIT_TRANSCRIPTION_DIALOG_H
#define EDIT_TRANSCRIPTION_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QWidget>
#include "inscription_transcription.h"
#include <QList>
#include <QImage>
#include <QPushButton>
#include <QTextEdit>
#include <QAction>
#include <QMenu>
#include <QMenuBar>

//modal dialog for editing trancsriptions

class EditTranscriptionDialog : public QDialog
{
    Q_OBJECT
public:
    EditTranscriptionDialog(    QWidget* parent,
                                InscriptionTranscription* const trans,
                                QList<QImage>& imgList); //constructor
private slots:
    void okClicked();
    void processInput(); //handles input from inputBox
    void graphLeft(); //moves edit point to the left
    void graphRight(); //moves edit point to the right
    void toggleNull(); //toggles null image at edit point
    void clearAllNulls(); //toggles all null images
    void deleteGraph(); //deletes graph at edit point
    void insertGraph(); //use input box to insert graph(s)
private:
    //methods
    void refreshTrans(); //sets text in transLabel
    void refreshGraph(); //sets graph image, or "no image", or ""
    //funtions to set up UI
    void createActions();
    void createMenus();
    //actions
    QAction* graphLeftAction;
    QAction* graphRightAction;
    QAction* cancelAction;
    QAction* okAction;
    QAction* toggleNullAction;
    QAction* clearAllNullsAction;
    QAction* deleteGraphAction;
    QAction* insertGraphAction;
    //menu bar and menus
    QMenuBar* dialogMenuBar;
    QMenu* fileMenu;
    QMenu* editMenu;



    //private data members
    QLabel* graphLabel; //holds image of current graph, or "no image"
    QLabel* transLabel; //label that holds the transcription string
    QTextEdit* inputBox; //text box for inputting transcription
    QPushButton* okButton; //click to accept changes to trans
    QPushButton* cancelButton; //click to leave trans unchanged
    InscriptionTranscription transCopy; //working copy of transcription to be edited,
    //so changes can be cancelled.
    InscriptionTranscription* trans; //pointer to transcription to be edited
    QList<QImage>& imgList; //ref to list of images of graphs
    int index; //position in transcription for insertion, deletion, etc.
};

#endif
