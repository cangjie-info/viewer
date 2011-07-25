#include "inscription_widget.h"
#include <QFont>
#include <QHBoxLayout>
#include <QLabel>

InscriptionWidget::InscriptionWidget(QWidget* parent, QString trans, int transNumber, int imgNumber, bool isCurrent)
	: QWidget(parent)
{
	//set name
	setObjectName("IW");
	//HBoxLayout
	QHBoxLayout* layout = new QHBoxLayout();
	//numberLabel
	QLabel* numLabel = new QLabel();
	QString numLabelText = QString("%1 / ").arg(transNumber+1); //displayed index base = 1, not 0
	if(imgNumber == -2)	//hack to represent canHaveImage == false. 
		numLabelText += "no image";
	else if(imgNumber == -1) //hack to represent no image supplied
		{ 
			//do nothing - blank imgNumber
		}
	else
		numLabelText += QString("%1").arg(imgNumber+1); //displayed index base = 1, not 0
	numLabel->setText(numLabelText);
	numLabel->setMaximumWidth(100);
	numLabel->setMinimumWidth(100);
	//add to layout
	layout->addWidget(numLabel);
	
	//transLabel
	QLabel* transLabel = new QLabel(trans);
	QFont font("HuaDong");
	font.setPixelSize(30);
	transLabel->setFont(font);
	transLabel->setAlignment(Qt::AlignLeft);
//TODO if isCurrent, do something to make the widget look different
	if(isCurrent)
		setStyleSheet("background-color: white");
	//add to layout
	layout->addWidget(transLabel);

	setLayout(layout);
}

void InscriptionWidget::setCurrent(bool isCurrent)
{
	if(isCurrent)
		setStyleSheet("background-color: white");
}
