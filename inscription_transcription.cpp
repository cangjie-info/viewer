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
void InscriptionTranscription::setCanHaveImage(bool can)
{
	canHaveImage = can;
}

bool InscriptionTranscription::getCanHaveImage() const
{
	return canHaveImage;
}
