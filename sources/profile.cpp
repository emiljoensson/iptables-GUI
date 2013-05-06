#include "profile.h"

Profile::Profile(QString name, QString defaultPolicyIN, QString defaultPolicyOUT, QString createdDate, QString editedDate)
{
    this->name = name;
    this->defaultPolicyIN = defaultPolicyIN;
    this->defaultPolicyOUT = defaultPolicyOUT;
    this->createdDate = createdDate;
    this->editedDate = editedDate;
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

Rule Profile::getRule(int index) const
{
    return *this->firewallRules.at(index);
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

Service Profile::getService(int index) const
{
    return *this->services.at(index);
}

void Profile::addService(QString name, int port, QString protocol, QString action)
{
    this->services.push_back(new Service(name, port, protocol, action));
}

void Profile::editService(int index, QString name, int port, QString protocol, QString action)
{
    this->services.at(index) = (new Service(name, port, protocol, action));
}

QString Profile::getAllServices()
{
    QString allServices;
    Service* tmp;
    QTextStream out(&allServices);
    int size = services.size();
    for (int i=0;i<size;i++) {
        tmp = this->services.at(i);
        out << tmp->getName() << "," << tmp->getPort() << "," << tmp->getProtocol() << "," << tmp->getAction() << "\n";
    }
    return allServices;
}

int Profile::getServiceCount() const
{
    return this->services.size();
}

void Profile::initializeServices()
{
    this->services.push_back(new Service("FTP", 21, "tcp", "drop"));
    this->services.push_back(new Service("SSH", 22, "tcp", "accept"));
    this->services.push_back(new Service("Telnet", 23, "tcp", "drop"));
    this->services.push_back(new Service("SMTP", 25, "tcp", "drop"));
    this->services.push_back(new Service("DNS", 53, "tcp/udp", "drop"));
    this->services.push_back(new Service("HTTP", 80, "tcp", "accept"));
    this->services.push_back(new Service("HTTPS", 443, "tcp", "drop"));
    this->services.push_back(new Service("NTP", 123, "udp", "drop"));
    this->services.push_back(new Service("IMAP", 143, "tcp", "drop"));
    this->services.push_back(new Service("SNMP", 161, "udp", "drop"));
    this->services.push_back(new Service("SMB", 445, "tcp", "drop"));
    this->services.push_back(new Service("PPTP", 1723, "tcp", "drop"));
    this->services.push_back(new Service("MySQL", 3306, "tcp", "drop"));
}
