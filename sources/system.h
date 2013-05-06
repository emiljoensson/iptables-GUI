#ifndef SYSTEM_H
#define SYSTEM_H

#include "service.h"
#include "profile.h"
#include <QFile>
#include <QStandardItem>

class System
{
private:
    QString systemPath = "/home/helga/Desktop/firewall"; //System path without trailing slash
    bool status;
    std::vector<QString> profileNames;
    Profile *currentProfile;
    bool firstTimeUse;
public:
    System();
    ~System();
    void changeCurrentProfile(QString name);
    void updateProfileInfo(QString name);
    void updateProfileInterfaces(QString name);
    void updateProfileServices(QString name);
    Profile* getCurrentProfile() const;
    std::vector<QString> getProfileNames() const;
    void saveCurrentProfile(std::vector<QString> interfaces, QString defaultPolicyIN, QString defaultPolicyOUT);
    void saveServices();
    void start();
    void stop();
    bool getStatus() const;
    bool getFirstTimeUse() const;
    QString getSystemPath() const;
    QVector<Service*> getDefaultServices();
    void createProfile(QString name, std::vector<QString> interfaces, QString defaultPolicyIN, QString defaultPolicyOUT);
};

#endif // SYSTEM_H
