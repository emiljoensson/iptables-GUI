#ifndef PROFILE_H
#define PROFILE_H

#include "rule.h"
#include "service.h"
#include <vector>
#include <QDateTime>
#include <QTextStream>
#include <QVector>

class Profile
{
private:
    QString name;
    std::vector<QString> interfaces;
    std::vector<Service*> services;
    std::vector<Rule*> firewallRules;
    QString createdDate, editedDate;
    QString defaultPolicyIN, defaultPolicyOUT;
public:
    Profile(QString name="New Profile", QString defaultPolicyIN="accept", QString defaultPolicyOUT="accept", QString createdDate="NULL", QString editedDate="NULL");
    QString getSystemTime() const;
    QString getName() const;
    QString getCreationDate() const;
    QString getEditedDate() const;
    QString getDefaultPolicyIN() const;
    QString getDefaultPolicyOUT() const;

    /* Interfaces */
    QString getInterface(int index) const;
    void addInterface(QString name);
    int getInterfaceCount() const;

    /* Services */
    Service getService(int index) const;
    void addService(QString name, int port, QString protocol, QString action);
    void editService(int index, QString name, int port, QString protocol, QString action);
    QString getAllServices();
    int getServiceCount() const;
    void initializeServices(); // delete this


    /* Rules */
    Rule getRule(int index) const;
};

#endif // PROFILE_H
