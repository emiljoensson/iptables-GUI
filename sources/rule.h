#ifndef RULE_H
#define RULE_H

#include <QString>

class Rule
{
private:
    QString sIP, dIP, protocol, interface;
    int sPort, dPort;
public:
    Rule();
    Rule(QString sIP, QString dIP, int sPort, int dPort, QString protocol, QString interface);
};

#endif // RULE_H
