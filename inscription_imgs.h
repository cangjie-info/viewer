#ifndef INSCRIPTION_IMGS_H
#define INSCRIPTION_IMGS_H

//consists of a list inherited from BoxList (is-a BoxList).
//also has a (or rather "is-a")  bounding box of its own corresponding to the bbox of the inscription

//an instance of the class corresponds to one row of ec.inscriptions
//together with the the associated entries from ec.inscriptionGraphs.

//transcriptional info is handled in a different class
//no changes to this class must change any representation of 
//the transcription

//inherits (is-a) BoxList for list of graph bounding boxes.
//inherits (is-a) BoundingBox.
//NOTE this should have been done with composition rather than inheritance,
//but there we are...

#include "graph_transcription.h"
#include "bounding_box.h"
#include "box_list.h"
#include <QList>

class InscriptionImgs : public BoxList, public BoundingBox
{
public:
	InscriptionImgs(); //constructor
	InscriptionImgs(BoundingBox inscrBox); //constructor
	void report() const;
private:
};

#endif
