#include "edit_transcription_dialog.h"
#include <QPixmap>
#include <QVBoxLayout>
#include <QKeyEvent>

EditTranscriptionDialog::EditTranscriptionDialog(QWidget* parent, InscriptionTranscription* const trans, QList<QImage>& imgList)
	: QDialog(parent), trans(trans), imgList(imgList)
{
	setFocusPolicy(Qt::StrongFocus);
	transCopy = *trans;

	QVBoxLayout* layout = new QVBoxLayout();
	transLabel = new QLabel(this);
	index = 3;
	refreshTrans();
	QFont font("HuaDong");
	font.setPixelSize(30);
	transLabel->setFont(font);
	transLabel->setAlignment(Qt::AlignLeft);
	layout->addWidget(transLabel);

	//standard buttons
	okButton = new QPushButton("OK", this);
	connect(okButton, SIGNAL(clicked()), this, SLOT(okClicked()));
	cancelButton = new QPushButton("Cancel", this);
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject())); //trans not updated

	QHBoxLayout* buttonLayout = new QHBoxLayout();
	buttonLayout->addWidget(cancelButton);
	buttonLayout->addWidget(okButton);

	layout->addLayout(buttonLayout);

	for(int i = 0; i<imgList.count(); i++)
	{
		QLabel* graphLabel = new QLabel(this);
		QPixmap pixmap;
		pixmap.convertFromImage(imgList.at(i));
		graphLabel->setPixmap(pixmap);
		layout->addWidget(graphLabel);
	}
	setLayout(layout);


	return;
}

void EditTranscriptionDialog::refreshTrans()
{
	transLabel->setText(trans->getInscrString(index));
}

void EditTranscriptionDialog::okClicked()
{
	*trans = transCopy;
	accept();
}

void EditTranscriptionDialog::keyPressEvent(QKeyEvent* event)
{
	switch(event->key())
	{
	case Qt::Key_Left:
		index--;
		break;
	case Qt::Key_Right:
		index++;
		break;
	}
	if(index<0)
		index = 0;
	else if(index >= imgList.count() && index > trans->count())
		index--;
	else refreshTrans();
}
