#include "db_handler.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QVariant>

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

bool DbHandler::readSurface(SurfaceImgs& surf, SurfaceTranscription& trans) const
{
    if(!pCorpusQuery) //corpus has not been set
        return false;
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
    return true;
}
int DbHandler::getGrapheme(QString searchString) //static
{
    //assume searchString is a name
    qDebug() << searchString;
    QSqlQuery signListQuery;
    QString queryString = QString("SELECT id FROM signList WHERE name=\"%1\";")
               .arg(searchString);
    qDebug() << queryString;
    signListQuery.exec(queryString);
    if(signListQuery.first())
        return signListQuery.value(0).toInt();
    else
       return 0;
}
//tries to find grapheme in ec.signList corresponding to searchString
//for now, only searches name field
//returns grapheme id
