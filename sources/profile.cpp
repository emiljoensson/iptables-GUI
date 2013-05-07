#include "profile.h"

Profile::Profile(QString name, QString defaultPolicyIN, QString defaultPolicyOUT, QString createdDate, QString editedDate)
{
    this->name = name;
    this->defaultPolicyIN = defaultPolicyIN;
    this->defaultPolicyOUT = defaultPolicyOUT;
    this->createdDate = createdDate;
    this->editedDate = editedDate;
}

Profile::~Profile()
{
    this->flushServices();
    this->flushRules();
}

QString Profile::getSystemTime() const
{
    QDateTime tmp;
    return tmp.currentDateTime().toString("yyyy-MM-dd hh:mm");
}

QString Profile::getName() const
{
    return this->name;
}

QString Profile::getCreationDate() const
{
    return this->createdDate;
}

QString Profile::getEditedDate() const
{
    return this->editedDate;
}

QString Profile::getDefaultPolicyIN() const
{
    return this->defaultPolicyIN;
}

QString Profile::getDefaultPolicyOUT() const
{
    return this->defaultPolicyOUT;
}

QString Profile::getInterface(int index) const
{
    return this->interfaces.at(index);
}

void Profile::addInterface(QString name)
{
    this->interfaces.push_back(name);
}

int Profile::getInterfaceCount() const
{
    return this->interfaces.size();
}

Service* Profile::getService(int index) const
{
    return this->services.at(index);
}

void Profile::addService(QString name, int port, QString protocol, QString action)
{
    this->services.push_back(new Service(name, port, protocol, action));
}

void Profile::flushServices()
{
    int nrOfServices = this->services.size();
    for (int i=0;i<nrOfServices;i++)
        delete this->services.at(i);
    this->services.clear();
}

QString Profile::serviceToString(int index) const
{
    QString returnVal;
    QTextStream out(&returnVal);
    out << static_cast<Service*>(this->services.at(index))->getName() << ",";
    out << static_cast<Service*>(this->services.at(index))->getPort() << ",";
    out << static_cast<Service*>(this->services.at(index))->getProtocol() << ",";
    out << static_cast<Service*>(this->services.at(index))->getAction() << "\n";
    return returnVal;

}

int Profile::getServiceCount() const
{
    return this->services.size();
}

void Profile::flushRules()
{
    int nrOfRules = this->rules.size();
    for (int i=0;i<nrOfRules;i++)
        delete this->rules.at(i);
    this->rules.clear();
}
