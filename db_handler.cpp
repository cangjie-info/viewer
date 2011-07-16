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
	QString currentId = pCorpusQuery->value(0).toString();
	//Query ec.surfaces 
	QString surfaceQueryString("SELECT imageFile, x1, y1, x2, y2, rotation FROM surfaces WHERE id=");
	surfaceQueryString += currentId;
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

	//query images
	//for each image, query graphs.
	return true;
}
