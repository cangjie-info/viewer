#ifndef GRAPH_VIEWER_H
#define GRAPH_VIEWER_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
//#include <QVBoxLayout>
#include "flow_layout.h"

class GraphViewer : public QWidget
{
    Q_OBJECT
public:
    explicit GraphViewer(QWidget *parent = 0);

signals:

public slots:
private slots:
    void processInput();

private:
    QLineEdit* inputBox;
    FlowLayout* mainLayout;
    QList<QLabel*> labelList;
};

#endif // GRAPH_VIEWER_H
