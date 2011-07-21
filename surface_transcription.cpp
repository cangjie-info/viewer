#include <QDebug>
#include "surface_transcription.h"
#include "inscription_transcription.h"

SurfaceTranscription::SurfaceTranscription()
	: publicationId(""), pubNumber(""), surfaceType("")
{
	//constructor
}

QString SurfaceTranscription::getPubId() const
{
	return publicationId;
}

void SurfaceTranscription::setPubId(QString newPubId)
{
	publicationId = newPubId;
}

QString SurfaceTranscription::getPubNumber() const
{
	return pubNumber;
}

void SurfaceTranscription::setPubNumber(QString newPubNumber)
{
	pubNumber = newPubNumber;
}


QString SurfaceTranscription::getSurfaceType() const
{
	return surfaceType;
}

void SurfaceTranscription::setSurfaceType(QString newSurfaceType)
{
	surfaceType = newSurfaceType;
}

void SurfaceTranscription::report() const
{
	qDebug() << "REPORT FOR SurfaceTranscription";
	qDebug() << "publicationId:" << publicationId;
	qDebug() << "pubNumber:" << pubNumber;
	qDebug() << "surfaceType:" << surfaceType;
	qDebug() << "Number of inscriptions =" << count();
	for(int i=0; i<count(); i++)
		at(i).report();
	qDebug() << "END OF REPORT FOR SurfaceTranscriptions.";
}
