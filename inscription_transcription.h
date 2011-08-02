#ifndef INSCRIPTION_TRANSCRIPTION_H
#define INSCRIPTION_TRANSCRIPTION_H

#include <QString>
#include "graph_transcription.h"
//class representing the transcription of a single inscription

class InscriptionTranscription : public QList<GraphTranscription>
{
public:
	InscriptionTranscription(); //constructor
        QString getInscrString(int index = -1, int markupToggles = 0) const;
                        //returns transcribed inscription
			//as a QString, unicode encoded as per HuaDong signlist.
			//index position marked
			//TODO with punctuation and other markup
	void report() const;
	void setCanHaveImage(bool can);
	bool getCanHaveImage() const;
private:
	void markAsCurrent(QString& graphString) const; //marks graph as current
	bool canHaveImage; //used to mark inscriptions in the transcription 
		//that have no corresponding bounding box in the surface image
		//TODO remove nullBox apparatus from imageLabel.
        void markAsUncertain(QString& graphString) const;
        void markAsCrackNumber(QString& graphString) const;
        void markAsEdsRestoration(QString& graphString, int i) const;
        void markAsHewen(QString& graphString) const;
        void markAsUnusual(QString& graphString) const;
        //TODO int markup;
	//QString notes;
};

#endif
