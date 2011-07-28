#include "transcription_window.h"
#include <QString>
#include <QWidget>
#include <QChar>
#include <QVBoxLayout>
#include <QDebug>

TranscriptionWindow::TranscriptionWindow(SurfaceTranscription* trans, const SurfaceImgs* const imgs)
	: surfTrans(trans), surfImgs(imgs), layout(NULL)
{
	layout = new QVBoxLayout();
	setLayout(layout);
	locked = true;
	currentInscription = 0;
	//generate InscriptionLabels, etc. reflecting current state of surfTrans
	refresh();
}

int TranscriptionWindow::getCurrentTranscriptionIndex() const
{
	return currentInscription;
}

int TranscriptionWindow::getCurrentImageIndex() const
{
	QList<InscriptionWidget*> oldList = this->findChildren<InscriptionWidget*>("IW");
	return oldList.at(currentInscription)->getImageIndex();
}

void TranscriptionWindow::refresh()
{
	//remove all old InscriptionWidgets
	QList<InscriptionWidget*> oldList = this->findChildren<InscriptionWidget*>("IW");
	while(!oldList.isEmpty())
		delete oldList.takeFirst();

	int windowHeight = 0; //sum the heights of the inscription labels, and use to set the height of the widget

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
		{
			inscrWidget->setCurrent(true);
		}
			//TODO get corresponding inscription img and setPixmap on image QLabel
		if(imgNumber != -2) //if we have accommodated an image (or already run out of imgs)
			imgsIndex++;

//This is a hack required to get the layout manager to behave as I want.
//TODO find orthodox solution to this.
windowHeight += inscrWidget->sizeHint().height();
inscrWidget->setMaximumHeight(inscrWidget->sizeHint().height());

		layout->addWidget(inscrWidget);
		transIndex++;
	}

	//add "append inscription" label
	InscriptionWidget* appendInscrWidget = new 
				InscriptionWidget(this, "append new", surfTrans->count(), -1, false);
	if(currentInscription == surfTrans->count())
	{
		appendInscrWidget->setCurrent(true);
	}
	windowHeight += appendInscrWidget->sizeHint().height();
	layout->addWidget(appendInscrWidget);
	
//This is a hack, to get the layout manager to behave appropriately
//TODO find proper solution.
	setMinimumHeight(windowHeight);
}

void TranscriptionWindow::toggleCanHaveImage()
{
	if(locked || currentInscription < 0 || currentInscription >= surfTrans->count())
	{
		//do nothing - oor or locked
	}
	else
	{
		(*surfTrans)[currentInscription].setCanHaveImage(
				!(*surfTrans)[currentInscription].getCanHaveImage());
		emit inscrListModified();
		refresh(); 	//unavoidable, despite the overhead, since the matching of 
					//imgs with trans may have changed.
	}
}

void TranscriptionWindow::allCanHaveImage() //sets can have image to TRUE for all inscriptions.
{
	if(locked)
		return;
	for(int index = 0; index < surfTrans->count(); index++)
		(*surfTrans)[index].setCanHaveImage(true);
	emit inscrListModified();
	refresh();
}

void TranscriptionWindow::unlock()
{
	locked = false;
}

void TranscriptionWindow::nextInscription()
{
	//get list of inscription widgets
	QList<InscriptionWidget*> inscriptionWidgetList = this->findChildren<InscriptionWidget*>("IW");
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
	QList<InscriptionWidget*> inscriptionWidgetList = this->findChildren<InscriptionWidget*>("IW");
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
	if(locked || currentInscription < 0 || currentInscription >= surfTrans->count())
		return;
	surfTrans->removeAt(currentInscription);
	//NB currentInscription is still in range
	emit inscrListModified();
	refresh();
}

void TranscriptionWindow::insertInscription()
{
	if(locked || currentInscription < 0 || currentInscription > surfTrans->count())
		return; //NB can insert at "append new" insertion-point
	surfTrans->insert(currentInscription, InscriptionTranscription());
	//currentInscription is still in range, and points to the newly inserted 
	//inscription
	emit inscrListModified();
	refresh();
}

void TranscriptionWindow::raiseInscription()
{
	if(locked || currentInscription < 0 || currentInscription >= surfTrans->count() - 1)
		return;
	surfTrans->swap(currentInscription, currentInscription+1);
	currentInscription++;
	emit inscrListModified();
	refresh();
}

void TranscriptionWindow::lowerInscription()
{
	if(locked || currentInscription < 1 || currentInscription > surfTrans->count())
		return;
	surfTrans->swap(currentInscription, currentInscription-1);
	currentInscription--;
	emit inscrListModified();
	refresh();
}

void TranscriptionWindow::copyTrans()
{
    if(locked || currentInscription < 0 || currentInscription > surfTrans->count())
        return;
    surfTrans->insert(currentInscription, surfTrans->at(currentInscription));
    emit inscrListModified();
    refresh();
}
