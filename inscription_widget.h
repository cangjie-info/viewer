#ifndef INSCRIPTION_WIDGET_H
#define INSCRIPTION_WIDGET_H

//class to display transcription of an inscription
//consists of two QLabels numLabel, and transLabel
//the first displays the serial number of the transcription
//followed by either 1/ the serial number of the associated image, 
//or 2/ a blank (indicating that an image has not yet been provided), 
//or 3/ "no image", idicating that canHanveImage is false for the related 
//InscriptionTranscription. FORMAT = transNumber / imgNumber
//transLabel contains the text of the transcription.
//The QLabels will be styled differently depending on whether this represents 
//the current inscription or not.

#include <QWidget>
#include <QString>

class InscriptionWidget : public QWidget
{
public:
		InscriptionWidget(QWidget* parent, QString trans, int transNumber, int imgNumber, bool isCurrent); //constructor, sets font and other features
		void setCurrent(bool isCurrent); //changes appearance of label to indicate 
			//current inscription or not.
private:
};
#endif
