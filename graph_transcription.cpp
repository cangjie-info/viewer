#include <QDebug>
#include "graph_transcription.h"

GraphTranscription::GraphTranscription(int graphMarkup, int graphemeId)
        : markup(graphMarkup), grapheme(graphemeId), canHaveImage(false)
{ }

int GraphTranscription::getMarkup() const
{
	return markup;
}

void GraphTranscription::setMarkup(int newMarkup)
{
	markup = newMarkup;
}

int GraphTranscription::getGrapheme() const
{
	return grapheme;
}

void GraphTranscription::setGrapheme(int newGrapheme)
{
	grapheme = newGrapheme;
}

void GraphTranscription::report() const
{
	qDebug() << "REPORT FOR GraphTranscription";
	qDebug() << "Markup = " << markup;
	qDebug() << "Grapheme = " << grapheme;
}

void GraphTranscription::setCanHaveImage(bool can)
{
	canHaveImage = can;
}
void GraphTranscription::toggleCanHaveImage()
{
    canHaveImage = !canHaveImage;
}

bool GraphTranscription::getCanHaveImage() const
{
	return canHaveImage;
}

