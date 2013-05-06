#ifndef SERVICE_H
#define SERVICE_H

#include <QString>

class Service
{
private:
    QString name;
    int port;
    QString protocol;
    QString action;
public:
    Service();
    Service(QString name, int port, QString protocol, QString action);
    QString getName() const;
    int getPort() const;
    QString getProtocol() const;
    QString getAction() const;
    void setName(QString name);
    void setPort(int port);
    void setProtocol(QString protocol);
    void setAction(QString action);
};

#endif // SERVICE_H
