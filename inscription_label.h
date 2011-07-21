#ifndef INSCRIPTION_LABEL_H
#define INSCRIPTION_LABEL_H

//class to display transcription of an inscription
//inherits QLabel

#include <QLabel>

class InscriptionLabel : public QLabel
{
public:
		InscriptionLabel(QWidget* parent); //constructor, sets font and other features
private:
};
#endif
