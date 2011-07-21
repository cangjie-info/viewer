#include "transcription_window.h"
#include <QString>
#include <QWidget>
#include <QChar>
#include <QVBoxLayout>

TranscriptionWindow::TranscriptionWindow(SurfaceTranscription* trans)
	: surfTrans(trans)
{
	//create surface label
	QLabel surfaceLabel = new QLabel(this);
	QString surfaceLabelText = QString("surface = %1: surface type = %2, number of inscriptions = %3")
		.arg(surfTrans->getPubId() + surfTrans->getPubNumber())
		.arg(surfTrans->getSurfaceType())
		.arg(surfTrans->count());
	//style label
	surfaceLabel->setText(surfaceLabelText);
	surfaceLabel->setMargin(10);
	surfaceLabel->setFrameStyle(QFrame::StyledPanel);
	surfaceLabel->setFrameShadow(QFrame::Raised);
	//add to layout
	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(surfaceLabel);
	
	//create QLabel to hold image of current inscription
	//add to layout
	inscrImgLabel* = new QLabel(this);
	layout->addWidget(inscrImgLabel);

	//generate InscriptionLabels, etc. reflecting current state of surfTrans
	refresh();
}

void TranscriptionWindow::refresh()
{
	//retrieve layout previously created
	QVBoxLayout* layout = this->findChild<QVBoxLayout*>;

	//imgsIndex = 0; transIndex = 0;
	int imgsIndex = 0;
	int tranIndex = 0;
	while(imgsIndex < 
	//while (imgsIndex in range || transIndex in range)
	//if: transIndex oor
		//append new inscription to surfTrans
		//transIndex is no longer oor.
	//transIndex in range
	//new HBoxLayout
	//new QLabel (A:B)
		//where A is transIndex, B is imgsIndex/"no image"/blank
	//if inscription canHaveImage
		//if imgsIndex oor
			//set B to blank
		//else set B to imgsIndex
			//get corresponding inscription img
			//setPixmap on image QLabel
				//resize if necessary
			//imgIndex++
	//else set B to "no image"
	//new InscriptionLabel, with inscription from surfTrans
		//if: currentInscription == transIndex - mark label appropriately
	//add both labels to HBoxLayout
	//add HBoxLayout to VBoxLayout.
	//transIndex++;
	//LOOP
	//add "append inscription" label
	//add to VBoxLayout.
}
	



	for(int index = 0; index < surfTrans->count(); index++)
	{
		InscriptionLabel* newLabel = new InscriptionLabel(this);
		inscrLabelList.append(newLabel);
		QString inscrLabelText = QString("Inscription %1: ")
			.arg(index);
		for(int graphIndex = 0; graphIndex < surfTrans->at(index).count(); graphIndex++)
		{
			int grapheme = surfTrans->at(index)[graphIndex].getGrapheme();
			int codePoint = 57343 + grapheme;
			if(graphIndex==3)
				inscrLabelText += QString("<span style='background-color:red'>");
			inscrLabelText += QString("%1").arg(QChar(codePoint));
			if(graphIndex==3)
				inscrLabelText += QString("</span>");
		}
		inscrLabelList.last()->setText(inscrLabelText);
		if(index==1)
			inscrLabelList.last()->setStyleSheet(QString("background: green"));
		layout->addWidget(inscrLabelList.last());
	}

	setLayout(layout);
}
