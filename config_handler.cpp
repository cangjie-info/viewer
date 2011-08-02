#include "config_handler.h"

#include <QFile>
#include <QDir>
#include <QTextStream>
#include "db_handler.h"

ConfigHandler::ConfigHandler()
    : lastSurf(0)
{
    //check to see whether config file exists
    QDir home = QDir().home();
    QFile configFile;
    QDir::setCurrent(home.path());
    configFile.setFileName(".cangjie");
    if(configFile.exists())
    {
        //read it
        configFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream config(&configFile);
        QString line(config.readLine());
        while(!line.isNull())
        {
            processLine(line);
            line = config.readLine();
        }
    }
    else
    {
        //make it
        configFile.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream config(&configFile);
        config << "lastSurf=0\n";
        configFile.close();
    }
}

void ConfigHandler::save() const
{
    QDir home = QDir().home();
    QDir::setCurrent(home.path());
    QFile::remove(".cangjie_old");
    QFile::rename(".cangjie", ".cangjie_old");
    QFile configFile;
    configFile.setFileName(".cangjie");
    configFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream config(&configFile);

    config << QString("lastSurf=%1\n")
            .arg(lastSurf);
    //and the rest - TODO
}

void ConfigHandler::processLine(QString line)
{
    QStringList nameValue = line.split("=");
    if(nameValue[0] == "lastSurf")
        lastSurf = nameValue[1].toInt();
    //other config varialbles
}
int ConfigHandler::getLastSurf() const
 {
     return lastSurf;
 }

void ConfigHandler::setLastSurf(const int index)
 {
     lastSurf = index;
 }
