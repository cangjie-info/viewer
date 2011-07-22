//class for bounding box around inscription or glyph in image
//TODO could rotation not just be made a public member??

#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <QRect>

class BoundingBox : public QRect
{
	//	Q_OBJECT  -  this generated an error 
public:
	BoundingBox(); //constructor
	BoundingBox(QPoint point1, QPoint point2, int angle/*,bool null*/); //constructor
	void setBox(QPoint point1, QPoint point2, int angle/*, bool null*/);
		//set all member variable of box
	int getRotation() const;		//returns rotation
//	void setNull(bool null); //sets isNull to null
//	void toggleNull(); //toggles isNull
	bool boxIsNull() const; //true if zero box
	void report() const;
private:
	int rotation;		//rotation of image when rectangle was set
							//center of rotation depends on image !!
//DELETE	bool isNull;	//if a box is null, it simply occupies a position in
			//a list of boxes; values of QRect and rotation are just junk.
			//This if for cases where there is a graph in the transcription but 
			//nothing visible in the surface image to have a bbox drawn around.
			//the same functionality could have been achieved by setting QRect to zero
			//but this seems simpler. It also means that null-ing a bbox could be made 
			//undoable, since the value of QRect will preserve.
};
	
#endif
