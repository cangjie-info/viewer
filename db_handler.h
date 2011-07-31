#ifndef DB_HANDLER_H
#define DB_HANDLER_H

#include <QSqlQuery>
#include <qimage.h>
#include "surface_imgs.h"
#include "surface_transcription.h"

//class to handle ALL interaction with the db
//AIM: no other class should have any QSqlQuery variables

class DbHandler
{
public:
    DbHandler();  //constructor
    bool connect();
    //connects to ec db as user "qt".
    //returns true if connection succesful, false + error message box otherwise
    bool setCorpus();
    //queries ec.inscribedSurfaces using corpusQuery
    //to get a result set consisting of id numbers for surfaces
    //the default is to select all surfaces in the table
    //if corpus has been set, position will be before the first record
    //must use nextSurface() before attempting to read
    //TODO variable corpora
    bool nextSurface();
    //move to next record in corpusQuery
    //if no netx record, return false and stay on last record
    bool previousSurface();
    //move to previous record in corpusQuery
    //if no previous record, return false and stay on first record
    int getCorpusSize() const;
    //returns number of surfaces in corpus
    int getPositionInCorpus() const;
    //returns ordinal position of current surface in corpus
    void readSurface(SurfaceImgs& surf, SurfaceTranscription& trans) const; //gets currentSurface via DbHandler
    static int getGrapheme(QString searchString);
    //tries to find grapheme in ec.signList corresponding to searchString
    //for now, only searches name field
    //returns grapheme id
    void writeSurface(SurfaceImgs& surf, SurfaceTranscription& trans); //writes currentSurface via DbHandler
    //must delete all inscriptions and graphs from old version of surface
    //use transaction
    static void findGraphemeInstances(QList<int>& graphIdList, const int grapheme);
        //returns list of graphIds for the corresponding grapheme
    static void getGraphImage(QImage& image, const int graphId, const int size);
        //returns image of graph specified by graphId.
private:
    //functions
    //data members
    QSqlDatabase db; //connection to the ec database
    QSqlQuery* pCorpusQuery;	//result is a list of surface ids.
    //must be a pointer, so that query can be created after db
    //is opened.
    //see: http://www.qtcentre.org/archive/index.php/t-25184.html
};

#endif
