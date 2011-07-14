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
private:
	int markup;
	int grapheme;
};
#endif
