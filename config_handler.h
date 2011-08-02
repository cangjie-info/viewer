#ifndef CONFIG_HANDLER_H
#define CONFIG_HANDLER_H

#include <QString>


class ConfigHandler
{
public:
    ConfigHandler();
    void save() const; //saves currnet configuration
    int getLastSurf() const; //returns last surface id
    void setLastSurf(const int); //set last surface id
    void processLine(const QString line); //processes line of config file
private:
    int lastSurf; //rank of last viewed surface within corpus
};

#endif // CONFIG_HANDLER_H
