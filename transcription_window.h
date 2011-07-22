#ifndef TRANSCRIPTION_WINDOW_H
#define TRANSCRIPTION_WINDOW_H

//class to display and edit set of transcriptions 
//for a single surface
//has read/write access to SurfaceTranscription instance 
//passed by const pointer from Viewer.
//TODO has read-only access to SurfaceImages instance passed
//by const pointer to const from Viewer.

//instance constructed by Viewer::advance() or back(), destroying 
//previous instance, in a dock window created by Viewer::Viewer() constructor.
//TranscriptionWindow constructor call refresh() which creates list of 
//Inscription labels. Changes to surfTrans require new call to refresh()

//TODO TranscriptionWindow can be switched on and off from Viewer, to reduce
//overhead, especially when drawing new inscription bboxes.

#include <QWidget>
#include <QLabel>
#include <QList>
#include <QVBoxLayout>
#include "inscription_widget.h"
#include "surface_transcription.h"
#include "surface_imgs.h"

class TranscriptionWindow : public QWidget
{
	Q_OBJECT
public:
	TranscriptionWindow(SurfaceTranscription* const trans, const SurfaceImgs* const imgs); //constructor
		//creates surfaceLabel and calls refresh()
//NO. PUT IN VIEWER.	void editCurrentInscription(); //creates new pop-up dialog to edit the current inscription
	void toggleCanHaveImg();	//sets the current inscription's canHaveImage, 
		//modifies surfTrans (may require adding or deleting inscriptions)
		//and calls refresh()
	void nextInscription(); //increments currentInscription.
		//navigation is over the set of InscriptionLabels ,plus the "append inscription" label.
	void prevInscription(); //decrements currentInscription
	void deleteInscription(); //deletes the current inscription (incl. graphs)
		//from surfTrans, followed by refresh()
		//NB the inscription will be restored by refresh() if there is a corresponding trans
	void insertInscription(); //inserts new empty inscription before current inscription
		//making appropriate changes to surfTrans, followed by refresh(). Note that if the inscription has a corresponding
		//image, it will still display but without any text. An "append inscription" label allows appending.
	void raiseInscription(); //raises the index of the current inscription, i.e. swaps index with next-higher inscription.
		//note that the inscription does not carry its images with it. TODO Images have their own raise/lower function.
		//does nothing is currentInscription points to insertion-point "append new inscription", 
		//or to the inscription with the highest index
		//currentInscription then points to the raised Inscription.
	void lowerInscription(); //lowers the index of the current inscription.

public slots:
	void refresh(); //implementation as a slot allows refresh() 
		//to be called in reponse to relevant events
		//like the insewrtion/deletion of an inscription bbox by ImageLabel. 
		//generates InscriptionLabels and adds them as children
		//also generates a set of QLabels for numbering the InscriptionLabels, and 
		//indicating whether they have/may have corresponding images or not.
		//number of Inscription labels = max of number of InscriptionImgs and InscriptionTranscriptions
		//also adds an "append inscription" label.
		//refresh() should be called as a slot by a signal emitted by ImageLabel::mouseUp. (i.e. on creation
		//of new image bbox. Also called on ImageLabel::deleteBox.
private:
	QVBoxLayout* layout; //top-level layout for widget
	SurfaceTranscription* surfTrans;
	int currentInscription; //index of current inscription label (incl. append inscription label)
	QLabel inscrImgLabel; //hold img of current inscription if there is one.
	const SurfaceImgs* const surfImgs;
};

#endif

