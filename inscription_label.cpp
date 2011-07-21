#include "inscription_label.h"
#include <QFont>

InscriptionLabel::InscriptionLabel(QWidget* parent)
	: QLabel(parent)
{
	QFont font("HuaDong");
	font.setPixelSize(30);
	setFont(font);
}

