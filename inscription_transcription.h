#ifndef INSCRIPTION_TRANSCRIPTION_H
#define INSCRIPTION_TRANSCRIPTION_H

#include <QString>
#include "graph_transcription.h"
//class representing the transcription of a single inscription

class InscriptionTranscription : public QList<GraphTranscription>
{
public:
	InscriptionTranscription(); //constructor
	QString getInscrString() const; //returns transcribed inscription
			//as a QString, unicode encoded as per HuaDong signlist.
			//TODO with punctuation and other markup
	void report() const;
	void setCanHaveImage(bool can);
	bool getCanHaveImage() const;
private:
	bool canHaveImage; //used to mark inscriptions in the transcription 
		//that have no corresponding bounding box in the surface image
		//TODO remove nullBox apparatus from imageLabel.
	//TODO int markup;
	//QString notes;
};

#endif