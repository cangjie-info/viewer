#include "transcription_window.h"
#include <QString>
#include <QWidget>
#include <QChar>
#include <QVBoxLayout>
#include <QDebug>

TranscriptionWindow::TranscriptionWindow(SurfaceTranscription* trans, const SurfaceImgs* const imgs)
	: surfTrans(trans), surfImgs(imgs), layout(NULL)
{
/* DELETE
	//create surface label
	QLabel* surfaceLabel = new QLabel(this);
	QString surfaceLabelText = QString("surface = %1\nsurface type = %2\nnumber of inscriptions = %3")
		.arg(surfTrans->getPubId() + surfTrans->getPubNumber())
		.arg(surfTrans->getSurfaceType())
		.arg(surfTrans->count());
	//style label
	surfaceLabel->setText(surfaceLabelText);
	surfaceLabel->setMargin(10);
//	surfaceLabel->setFrameStyle(QFrame::StyledPanel);
//	surfaceLabel->setFrameShadow(QFrame::Raised);
	//make scroll area to hold everything in
	//add to layout
	layout = new QVBoxLayout();
	setLayout(layout);
*/	
	//TODO ? create QLabel to hold image of current inscription
	//generate InscriptionLabels, etc. reflecting current state of surfTrans
	refresh();
}

void TranscriptionWindow::refresh()
{
	//remove all old InscriptionWidgets
	QList<InscriptionWidget*> oldList = this->findChildren<InscriptionWidget*>("IW");
	while(!oldList.isEmpty())
		delete oldList.takeFirst();
	currentInscription = 0;
	//trash old layout, and make new one
	if(layout)
		delete layout;
	layout = new QVBoxLayout();
	setLayout(layout);
	//generate InscriptionWidgets to accommodate surfTrans, and surfImgs
	//imgsIndex = 0; transIndex = 0;
	int imgsIndex = 0; //index to QList of InscriptionImgs in surfImgs
	int transIndex = 0; //index to QList of InscriptionTranscriptions in surfTrans
	while(imgsIndex < surfImgs->inscriptionCount() || transIndex < surfTrans->count())
		//while there are still imgs or trans to be accommodated
	{
		if(transIndex >= surfTrans->count()) //trans all accommodated, but still some imgs
		{
			//append new inscription to surfTrans
			InscriptionTranscription newTrans;
			newTrans.setCanHaveImage(true); //make sure that this can be associated with an image
         surfTrans->append(newTrans);
			//transIndex is no longer oor.
		}
		//transIndex in range
		//new InscriptionWidget
		QString trans = (*surfTrans)[transIndex].getInscrString();
		int imgNumber = imgsIndex; //image serial number for InscriptionWidget
		if(imgsIndex >= surfImgs->inscriptionCount())
			imgNumber = -1; //for blank=""
		if(surfTrans->at(transIndex).getCanHaveImage() == false)
			imgNumber = -2; //for "no image"
		InscriptionWidget* inscrWidget = new 
					InscriptionWidget(this, trans, transIndex, imgNumber, false);
		if(transIndex == currentInscription)
			inscrWidget->setCurrent(true);
			//TODO get corresponding inscription img and setPixmap on image QLabel
		if(imgNumber != -2) //if we have accommodated an image (or already run out of imgs)
			imgsIndex++;
		layout->addWidget(inscrWidget);
		transIndex++;
	}

	//add "append inscription" label
	InscriptionWidget* appendInscrWidget = new 
				InscriptionWidget(this, "append new", surfTrans->count(), -1, false);
	if(currentInscription == surfTrans->count())
		appendInscrWidget->setCurrent(true);
	layout->addWidget(appendInscrWidget);
}

void TranscriptionWindow::toggleCanHaveImg()
{
	if(currentInscription < 0 || currentInscription >= surfTrans->count())
	{
		//do nothing - oor 
	}
	else
		(*surfTrans)[currentInscription].setCanHaveImage(
				!(*surfTrans)[currentInscription].getCanHaveImage());
	refresh(); 	//unavoidable, despite the overhead, since the matching of 
					//imgs with trans may have changed.
}

void TranscriptionWindow::nextInscription()
{
	//get list of inscription widgets
	QList<InscriptionWidget*> inscriptionWidgetList = this->findChildren<InscriptionWidget*>();
	//de-current current
	inscriptionWidgetList[currentInscription]->setCurrent(false);
	//increment current
	currentInscription++;
	if(currentInscription > surfTrans->count()) 	//NB current inscription can correspond to the 
		currentInscription = 0; 					 	//"append new" insertion-point label
	//mark new current widget as current
	inscriptionWidgetList[currentInscription]->setCurrent(true);
	update();
}

void TranscriptionWindow::prevInscription()
{
	QList<InscriptionWidget*> inscriptionWidgetList = this->findChildren<InscriptionWidget*>();
	inscriptionWidgetList[currentInscription]->setCurrent(false);
	currentInscription--;
	if(currentInscription < 0)
		currentInscription = surfTrans->count(); //NB this corresponds to the "append new"
						//insertion-point label.
	//mark new current widget as current
	inscriptionWidgetList[currentInscription]->setCurrent(true);
	update();
}

void TranscriptionWindow::deleteInscription()
{
	if(currentInscription < 0 || currentInscription >= surfTrans->count())
		return;
	surfTrans->removeAt(currentInscription);
	//NB currentInscription is still in range
	refresh();
}

void TranscriptionWindow::insertInscription()
{
	if(currentInscription < 0 || currentInscription > surfTrans->count())
		return; //NB can insert at "append new" insertion-point
	surfTrans->insert(currentInscription, InscriptionTranscription());
	//currentInscription is still in range, and points to the newly inserted 
	//inscription
	refresh();
}

void TranscriptionWindow::raiseInscription()
{
	if(currentInscription < 0 || currentInscription > surfTrans->count() - 1)
		return;
	surfTrans->swap(currentInscription, ++currentInscription);
	refresh();
}

void TranscriptionWindow::lowerInscription()
{
	if(currentInscription < 1 || currentInscription > surfTrans->count())
		return;
	surfTrans->swap(currentInscription, --currentInscription);
	refresh();
}
