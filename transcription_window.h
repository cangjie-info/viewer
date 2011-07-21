#ifndef TRANSCRIPTION_WINDOW_H
#define TRANSCRIPTION_WINDOW_H

//class to display and edit set of transcriptions 
//for a single surface
//has read/write access to SurfaceTranscription instance 
//passed by const pointer from Viewer.
//TODO has read-only access to SurfaceImages instance passed
//by const pointer to const from Viewer.

//instance constructed by Viewer::advance() or back(), destroying 
//previous instance, in a dock window created by Viewer::Viewer() constructor
//constructor call refresh() which creates list of 
//Inscription labels.

#include <QWidget>
#include <QLabel>
#include <QList>
#include "inscription_label.h"
#include "surface_transcription.h"

class TranscriptionWindow : public QWidget
{
	Q_OBJECT
public:
	TranscriptionWindow(SurfaceTranscription* const trans /*TODO , const SurfaceImgs* const imgs */); //constructor
		//creates surfaceLabel and calls refresh()
	void refresh(); //generates InscriptionLabels and adds them as children
		//also generates a set of QLabels for numbering the InscriptionLabels, and 
		//indicating whether they have/may have corresponding images or not.
		//number of Inscription labels = max of number of InscriptionImgs and InscriptionTranscriptions
		//also adds an "append inscription" label.
		//refresh() should be called as a slot by a signal emitted by ImageLabel::mouseUp. (i.e. on creation
		//of new image bbox. Also called on ImageLabel::deleteBox.
//NO. PUT IN VIEWER.	void editCurrentInscription(); //creates new pop-up dialog to edit the current inscription
	void toggleCanHaveImg(bool can);	//sets the current inscription's canHaveImage, 
		//modifies surfTrans (may require adding or deleting inscriptions)
		//and calls refresh()
	void nextInscription(); //increments currentInscription.
		//navigation is over the set of InscriptionLabels ,plus the "append inscription" label.
	void prevInscription(); //decrements currentInscription
	void deleteInscription(); //deletes the current inscription (incl. graphs)
		//from surfTrans, followed by refresh()
	void insertInscription(); //inserts new empty inscription before current inscription
		//making appropriate changes to surfTrans, followed by refresh(). Note that if the inscription has a corresponding
		//image, it will still display but without any text. An "append inscription" label allows appending.
	void raiseInscription(); //raises the index of the current inscription, i.e. swaps index with next inscription.
		//note that the inscription does not carry its images with it. TODO Images have their own raise/lower function.
	void lowerInscription(); //lowers the index of the current inscription.

private:
	SurfaceTranscription* surfTrans;
	int currentInscription; //index of current inscription label (incl. append inscription label)
	QLabel inscrImgLabel; //hold img of current inscription if there is one.
	const SurfaceImgs* const surfImgs;
};

#endif

