#ifndef SYSTEM_H
#define SYSTEM_H

#include "service.h"
#include "profile.h"
#include <QFile>

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
    void importCurrentProfile(QString name);
    void changeCurrentProfile(QString name);
    Profile* getCurrentProfile() const;
    std::vector<QString> getProfileNames() const;
    void saveCurrentProfile(std::vector<QString> interfaces, QString defaultPolicyIN, QString defaultPolicyOUT);
    void start();
    void stop();
    bool getStatus() const;
    bool getFirstTimeUse() const;
    void createProfile(QString name, std::vector<QString> interfaces, QString defaultPolicyIN, QString defaultPolicyOUT);
};

#endif // SYSTEM_H
