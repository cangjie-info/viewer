#ifndef EDIT_TRANSCRIPTION_DIALOG_H
#define EDIT_TRANSCRIPTION_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QWidget>
#include "inscription_transcription.h"
#include <QList>
#include <QImage>

//modal dialog for editing trancsriptions

class EditTranscriptionDialog : public QDialog
{
public:
	EditTranscriptionDialog(QWidget* parent, InscriptionTranscription* const trans, QList<QImage>& imgList); //constructor
private:
	QLabel* transLabel; //label that holds the transcription string
};


#endif
 
