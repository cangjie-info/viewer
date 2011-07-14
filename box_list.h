#ifndef BOX_LIST_H
#define BOX_LIST_H

//class to represent a list of bounding boxes
//boxes can be for graphs, inscriptions or surfaces
//base class of Inscription and Surface classes
//MAYBE? base class of Image class?
//Contains a QList of bounding boxes 

#include "bounding_box.h"
#include <QList>

class BoxList
{
public:
	BoxList(); //constructor
	void insertBox(BoundingBox newBox, int index); //inserts box at index
		//list unchanged if out of range
	void deleteBox(int index); //deletes box at index
		//list unchanged if out of range
	void toggleNull(int index); //toggles value of null at index
		//list unchanged if index oor.
	BoundingBox boxAt(int index) const; //returns box at index
		//returns null zero box if index oor.
	bool boxNullAt(int index) const; //returns isNull for box at index
		//true if oor.
	int boxCount() const; //number of boxes in list
	void report() const;

private:
	//variables
	QList<BoundingBox> boxList;
};

#endif
