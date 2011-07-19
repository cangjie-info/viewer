#ifndef SURFACE_IMGS_H
#define SURFACE_IMGS_H

//consists of (has-a) QList of InscriptionImgs instances
//also is-a bounding box, corresponding to the bbox of the surface

//instance corresponds to row of ec.surfaces

//class has no access to transcriptional info

#include <QList>
#include <QString>
#include "bounding_box.h"
#include "inscription_imgs.h"

class SurfaceImgs : public BoundingBox
{
public:
	SurfaceImgs(); //constructor
	SurfaceImgs(BoundingBox surfBox); //constructor
	void setImageFile(QString path);
	QString getImageFile();	//reuturns path to image from text_imgs/
	void insertInscr(BoundingBox inscrBox, int index); //inserts an inscription 
				//at index. does nothing if index oor
	void deleteInscr(int index); //deletes inscription at index.
				//does nothing if index oor
	void deleteAllInscriptions(); //deletes all inscriptions
	int inscriptionCount() const; //returns number of inscripitons on surface
	InscriptionImgs inscrAt(int index) const; //returns (value) the inscription at index
			//returns a zero inscripion if index oor.
	InscriptionImgs* ptrInscrAt(int index);  //returns a pointer
				// to the insription at index. returns NULL pointer if index oor
	void report() const;
private:
	QString imageFile; //path to image file in repository
	QList<InscriptionImgs> inscrImgsList;
};
#endif
