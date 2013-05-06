#include "rule.h"

Rule::Rule()
{
    this->sIP = "NULL";
    this->dIP = "NULL";
    this->protocol = "NULL";
    this->interface = "NULL";
    this->sPort = NULL;
    this->dPort = NULL;
}

Rule::Rule(QString sIP, QString dIP, int sPort, int dPort, QString protocol, QString interface)
{
    this->sIP = sIP;
    this->dIP = dIP;
    this->sPort = sPort;
    this->dPort = dPort;
    this->protocol = protocol;
    this->interface = interface;
}
