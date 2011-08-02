#include "db_handler.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QVariant>
#include "cangjie_img.h"

DbHandler::DbHandler()
    : pCorpusQuery(NULL)
{
}

bool DbHandler::connect()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("ec");
    db.setUserName("qt");
    db.setPassword("abc123");
    if(!db.open())
    {
        QMessageBox::critical(0, QObject::tr("Database Error"),
                              db.lastError().text());
        return false;
    }
    return true;
}

bool DbHandler::setCorpus()
{
    pCorpusQuery = new QSqlQuery();
    pCorpusQuery->exec("SELECT id FROM surfaces;");
    if(pCorpusQuery->size() > 0)
    {
        qDebug() << pCorpusQuery->size() << "rows in the query result";
        return true;
    }
    else
        return false;
}

bool DbHandler::nextSurface()
{
    if(pCorpusQuery->next())
        return true;
    else
    {
        pCorpusQuery->last(); //move back to last
        return false;
    }
}

bool DbHandler::previousSurface()
{
    if(pCorpusQuery->previous())
        return true;
    else
    {
        pCorpusQuery->first(); //move back to first
        return false;
    }
}

void DbHandler::moveToSurf(int index)
{
    if(!pCorpusQuery->seek(index))
        pCorpusQuery->first();
}

int DbHandler::getCorpusSize() const
{
    if(pCorpusQuery)
        return pCorpusQuery->size();
    else
        return 0; //if corpus has not been set
}

int DbHandler::getPositionInCorpus() const
{
    if(pCorpusQuery)
        return pCorpusQuery->at(); //zero for first record
    //Qt DOC: <<If the position is invalid, the function returns QSql::BeforeFirstRow
    //or QSql::AfterLastRow, which are special negative values.>>
    else
        return -1; //if corpus has not been set
}

int DbHandler::getCurrentSurfaceId() const //returns ec.surfaces.id
{
    return pCorpusQuery->value(0).toInt();
}

void DbHandler::readSurface(SurfaceImgs& surf, SurfaceTranscription& trans) const
{
    //clear both lists of inscriptions (which deletes graphs too)
    surf.deleteAllInscriptions();
    trans.clear();

    QString currentSurfId = pCorpusQuery->value(0).toString();
    //Query ec.surfaces
    QString surfaceQueryString(
            "SELECT imageFile, x1, y1, x2, y2, rotation, publicationId, pubNumber, surfaceType FROM surfaces WHERE id=");
    surfaceQueryString += currentSurfId;
    surfaceQueryString += ";";
    QSqlQuery surfaceQuery(surfaceQueryString);
    surfaceQuery.next(); //move first (and last, we hope)
    //get imageFile
    surf.setImageFile(surfaceQuery.value(0).toString());
    //get surface box
    //if x1 is NULL, then surface box is zero
    if(surfaceQuery.isNull(1))
    {
        surf.setBox(QPoint(0,0), QPoint(0,0), 0/*, true*/);
    }
    else
    {
        surf.setBox(	QPoint(surfaceQuery.value(1).toInt(), surfaceQuery.value(2).toInt()),
                        QPoint(surfaceQuery.value(3).toInt(), surfaceQuery.value(4).toInt()),
                        surfaceQuery.value(5).toInt()/*, false*/);
    }
    //for trans, get last three fields in query
    trans.setPubId(surfaceQuery.value(6).toString());
    trans.setPubNumber(surfaceQuery.value(7).toString());
    trans.setSurfaceType(surfaceQuery.value(8).toString());

    //query ec.inscriptions
    QString inscriptionQueryString("SELECT id, x1, y1, x2, y2, rotation FROM inscriptions WHERE surfaceId=");
    inscriptionQueryString += currentSurfId;
    inscriptionQueryString += " ORDER BY id DESC;"; //DESC allows insertion at index=0
    //TODO order by serialNumber instead
    QSqlQuery inscriptionQuery(inscriptionQueryString);
    while(inscriptionQuery.next())
    {
        //add inscription to trans, no matter what
        trans.prepend(InscriptionTranscription());
        if(!inscriptionQuery.isNull(1)) //add inscription bbox to list if not null
        {
            surf.insertInscr(
                    BoundingBox(
                            QPoint(inscriptionQuery.value(1).toInt(), inscriptionQuery.value(2).toInt()),
                            QPoint(inscriptionQuery.value(3).toInt(), inscriptionQuery.value(4).toInt()),
                            inscriptionQuery.value(5).toInt()/*, false*/),
                    0); //inserting at the beginning of the list
            trans[0].setCanHaveImage(true);
        }
        else //if bbox is null, set insription trans canHaveImage to false
            trans[0].setCanHaveImage(false);

        //for each inscription, query graphs.
        QString currentInscrId = inscriptionQuery.value(0).toString();
        QString graphQueryString(
                "SELECT x1, y1, x2, y2, rotation, markup, graphemeId FROM inscriptionGraphs WHERE inscriptionId=");
        graphQueryString += currentInscrId;
        graphQueryString += " ORDER BY graphNumber DESC;"; //DESC allows insertion at index=0
        QSqlQuery graphQuery(graphQueryString);
        while(graphQuery.next())
        {
            //add graph transcription to trans[0]
            GraphTranscription newGraph(
                    graphQuery.value(5).toInt(), 	//markup
                    graphQuery.value(6).toInt()); 	//grapheme
            trans[0].prepend(newGraph);
            //add graph bbox to inscription
            if(!graphQuery.isNull(0)) //if non-null graph box
            {
                surf.ptrInscrAt(0)->insertBox(
                        BoundingBox(
                                QPoint(graphQuery.value(0).toInt(), graphQuery.value(1).toInt()),
                                QPoint(graphQuery.value(2).toInt(), graphQuery.value(3).toInt()),
                                graphQuery.value(4).toInt()/*, false*/),
                        0); 	//prepend (i.e. insert at index 0) since we used DESC
                trans[0][0].setCanHaveImage(true);
            }
            else //null graph box, so set canHaveImage for transcription to false
                trans[0][0].setCanHaveImage(false);
        }
    }
}

