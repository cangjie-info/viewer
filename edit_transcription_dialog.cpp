#include "edit_transcription_dialog.h"
#include <QPixmap>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QDebug>
#include "db_handler.h"

EditTranscriptionDialog::EditTranscriptionDialog(
        QWidget* parent,
        InscriptionTranscription* const trans,
        QList<QImage>& imgList)
            : QDialog(parent), trans(trans), imgList(imgList)
{
    setFocusPolicy(Qt::StrongFocus); //so that the widget will capture key strokes
    transCopy = *trans; //make working copy, so that changes may be cancelled
    index = 0; //edit point at first graph or append point if there are no trans graphs

    createActions();
    createMenus();

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(dialogMenuBar);

    //graph label
    graphLabel = new QLabel();
    graphLabel->setFixedSize(100, 100);
    layout->addWidget(graphLabel);
    refreshGraph();

    //transcription label
    transLabel = new QLabel(this);
    QFont font("HuaDong");
    font.setPixelSize(30);
    transLabel->setFont(font);
    transLabel->setAlignment(Qt::AlignLeft);
    layout->addWidget(transLabel);
    refreshTrans(); //to set the transcription text and markup

    //text box
    inputBox = new QTextEdit();
    layout->addWidget(inputBox);
    inputBox->setFocusPolicy(Qt::NoFocus);
    inputBox->setText("I to begin.");
    connect(inputBox, SIGNAL(textChanged()), this, SLOT(processInput()));

    //standard buttons
    okButton = new QPushButton("OK\n(Ctrl+Enter)", this);
    connect(okButton, SIGNAL(clicked()), this, SLOT(okClicked())); //trans updated
    okButton->setFocusPolicy(Qt::NoFocus);
    cancelButton = new QPushButton("Cancel\n(Escape)", this);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject())); //trans not updated
    cancelButton->setFocusPolicy(Qt::NoFocus);
    //button layout
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(okButton);
    layout->addLayout(buttonLayout);

    setLayout(layout);
    return;
}

void EditTranscriptionDialog::createActions()
{

    graphLeftAction = new QAction("Edit point &left", this);
    graphLeftAction->setShortcut(Qt::Key_Left);
    connect(graphLeftAction, SIGNAL(triggered()), this, SLOT(graphLeft()));

    graphRightAction = new QAction("Edit point &right", this);
    graphRightAction->setShortcut(Qt::Key_Right);
    connect(graphRightAction, SIGNAL(triggered()), this, SLOT(graphRight()));

    cancelAction = new QAction("&Cancel - discard changes", this);
    cancelAction->setShortcut(Qt::Key_Escape);
    connect(cancelAction, SIGNAL(triggered()), this, SLOT(reject()));

    okAction = new QAction("&OK - save changes", this);
    okAction->setShortcut(Qt::Key_Return + Qt::ControlModifier);
    connect(okAction, SIGNAL(triggered()), this, SLOT(okClicked()));

    toggleNullAction = new QAction("&Toggle null image", this);
    toggleNullAction->setShortcut(Qt::Key_N);
    connect(toggleNullAction, SIGNAL(triggered()), this, SLOT(toggleNull()));

    clearAllNullsAction = new QAction("&Clear all null images", this);
    clearAllNullsAction->setShortcut(Qt::Key_N + Qt::ShiftModifier);
    connect(clearAllNullsAction, SIGNAL(triggered()), this, SLOT(clearAllNulls()));

    deleteGraphAction = new QAction("&Delete graph at edit point", this);
    deleteGraphAction->setShortcut(Qt::Key_Backspace);
    connect(deleteGraphAction, SIGNAL(triggered()), this, SLOT(deleteGraph()));

    insertGraphAction = new QAction("&Insert graph(s) at edit point", this);
    insertGraphAction->setShortcut(Qt::Key_I);
    connect(insertGraphAction, SIGNAL(triggered()), this, SLOT(insertGraph()));
}

