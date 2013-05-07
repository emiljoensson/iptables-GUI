#ifndef RULE_H
#define RULE_H

#include <QString>

class Rule
{
private:
    QString sIP, dIP, protocol, interface;
    int sPort, dPort;
public:
    Rule(QString sIP="null", QString dIP="null", int sPort=0, int dPort=0, QString protocol="null", QString interface="null");
};

#endif // RULE_H