void DbHandler::writeSurface(SurfaceImgs& imgs, SurfaceTranscription& trans)
{
db.transaction(); //begin transaction
    QString surfaceId = pCorpusQuery->value(0).toString();

    //*** SURFACE ***//

    //UPDATE bbox in ec.surface
    QString surfaceUpdateString = QString(
            "UPDATE surfaces SET x1=%1, y1=%2, x2=%3, y2=%4, rotation=%5 WHERE id=%6;")
            .arg(imgs.x())  //x1
            .arg(imgs.y())  //y1
            .arg(imgs.x()+imgs.width())     //x2
            .arg(imgs.y()+imgs.height())    //y2
            .arg(imgs.getRotation())        //rotation
            .arg(surfaceId); //id
    QSqlQuery surfaceUpdateQuery;
    if(!surfaceUpdateQuery.exec(surfaceUpdateString))
    {
  db.rollback(); //handle as error!!
        return;
    }

    //*** DELETE OLD INSCRIPTIONS AND GRAPHS ***//

    //DELETE query for corresponding inscriptions and graphs
    QString inscriptionDeleteString = QString(
            "DELETE inscriptions, inscriptionGraphs "
            "FROM inscriptions LEFT JOIN inscriptionGraphs "
            "ON inscriptions.id=inscriptionGraphs.inscriptionId "
            "WHERE inscriptions.surfaceId=%1;")
            .arg(surfaceId); //surface id
    QSqlQuery inscriptionDeleteQuery;
    if(!inscriptionDeleteQuery.exec(inscriptionDeleteString))
    {
    db.rollback();
        return;
    }

    //*** INSERT NEW INSCIRPTIONS AND GRAPHS ***//

    //for each inscription in trans, INSERT inscription,
    //then INSERT inscription graphs
    int imgsIndex = -1; //increment on encountering each image
    for(int transIndex = 0; transIndex < trans.count(); transIndex++)
    {

        //*** NEW INSCRIPTION ***//

        QString insertInscriptionString = QString(
                "INSERT INTO inscriptions "
                "SET serialNumber=%1, "
                    "surfaceId=%2")
                .arg(transIndex + 1) //1-based index in ec, not zero based
                .arg(surfaceId);
        bool inscrHasImage = false;
        if(imgs.inscriptionCount() > 0
           && imgsIndex < imgs.inscriptionCount()-1
            && trans[transIndex].getCanHaveImage())
            //inscription has an img
        {
            inscrHasImage = true;
            imgsIndex++;
            int x1 = imgs.ptrInscrAt(imgsIndex)->x();
            int y1 = imgs.ptrInscrAt(imgsIndex)->y();
            int x2 = x1 + imgs.ptrInscrAt(imgsIndex)->width();
            int y2 = y1 + imgs.ptrInscrAt(imgsIndex)->height();
            int rotation = imgs.ptrInscrAt(imgsIndex)->getRotation();

            insertInscriptionString += QString(
                    ", x1=%1, y1=%2, x2=%3, y2=%4, rotation=%5")
                    .arg(x1).arg(y1).arg(x2).arg(y2).arg(rotation);
        }
        else
        {
            //nothing to do - db will provide default NULL values
        }
        insertInscriptionString += ";";
        QSqlQuery insertInscriptionQuery;
        if(!insertInscriptionQuery.exec(insertInscriptionString))
        {
db.rollback();
            return;
        }

        //*** NEW GRAPHS FOR LAST INSCRIRPTION ***//

        //INSERT graphs for the inscription just inserted if there are any
        //number of rows = maximum of trans[transIndex].count() and imgs.ptrInscAt(imgsIndex)->count()
        int numberOfGraphs;
        if(inscrHasImage)
        {
            numberOfGraphs = (trans[transIndex].count() > imgs.ptrInscrAt(imgsIndex)->boxCount()
                            ? trans[transIndex].count() : imgs.ptrInscrAt(imgsIndex)->boxCount());
        }
        else //no image, only transcription or nothing
        {
            numberOfGraphs = trans[transIndex].count();
        }
        if(numberOfGraphs == 0)
        {
            //no need for INSERT query - do nothing
        }
        else
        {
            int inscriptionId = insertInscriptionQuery.lastInsertId().toInt();
            int graphImgsIndex = 0;
            //this should return the autonumber field inscriptions.id
            QString insertGraphsString = QString(
                    "INSERT INTO inscriptionGraphs "
                    "(inscriptionId, graphNumber, x1, y1, x2, y2, rotation, markup, graphemeId) "
                    "VALUES ");             //in format (1, 2, 3, 4, 5, 6, 7, 8, 9), ...(...);
            for (int graphIndex = 0; graphIndex < numberOfGraphs; graphIndex++)
            {
                QString x1, y1, x2, y2, rotation;
                int markup, graphemeId;
                //if imgs are left && (no trans left || canHaveImage) => row has graph image
                if(inscrHasImage
                    && graphImgsIndex < imgs.ptrInscrAt(imgsIndex)->boxCount() //imgs are left
                    && (
                        graphIndex >= trans[transIndex].count() //no trans left
                        || trans[transIndex][graphIndex].getCanHaveImage() == true) ) //can have image
                    //the last term in the boolean is only evaluated if there are trans left
                { //get bbox and add to query, increment graphsImgsIndex
                    BoundingBox graphBox = imgs.ptrInscrAt(imgsIndex)->at(graphImgsIndex);
                    x1.setNum( graphBox.x() );
                    y1.setNum( graphBox.y() );
                    x2.setNum( graphBox.x() + graphBox.width() );
                    y2.setNum( graphBox.y() + graphBox.height() );
                    rotation.setNum( graphBox.getRotation() );
                    graphImgsIndex++;
                }
                else //NULL bbox
                {
                    x1 = "NULL";
                    y1 = "NULL";
                    x2 = "NULL";
                    y2 = "NULL";
                    rotation = "NULL";
                }
                //if trans left
                if(graphIndex < trans[transIndex].count())
                {
                    //get markup and graphemeId and add to query
                    markup = trans[transIndex][graphIndex].getMarkup();
                    graphemeId = trans[transIndex][graphIndex].getGrapheme();
                }
                else // no more trans graphs
                {
                    markup = 0;
                    graphemeId = 0;
                }
                QString values = QString("(%1, %2, %3, %4, %5, %6, %7, %8, %9)")
                                 .arg(inscriptionId)
                                 .arg(graphIndex + 1) //one-based, not zero-based in ec
                                 .arg(x1).arg(y1).arg(x2).arg(y2)
                                 .arg(rotation)
                                 .arg(markup)
                                 .arg(graphemeId);
                if(graphIndex != 0)
                    values.prepend(", ");
                insertGraphsString += values;
            }
            insertGraphsString += ";"; //query string finished
            QSqlQuery insertGraphsQuery;
            if(!insertGraphsQuery.exec(insertGraphsString))
            {
        db.rollback();
                return;
            }
        }
    }
db.commit(); //done - yeay!
}

