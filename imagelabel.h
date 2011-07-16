#ifndef IMAGELABEL_H
#define IMAGELABEL_H

//class for image being viewed,
//and for manipulations performed on it

#include <QLabel>
#include <QWidget>
#include <QPoint>
#include <QTransform>
#include <QList>
#include <QString>
#include "bounding_box.h"

class QRubberBand;
class QSize;

class ImageLabel : public QLabel
{
	Q_OBJECT	//needed for signals and slots

public:
	ImageLabel(QWidget* parent, QList<BoundingBox>* pBoxList, const QImage* image); //constructor
	void refresh();
	void setMaxListLength(int max);

public slots:
	//the following are triggered by Viewer QActions.
	void zoomIn();
	void zoomOut();
	void zoomRestore();
	void rotateClockwise();
	void rotateAntiClockwise();
	void rotateRestore();
	void saveThumbnails();	//saves the contents of each bounding box to disk
	void toggleIndexNumbers();
	int getCurrentBoxIndex();
	void setCurrentBoxIndex(int index);
	void advanceCurrentBoxIndex();
	void reverseCurrentBoxIndex();
	void deleteCurrentBox();
	void lock(); //sets locked to true
	void unlock(); //sets locked to false
	bool isLocked(); //returns value of locked

protected:
	void paintEvent(QPaintEvent* event); //reimplement paintEvent(?)

private:
	//rubberband selection
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void transformImage();

	QPoint origin; //start of rubberband
	QRubberBand* rubberBand;
	
	const QImage* image; //pointer to Viewer::image, passed to constructor
	QImage transformedImage; //version of image that is displayed

	double zoom; //current magnification of image
	int rotation; //rotation of image in degrees clockwise
	QTransform transform; //matrix for performing zoom and rotation

	QList<BoundingBox>* pBoxList;
	bool indexNumbersVisible;
	int currentBoxIndex;
	int maxListLength; //maximum length of pBoxList: 1 for SURFACE, and 1000 otherwise.
	bool locked; //if locked, no rubber bands can be dragged or other changes made to box list
};

#endif
