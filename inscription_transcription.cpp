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

QString InscriptionTranscription::getInscrString(int index) const
{
    QString inscrString;
    QString graphString;
    for(int i=0; i<count(); i++)
    {
        int grapheme = at(i).getGrapheme();
        if(grapheme == 0)
            //null transcription, use space~dot
            grapheme = 1001;
        graphString = QChar(57343 + at(i).getGrapheme());
        //offset 57343 = DFFF for Private Use Area
        if(index == i)
            markAsCurrent(graphString);
        inscrString += graphString;
    }
    if(index>=0)
    {
        graphString = QChar(57343 + 1001); //add append point for indexed transcription
        if(index==count())
            markAsCurrent(graphString);
        inscrString += graphString;
    }
    return inscrString;
}		

/* MARKUP POSSIBILITIES
background-color
color
background-image
text-decoration > none | [ underline || overline || line-through ]

*/

void InscriptionTranscription::markAsCurrent(QString& graphString) const
{
    //	graphString.prepend("<u>");

    graphString.prepend("<style type=\"text/css\">.selection {background-color: #FFAFAF;}</style><span class=\"selection\">");
    graphString.append("</span>");

    //	graphString.append("</u>");
}

void InscriptionTranscription::setCanHaveImage(bool can)
{
    canHaveImage = can;
}

bool InscriptionTranscription::getCanHaveImage() const
{
    return canHaveImage;
}
