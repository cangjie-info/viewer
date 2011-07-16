#include "imagelabel.h"
#include <QRubberBand>
#include <QMouseEvent>
#include <QRect>
#include <QImage>
#include <QPainter>
#include <QString>
#include <QChar>
#include <QFont>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>

ImageLabel::ImageLabel(QWidget* parent, QList<BoundingBox>* pBoxList, const QImage* currentImage) //constructor
	: QLabel(parent), rubberBand(NULL), 
		zoom(1), rotation(0), 
				pBoxList(pBoxList), image(currentImage)
{
	setAlignment(Qt::AlignTop | Qt::AlignLeft); 	//label contents aligned top left
	refresh();
}

void ImageLabel::refresh()
{
	transformedImage = *image;
	zoom = 1;
	rotation = 0;
	indexNumbersVisible = false;
	const QRect res = QApplication::desktop()->availableGeometry();
	while(transformedImage.height() > res.height() || 
			transformedImage.width() > res.width())
	{
		zoomOut();
	}
	//in case no resizing took place, we still need to ensure that 
	//private member transform is updated, so...
	transformImage();
	if(pBoxList->isEmpty())
		currentBoxIndex = -1;
	else
		currentBoxIndex = 0;
	update();
}
void ImageLabel::setCurrentBoxIndex(int index)
{
	if(index<0 || index>=pBoxList->size())
	{
		return;
	}
	else
	{
		currentBoxIndex=index;
		return;
	}
}

void ImageLabel::setMaxListLength(int max)
{
	maxListLength = max;
}	

void ImageLabel::lock()
{
	locked = true;
}

void ImageLabel::unlock()
{
	locked = false;
}

bool ImageLabel::isLocked()
{
	return locked;
}

void ImageLabel::toggleIndexNumbers()
{
	indexNumbersVisible = !indexNumbersVisible;
	update(); //to ensure that they become (in)visible immediately
}

int ImageLabel::getCurrentBoxIndex()
{
	return currentBoxIndex;
}

void ImageLabel::advanceCurrentBoxIndex()
{
	currentBoxIndex++;	//advance index by one
	if(currentBoxIndex >= pBoxList->size())
		currentBoxIndex = 0;	//wrap around if necessary
	update();
}

void ImageLabel::reverseCurrentBoxIndex()
{
	currentBoxIndex--; //go back one
	if(currentBoxIndex < 0)	//wrap around if necessary
		currentBoxIndex = pBoxList->size() - 1;
	update(); //so that that current box is visible
}

void ImageLabel::deleteCurrentBox()
//TODO: if an inscription or surface box is deleted
//all boxes below it in the hierarchy should be deleted too
{
	if(pBoxList->isEmpty() || locked)
	{
		return;
	}
	else
	{
		pBoxList->removeAt(currentBoxIndex);
		currentBoxIndex--;
		if(currentBoxIndex == -1 && pBoxList->isEmpty()==false)
			currentBoxIndex = 0;
		update();
	}
}

void ImageLabel::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);

	//draw the transformed version of the text-object image
	painter.drawImage(0, 0, transformedImage); 
	
	//then we draw the bounding boxes
			//TODO inscriptions with at least one graph are a different color (?)

	//in case index numbers are visible, set font
	QFont font;
	font.setPixelSize(30); //TODO make font size dependent on resolution
	painter.setFont(font);
	painter.setPen(Qt::red);
	//iterate through the list of bounding boxes
	for (int i=0; i<pBoxList->size(); i++)
	{
qDebug() << "now drawing box " << i;
		BoundingBox currentBox = pBoxList->at(i);

		//the bounding boxes need to be rotated and scaled
		QTransform boxTransform; //identity matrix
		//first we need to handle the bounding box's own rotation
		//by inverting the true matrix that was applied to the image
		//at the time each bounding box was created
		//(note: we don't need to worry about scale, as we took account of that
		//when the bounding box was created)
		boxTransform.rotate(currentBox.getRotation());
		boxTransform = QImage::trueMatrix(boxTransform, image->width(), image->height());
		boxTransform = boxTransform.inverted();
	
		//then we compound the above matrix with the current transformation of the image
		QTransform imageTrueTransform = QImage::trueMatrix(transform, image->width(), image->height());
		painter.setWorldTransform(boxTransform * imageTrueTransform);
		//now draw the box
		//pen color is red; set the pen-color to green if this is the current box.
		if(i==currentBoxIndex)
		{
			painter.setPen(Qt::green);
		}
		painter.drawRect(currentBox);
		//and add an (optional) index number
		if(indexNumbersVisible)
		{
			painter.drawText(currentBox.left(), currentBox.top(), 50, 50, Qt::AlignBottom,  QString("%1").arg(i));
		}
		//return pen color to red (might be green)
		painter.setPen(Qt::red);
	}

	//if label is not resized, it will stay large on zoom out
	//resulting in misleading / redundant scroll bars
	resize(transformedImage.size()); // keep label same size as image
}

