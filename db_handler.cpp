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
	pCorpusQuery = new QSqlQuery("");
	return true;
}

bool DbHandler::setCorpus()
{
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

bool DbHandler::readSurface(SurfaceImgs& surf) const
{
	if(!pCorpusQuery) //corpus has not been set
		return false;
	//clear list of inscriptions
	surf.deleteAllInscriptions();
	QString currentSurfId = pCorpusQuery->value(0).toString();
	//Query ec.surfaces 
	QString surfaceQueryString("SELECT imageFile, x1, y1, x2, y2, rotation FROM surfaces WHERE id=");
	surfaceQueryString += currentSurfId;
	surfaceQueryString += ";";

	QSqlQuery surfaceQuery(surfaceQueryString);
	surfaceQuery.next(); //move first
	//get imageFile
	surf.setImageFile(surfaceQuery.value(0).toString());
	//get surface box
	//if x1 is NULL, then surface box is null
	if(surfaceQuery.isNull(1))
	{
		surf.setBox(QPoint(0,0), QPoint(0,0), 0, true);
	}
	else
	{
		surf.setBox(	QPoint(surfaceQuery.value(1).toInt(), surfaceQuery.value(2).toInt()),
							QPoint(surfaceQuery.value(3).toInt(), surfaceQuery.value(4).toInt()),
							surfaceQuery.value(5).toInt(), false);
	}
	//query ec.inscriptions
	QString inscriptionQueryString("SELECT id, x1, y1, x2, y2, rotation FROM inscriptions WHERE surfaceId=");
	inscriptionQueryString += currentSurfId;
	inscriptionQueryString += " ORDER BY id DESC;"; //order DESC allows us to ignore trailing nulls
				//TODO order by serialNumber (DESC) instead
	QSqlQuery inscriptionQuery(inscriptionQueryString);
	//first discard trailing null boxes
	while(inscriptionQuery.next() && inscriptionQuery.isNull(1))
	{
		//skip - do nothing
	}
	//query is now either after the last record (if there were no non-null boxes)
	//or on the first non-null box
	inscriptionQuery.previous(); //to accommodate the following .next()
	while(inscriptionQuery.next())
	{
		//add inscription bbox to list
		if(inscriptionQuery.isNull(1))
			surf.insertInscr(
				BoundingBox(QPoint(0,0), QPoint(0,0), 0, true), 
				0); //prepend (insert at index 0) since we used DESC
		else
			surf.insertInscr(
				BoundingBox(	
					QPoint(inscriptionQuery.value(1).toInt(), inscriptionQuery.value(2).toInt()),
					QPoint(inscriptionQuery.value(3).toInt(), inscriptionQuery.value(4).toInt()),
					inscriptionQuery.value(5).toInt(), false),
				0); //prepend since we used DESC
		//for each inscription, query graphs.
		QString currentInscrId = inscriptionQuery.value(0).toString();
		QString graphQueryString("SELECT x1, y1, x2, y2, rotation FROM inscriptionGraphs WHERE inscriptionId=");
		graphQueryString += currentInscrId;
		graphQueryString += " ORDER BY graphNumber DESC;"; //order descending allows us to ignore trailing nulls
		QSqlQuery graphQuery(graphQueryString);
		//first ignore the trailing null graph images
		while(graphQuery.next() && graphQuery.isNull(0))
		{
			//skip - do nothing
		}
		//query is now either after the last record (if there were no non-null boxes)
		//or on the first non-null box
		graphQuery.previous(); //move previous to accommodate subsequent .next()
		while(graphQuery.next())
		{
			//add graph bbox to inscription
			if(graphQuery.isNull(0)) //null graph box
				surf.ptrInscrAt(0)->insertBox(
					BoundingBox(
						QPoint(0,0), QPoint(0,0), 0, true),
					0);	//prepend (i.e. insert at index 0) since we used DESC
			else //non-null graph box
				surf.ptrInscrAt(0)->insertBox(
					BoundingBox(
						QPoint(graphQuery.value(0).toInt(), graphQuery.value(1).toInt()),
						QPoint(graphQuery.value(2).toInt(), graphQuery.value(3).toInt()),
						graphQuery.value(4).toInt(), false),
					0); 	//prepend (i.e. insert at index 0) since we used DESC
		}		
	}
	surf.report();
	return true;
}
