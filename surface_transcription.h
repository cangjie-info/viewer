#ifndef SURFACE_TRANSCRIPTION_H
#define SURFACE_TRANSCRIPTION_H
#include "inscription_transcription.h"

//holds trancriptions and markup for one surface

class SurfaceTranscription : public QList<InscriptionTranscription>
{
public:
	SurfaceTranscription(); //constructor
	QString getPubId() const;
	void setPubId(QString newPubId);
	QString getPubNumber() const;
	void setPubNumber(QString newPubNumber);
	QString getSurfaceType() const;
	void setSurfaceType(QString newSurfaceType);
	void report() const;
private:
	QString publicationId; //ec.surfaces.publicationId
	QString pubNumber; //ec.surfaces.pubNumber
	QString surfaceType; //ec.surfaces
};
#endif