void EditTranscriptionDialog::createMenus()
{
    dialogMenuBar = new QMenuBar(this);

    fileMenu = dialogMenuBar->addMenu("&File");
    fileMenu->addAction(okAction);
    fileMenu->addAction(cancelAction);

    editMenu = dialogMenuBar->addMenu("&Edit");
    editMenu->addAction(graphLeftAction);
    editMenu->addAction(graphRightAction);
    editMenu->addAction(toggleNullAction);
    editMenu->addAction(clearAllNullsAction);
    editMenu->addAction(deleteGraphAction);
    editMenu->addAction(insertGraphAction);
}

void EditTranscriptionDialog::graphLeft()
{
    index--;
    if(index<0)
        index = 0;
    else
    {
        refreshTrans();
        refreshGraph();
    }
}

void EditTranscriptionDialog::graphRight()
{
    index++;
    if(index > transCopy.count()) //remember the append point
        index=transCopy.count();
    else
    {
        refreshTrans();
        refreshGraph();
    }
}

void EditTranscriptionDialog::toggleNull()
{
    if(index == transCopy.count())
    {
        //do nothing - at append point
    }
    else
    {
        transCopy[index].toggleCanHaveImage();
        refreshGraph();
    }
}

void EditTranscriptionDialog::clearAllNulls()
{
    for(int i=0; i<transCopy.count(); i++)
        transCopy[i].setCanHaveImage(true);
    refreshGraph();
}

void EditTranscriptionDialog::deleteGraph()
{
    if(!(index == transCopy.count())) //not the append point
    {
        transCopy.removeAt(index);
        if(index != 0)
            index--;
        refreshGraph();
        refreshTrans();
    }
}

void EditTranscriptionDialog::insertGraph()
{
    //focus to insert box
    //insert box keeps focus until <Return>
    setFocusPolicy(Qt::NoFocus);
    inputBox->setFocus();
    inputBox->setText("");
}

void EditTranscriptionDialog::refreshTrans()
{
    transLabel->setText(transCopy.getInscrString(index));
}

void EditTranscriptionDialog::refreshGraph()
{
    //account for null images before index position
    int imgIndex = 0;
    for(int i = 0; i < index; i++)
    {
        if(transCopy[i].getCanHaveImage() == true)
            imgIndex++;
    }
    //four possibilities, either
    if (index == transCopy.count()) //we are at the append point
        graphLabel->setText("NULL");
    else if(transCopy[index].getCanHaveImage() == false)  //"NULL" image
        //clear pixmap and set text to "NULL"
        graphLabel->setText("NULL");
    //or
    else if(imgIndex >= imgList.count())	//no more graph images
        //clear pixmap and set text to ""
        graphLabel->setText("");
    //or
    else
    {
        //clear text and set pixmap to graph image
        QPixmap pixmap;
        pixmap.convertFromImage(imgList.at(imgIndex));
        graphLabel->setPixmap(pixmap);
    }
}

void EditTranscriptionDialog::okClicked()
{
    *trans = transCopy;
    done(1);
    qDebug() << "okClicked()";
}

void EditTranscriptionDialog::processInput()
{
    QString inputString = inputBox->toPlainText();
    if(inputString.endsWith("\n"))
    {
        inputString = inputString.trimmed(); //removes white space incl. \n from each end
        //do work
        int grapheme = DbHandler::getGrapheme(inputString);
        if(grapheme > 0)
        {
            transCopy.insert(index, GraphTranscription(0, grapheme));
            transCopy[index].setCanHaveImage(true);
            index++;
            refreshGraph();
            refreshTrans();
        }
        inputBox->setText("I to begin.");
        inputBox->setFocusPolicy(Qt::NoFocus);
        this->setFocusPolicy(Qt::StrongFocus);
        this->setFocus();
    }
    else
    {
        //keep going
    }
}
