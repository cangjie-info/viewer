#include "inscription_widget.h"
#include <QFont>
#include <QHBoxLayout>
#include <QLabel>

InscriptionWidget::InscriptionWidget(QWidget* parent, QString trans, int transNumber, int imgNumber, bool isCurrent)
	: QWidget(parent)
{
	//HBoxLayout
	QHBoxLayout* layout = new QHBoxLayout();
	//numberLabel
	QLabel* numLabel = new QLabel();
	QString numLabelText = QString("%1 / ").arg(transNumber);
	if(imgNumber == -2)	//hack to represent canHaveImage == false. 
		numLabelText += "no image";
	else if(imgNumber == -1) //hack to represent no image supplied
		{ 
			//do nothing - blank imgNumber
		}
	else
		numLabelText += QString("%1").arg(imgNumber);
	numLabel->setText(numLabelText);
	numLabel->setMinimumWidth(80); //300 pixels wide - see how it looks and edit accordingly
	//add to layout
	layout->addWidget(numLabel);
	
	//transLabel
	QLabel* transLabel = new QLabel(trans);
	QFont font("HuaDong");
	font.setPixelSize(30);
	transLabel->setFont(font);
//TODO if isCurrent, do something to make the widget look different
	if(isCurrent)
		setStyleSheet("background-color: white");
	else
		setStyleSheet("background-color: grey");
	//add to layout
	layout->addWidget(transLabel);

	setLayout(layout);
}

void InscriptionWidget::setCurrent(bool isCurrent)
{
	if(isCurrent)
		setStyleSheet("background-color: white");
	else
		setStyleSheet("background-color: grey");
}
