#ifndef EDIT_TRANSCRIPTION_DIALOG_H
#define EDIT_TRANSCRIPTION_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QWidget>
#include "inscription_transcription.h"
#include <QList>
#include <QImage>
#include <QPushButton>

//modal dialog for editing trancsriptions

class EditTranscriptionDialog : public QDialog
{
	Q_OBJECT
public:
	EditTranscriptionDialog(QWidget* parent, InscriptionTranscription* const trans, QList<QImage>& imgList); //constructor
private slots:
	void okClicked();
private:
	//methods
	void keyPressEvent(QKeyEvent* event); //reimplementation
	void refreshTrans(); //sets text in transLabel
	//variables
	QLabel* transLabel; //label that holds the transcription string
	QPushButton* okButton; //click to accept changes to trans
	QPushButton* cancelButton; //click to leave trans unchanged
	InscriptionTranscription transCopy; //working copy of transcription to be edited, 
			//so changes can be cancelled.
	InscriptionTranscription* trans; //pointer to transcription to be edited
	QList<QImage>& imgList; //ref to list of images of graphs
	int index; //position in transcription for insertion, deletion, etc.
};

#endif