int DbHandler::getGrapheme(QString searchString) //static
{
    //assume searchString is a name
    QSqlQuery signListQuery;
    QString queryString = QString("SELECT id FROM signList WHERE name=\"%1\";")
                          .arg(searchString);
    signListQuery.exec(queryString);
    if(signListQuery.first())
        return signListQuery.value(0).toInt();
    else
        return 0;
}
//tries to find grapheme in ec.signList corresponding to searchString
//for now, only searches name field
//returns grapheme id

void DbHandler::findGraphemeInstances(QList<int> &graphIdList, const int grapheme)
{
    QString queryString = QString("SELECT id FROM inscriptionGraphs WHERE graphemeId=%1;")
                          .arg(grapheme);

    QSqlQuery query;
    query.exec(queryString);
    while(query.next())
    {
        graphIdList.append(query.value(0).toInt());
    }
}

void DbHandler::getGraphImage(QImage& graphImage, const int graphId, const int size)
{
    //query graphId (with inner joins to inscriptions and surfaces)
    QString queryString = QString("SELECT surfaces.imageFile, "   //0
                                  "surfaces.x1, surfaces.y1, "  //1, 2
                                  "surfaces.x2, surfaces.y2, "  //3, 4
                                  "surfaces.rotation, "         //5
                                  "inscriptions.x1, inscriptions.y1, " //6, 7
                                  "inscriptions.x2, inscriptions.y2, " //8, 9
                                  "inscriptions.rotation, "     //10
                                  "inscriptionGraphs.x1, inscriptionGraphs.y1, " //11, 12
                                  "inscriptionGraphs.x2, inscriptionGraphs.y2, " //13, 14
                                  "inscriptionGraphs.rotation " //15
                                  "FROM inscriptionGraphs "
                                  "INNER JOIN inscriptions "
                                  "INNER JOIN surfaces "
                                  "ON inscriptionGraphs.inscriptionId=inscriptions.id "
                                  "AND inscriptions.surfaceId=surfaces.id "
                                  "WHERE inscriptionGraphs.id=%1;"
                                  ).arg(graphId);
    QSqlQuery query;
    qDebug() << queryString;
    query.exec(queryString);
    //if null image return black image
    if(!query.next() || query.value(11).isNull())
    {
        graphImage = QImage();
        return;
    }
    else
    {
        //get image file
        QString fileName = query.value(0).toString();
        fileName.prepend("/home/ads/repository/text_imgs/");
        QImage image = QImage(fileName);
        QImage surfaceImage;
    //make surface image
        CangjieImg::rotateAndCrop(image,
            BoundingBox(QPoint(query.value(1).toInt(), query.value(2).toInt()),
                        QPoint(query.value(3).toInt(), query.value(4).toInt()),
                        query.value(5).toInt()),
            surfaceImage);
    //make inscription image
        QImage inscriptionImage;
        CangjieImg::rotateAndCrop(surfaceImage,
            BoundingBox(QPoint(query.value(6).toInt(), query.value(7).toInt()),
                        QPoint(query.value(8).toInt(), query.value(9).toInt()),
                        query.value(10).toInt()),
            inscriptionImage);
    //make graph image
        CangjieImg::rotateAndCrop(inscriptionImage,
            BoundingBox(QPoint(query.value(11).toInt(), query.value(12).toInt()),
                        QPoint(query.value(13).toInt(), query.value(14).toInt()),
                        query.value(15).toInt()),
            graphImage);
    //adjust to size
        graphImage = graphImage.scaled(QSize(size, size), Qt::KeepAspectRatio);
    }
}

