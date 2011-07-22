#include <QDebug>
#include "inscription_transcription.h"
#include "graph_transcription.h"

InscriptionTranscription::InscriptionTranscription()
{
	//constructor
}

void InscriptionTranscription::report() const
{
	qDebug() << "REPORT FOR InscriptionTranscription";
	qDebug() << "Number of graphs =" << count();
	for(int i=0; i<count(); i++)
		at(i).report();
	qDebug() << "END REPORT FOR InscriptionTranscription";
}

QString InscriptionTranscription::getInscrString() const
{
	QString inscrString;
	for(int i=0; i<count(); i++)
	{
		if(at(i).getGrapheme() == 0)
		{ 
			/*null transcription, do nothing*/
		} else { //add grapheme to transcription
			inscrString += QChar(57343 + at(i).getGrapheme()); 
				//offset 57343 = DFFF for Private Use Area
		}
	}
	return inscrString;
}		

void InscriptionTranscription::setCanHaveImage(bool can)
{
	canHaveImage = can;
}

bool InscriptionTranscription::getCanHaveImage() const
{
	return canHaveImage;
}
