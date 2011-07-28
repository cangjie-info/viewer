#ifndef IMAGE_LABEL_H
#define IMAGE_LABEL_H

//class for image being viewed,
//and for manipulations performed on it

#include <QLabel>
#include <QWidget>
#include <QPoint>
#include <QTransform>
#include <QList>
#include <QString>
#include "bounding_box.h"
#include "surface_imgs.h"

class QRubberBand;
class QSize;

class ImageLabel : public QLabel
{
	Q_OBJECT	//needed for signals and slots

public:
	ImageLabel(QWidget* parent, SurfaceImgs* surface); //constructor
	enum Mode {SURFACE, INSCRIPTION, GRAPH}; 
	void newSurf(); //gets image from disk, sets mode to 
			//SURFACE, resets display
	void reset(); //sets view after change of mode or new surface.
	Mode getMode() const; //returns current value of mode
	QString getModeName() const; //returns name of mode as a string
	double getZoom() const; //returns zoom
	double getRotation() const ; //returns rotation
        void getGraphImageList(const int imageIndex, QList<QImage>& imgList, QSize size);
            //appends graph images to graphList, scaled to size, keeping aspect ratio

public slots:
	//the following are triggered by Viewer QActions.
	void zoomIn();
	void zoomOut();
	void zoomRestore();
	void rotateClockwise();
	void rotateAntiClockwise();
	void rotateRestore();
	void modeUp();
	void modeDown();
//	void saveThumbnails();	//saves the contents of each bounding box to disk
	void toggleIndexNumbers();
//	int getCurrentBoxIndex();
//	void setCurrentBoxIndex(int index);
	void advanceCurrentBoxIndex();
	void reverseCurrentBoxIndex();
	void deleteCurrentBox();
//	void lock(); //sets locked to true
	void unlock(); //sets locked to false
//	bool isLocked(); //returns value of locked

signals:
	void inscrImgListModified();

protected:
	void paintEvent(QPaintEvent* event); //reimplement paintEvent(?)

private:
	void rotateAndCrop(const QImage& startImg, const BoundingBox* box, QImage& endImg);
		//sets endImg to cropped and rotated startImg
		//TODO reimplement other methods using rotateAndCrop
        bool confirmDelete(QString type); //Message box to confirm that consequent deletion
            //of graph or inscription images is ok. "type" names the type of image.
	
	//rubberband selection
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void transformImage();

	SurfaceImgs* surf;
	Mode mode;
	QPoint origin; //start of rubberband
	QRubberBand* rubberBand;
	
	QImage originalImage; //image loaded from disk
					//store currnent image on SURFACE > INSCRIPTIONS
	QImage surfaceImage; //image cropped to surface
					//stores current image on INSCRIPTIONS > GRAPHS
	QImage currentImage; //holds untransformed version of image 
					//currently being displayed
	QImage transformedImage; //version of image that is displayed

	double zoom; //current magnification of image
	int rotation; //rotation of image in degrees clockwise
	QTransform transform; //matrix for performing zoom and rotation

	bool indexNumbersVisible;
	int currentBoxIndex;
	int currentInscrIndex; //=0 on entry to INSCRIPTIONS mode
				//stores current inscription index while in GRAPH mode
	bool locked; //if locked, no rubber bands can be dragged or other changes made to box list
};

#endif
