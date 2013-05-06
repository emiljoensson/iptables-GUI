#include "service.h"

Service::Service()
{
    this->name = "NULL";
    this->port = 0;
    this->protocol = "NULL";
    this->action = "NULL";
}

Service::Service(QString name, int port, QString protocol, QString action)
{
    this->name = name;
    this->port = port;
    this->protocol = protocol;
    this->action = action;
}

QString Service::getService() const {
    QString returnVal;
    QTextStream stringStream(&returnVal);
    stringStream << this->name << "," << this->port << "," << this->protocol << "," << this->action;
    return returnVal;
}

QString Service::getName() const {
    return this->name;
}

QString Service::getProtocol() const {
    return this->protocol;
}

int Service::getPort() const {
    return this->port;
}

QString Service::getAction() const {
    return this->action;
}


void Service::setName(QString name) {
    this->name = name;
}

void Service::setProtocol(QString protocol) {
    this->protocol = protocol;
}

void Service::setPort(int port) {
    this->port = port;
}

void Service::setAction(QString action) {
    this->action = action;
}