void ImageLabel::transformImage()
{
	transform.reset(); //set to identity matrix
	transform.rotate(rotation);	//set matrix
	transform.scale(zoom, zoom);
	transformedImage = image->transformed(transform);		//make rotated version of image
}	

//mouse down, start rubberband
void ImageLabel::mousePressEvent(QMouseEvent* event)
{
	origin = event->pos();
	if(!rubberBand)
	{
		rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
		//attempting to set the palette of the QRubberBand
		//for colors etc. does not have any affect on apearance.
	}
	rubberBand->setGeometry(QRect(origin, QSize())); 	//rubberband as a QRect 
																		//of zero size at origin
	rubberBand->show();
}

//dragging rubberband
void ImageLabel::mouseMoveEvent(QMouseEvent* event)
{
	rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
	//normalized, in case dragging up or left
}

//mouse up, end rubberband
void ImageLabel::mouseReleaseEvent(QMouseEvent* event)
{
	rubberBand->hide();

	//get bBox, accounting for rotation and zoom
	QRect rect(origin/zoom, event->pos()/zoom);
	rect = rect.normalized(); //topLeft etc. are not reliable without this
	//if it was a tiny drag, it was probably an accident
	if(rect.height()<3 || rect.width()<3 || locked)
	{
		update(); //to remove the mess
		return;
	}
	BoundingBox bBox(rect.topLeft(), rect.bottomRight(), rotation);

	//remove any list items that would make list too long
	while(pBoxList->length() >= maxListLength)
	{
		pBoxList->removeFirst();
		--currentBoxIndex;
	}
	//add bBox to list and increment index
	pBoxList->insert(++currentBoxIndex, bBox);

	//clear up mess left by rubber band
	update();
}

void ImageLabel::zoomIn()
{
	if(zoom<5.0) 
	{
		zoom *= 4.0/3.0; //enlarge displayed image by 1+1/3
		transformImage();
		update(); 
	}
}

void ImageLabel::zoomOut()
{
	if(zoom>0.1)
	{
		zoom*=0.75; //shrink displayed image to 3/4
		transformImage();
		update();
	}
}

void ImageLabel::zoomRestore()
{
	zoom=1.0; //reset displayed image scale
	transformImage();
	update();
}

void ImageLabel::rotateClockwise()
{
	rotation+=5; // +5 degrees
	if(rotation>=360)			//keep 0 <= rotation < 360
		rotation -= 360;
	transformImage();
	update();
}

void ImageLabel::rotateAntiClockwise()
{
	rotation-=5; // -5 degrees
		if(rotation < 0)		//keep 0 <= rotation < 360
			rotation += 360;
	transformImage();
	update();
}

void ImageLabel::rotateRestore()
{
	rotation=0;
	transformImage();
	update();
}

void ImageLabel::saveThumbnails()
{
	//iterate through list of bBoxes
	for (int i=0; i < pBoxList->size(); i++)
	{
		BoundingBox currentBox = pBoxList->at(i);
		//rotate copy of image by the appropriate amount
		QTransform trans; //identity matrix
		trans.rotate(pBoxList->at(i).getRotation());
		QRect selection = pBoxList->at(i);
		QImage imageCopy = ( image->transformed(trans) ) . copy(selection);
		//make zero padded file-name
		QString fileName = QString("img_%1.jpg").arg(i, 3, 10, QChar('0'));
		//save selection to disk
		imageCopy.save(fileName);
	}
}
