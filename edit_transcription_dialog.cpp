#include "edit_transcription_dialog.h"
#include <QPixmap>
#include <QVBoxLayout>

EditTranscriptionDialog::EditTranscriptionDialog(QWidget* parent, InscriptionTranscription* const trans, QList<QImage>& imgList)
	: QDialog(parent)
{
	QVBoxLayout* layout = new QVBoxLayout();
	transLabel = new QLabel(this);
	transLabel->setText(trans->getInscrString());
	QFont font("HuaDong");
	font.setPixelSize(30);
	transLabel->setFont(font);
	transLabel->setAlignment(Qt::AlignLeft);
	layout->addWidget(transLabel);
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

