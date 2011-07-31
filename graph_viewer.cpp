#include <QVBoxLayout>
#include <QPushButton>
#include <QString>
#include <QList>
#include <QLabel>
#include "graph_viewer.h"
#include "db_handler.h"

GraphViewer::GraphViewer(QWidget *parent) :
        QWidget(parent)
{
    mainLayout = new FlowLayout;
    inputBox = new QLineEdit;
    mainLayout->addWidget(inputBox);
    connect(inputBox, SIGNAL(returnPressed()), this, SLOT(processInput()));
    setLayout(mainLayout);
}

void GraphViewer::processInput()
{
    //remove all labels in list
    foreach(QLabel* label, labelList) delete label;
    labelList.clear();

    //repopulate
    QString inputString = inputBox->text().trimmed();
    int grapheme = DbHandler::getGrapheme(inputString);
    QList<int> graphList;
    DbHandler::findGraphemeInstances(graphList, grapheme);
    while(!graphList.isEmpty())
    {
        int graphId = graphList.takeFirst();
        QImage graphImage;
        DbHandler::getGraphImage(graphImage, graphId, 100);
        if(!graphImage.isNull())
        {
            QPixmap pixmap;
            pixmap.convertFromImage(graphImage);
            QLabel* label = new QLabel(this);
            label->setPixmap(pixmap);
            mainLayout->addWidget(label);
            label->show();
            labelList << label;
        }
    }
  resize(mainLayout->minimumSize());
}
