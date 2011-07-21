#ifndef GRAPH_TRANSCRIPTION_H 
#define GRAPH_TRANSCRIPTION_H

//class representing  ec.inscriptionGraphs.markup and .graphemeId,
//i.e. the transcription of one graph in an inscription

class GraphTranscription
{
public:
	GraphTranscription(int graphMarkup, int graphemeId); //constructor
	int getMarkup() const;
	void setMarkup(int newMarkup);
	int getGrapheme() const;
	void setGrapheme(int newGrapheme);
	void report() const;
	void setCanHaveImage(bool can); //set canHanveImage member variable
	bool getCanHaveImage() const; //get canHaveImage member variable
private:
	int markup; //from ec
	int grapheme; //from ec
	bool canHaveImage; //used to mark graphs or other inline elements 
		//in the transcription that have no corresponding bounding box
		//in the surface image
		//TODO remove nullBox apparatus from imageLabel.
};
#endif