void DbHandler::fixCracks()
{
    //select all graphs "." or numerals
    QString queryString = "SELECT id, inscriptionId, graphNumber, graphemeId "
                          "FROM inscriptionGraphs "
                          "WHERE (graphemeId>=718 "
                          "AND graphemeId<=727) "
                          "OR graphemeId=1001 "
                          "ORDER BY inscriptionId, graphNumber;";
    QSqlQuery query;
    query.exec(queryString);
    int previousInscriptionId = 0;
    int previousGraphNumber = 0;
    int previousGraphemeId = 0;

    int currentGraphId = 0;
    int currentInscriptionId = 0;
    int currentGraphNumber = 0;
    int currentGraphemeId = 0;
	 int adjustment = 0;
    while(query.next())
    {
        currentGraphId = query.value(0).toInt();
        currentInscriptionId = query.value(1).toInt();
        currentGraphNumber = query.value(2).toInt();
        currentGraphemeId = query.value(3).toInt();
        if(previousInscriptionId == currentInscriptionId
           && previousGraphNumber + 1 == currentGraphNumber
           && previousGraphemeId == 1001)
        {
					adjustment++;
            QString updateQueryString = QString("UPDATE inscriptionGraphs "
                                        "SET markup = markup | %1 , graphNumber = %2 "
                                        "WHERE id = %3;")
                    .arg(GraphTranscription::CRACK_NUMBER)
                    .arg(previousGraphNumber - adjustment)
                    .arg(currentGraphId);
            QSqlQuery updateQuery;
            updateQuery.exec(updateQueryString);

            qDebug() << currentGraphId;
        }
		  if(previousInscriptionId != currentInscriptionId)
		  	adjustment = 0;
        previousInscriptionId = currentInscriptionId;
        previousGraphNumber = currentGraphNumber;
        previousGraphemeId = currentGraphemeId;
    }

    QString deleteQueryString = "DELETE inscriptionGraphs "
                        "WHERE graphemeId = 1001;";
    QSqlQuery deleteQuery;
    deleteQuery.exec(deleteQueryString);
}
