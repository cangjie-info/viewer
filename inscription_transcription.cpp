#include <QDebug>
#include "inscription_transcription.h"
#include "graph_transcription.h"

InscriptionTranscription::InscriptionTranscription()
    : canHaveImage(false)
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

QString InscriptionTranscription::getInscrString(int index, int markupToggles) const
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

        int markup = at(i).getMarkup() & markupToggles;

        //mark graph uncertain
        if(markup & GraphTranscription::GRAPH_UNCERTAIN)
            markAsUncertain(graphString);

        //mark graph as crack number
        if(markup & GraphTranscription::CRACK_NUMBER)
            markAsCrackNumber(graphString);

            //mark as hewen or chongwen
        if(markup & (GraphTranscription::HEWEN_LEFT | GraphTranscription::HEWEN_RIGHT
                     | GraphTranscription::CHONGWEN_LEFT | GraphTranscription::CHONGWEN_RIGHT))
            markAsHewen(graphString);

        //mark unusual form
        if(markup & GraphTranscription::FORM_UNUSUAL)
            markAsUnusual(graphString);

        //mark current
        if(index == i)
            markAsCurrent(graphString);

        //make as editorial restoration
        if(markup & GraphTranscription::EDS_RESTORATION)
            markAsEdsRestoration(graphString, i);

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

void InscriptionTranscription::markAsUncertain(QString& graphString) const
{
    graphString.prepend("<style type=\"text/css\">.uncertain {color: #FF0000;}</style><span class=\"uncertain\">");
    graphString.append("</span>");
}

void InscriptionTranscription::markAsCrackNumber(QString& graphString) const
{
    graphString.prepend("<style type=\"text/css\">.crack {background-color: #FFFF00;}</style><span class=\"crack\">");
    graphString.append("</span>");
}

void InscriptionTranscription::markAsEdsRestoration(QString& graphString, int index) const
{
    static bool begun = false;
    if(!begun) //prepend "["
    {
        graphString.prepend("[");
        begun = true;
    }
    if (index == count() - 1 || ((at(index+1).getMarkup() & GraphTranscription::EDS_RESTORATION) == false))
    {
        graphString.append("]");
        begun = false;
    }
}

void InscriptionTranscription::markAsHewen(QString& graphString) const
{
    graphString.prepend("<style type=\"text/css\">.hewen {text-decoration: underline;}</style><span class=\"hewen\">");
    graphString.append("</span>");
}

void InscriptionTranscription::markAsUnusual(QString& graphString) const
{
    graphString.prepend("<style type=\"text/css\">.unusual {text-decoration: overline;}</style><span class=\"unusual\">");
    graphString.append("</span>");
}

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
