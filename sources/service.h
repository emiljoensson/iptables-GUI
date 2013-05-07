#ifndef SERVICE_H
#define SERVICE_H

#include <QString>
#include <QTextStream>

class Service
{
private:
    QString name;
    int port;
    QString protocol;
    QString action;
public:
    Service(QString name="null", int port=0, QString protocol="null", QString action="null");
    QString getService() const;
    QString getName() const;
    int getPort() const;
    QString getProtocol() const;
    QString getAction() const;
};

#endif // SERVICE_H
