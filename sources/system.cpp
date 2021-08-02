#include "system.h"

System::System()
{
    this->systemPath = "/home/helga/Desktop/firewall"; //System path without trailing slash
    this->status = false;
    this->currentProfile = NULL;

    //Opening the profileList file to check for profiles
    QFile profileList("/home/helga/Desktop/firewall/profileList.txt");  //TEMP: CHANGE PATH
    profileList.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&profileList);

    //Get the amount of profiles, stored in first line of the file
    int nrOfProfiles = in.readLine().toInt();
    QString currentProfileName;

    //Checking if we have any profiles or not

    if (nrOfProfiles == 0) { //If there are no profiles
        this->firstTimeUse = true;
    } else if (nrOfProfiles > 0) { //If we have at least 1 profile
        this->firstTimeUse = false;

        currentProfileName = in.readLine(); //Reading currentProfile from line 2

        //Get the names for the profiles
        for (int i=1;i<=nrOfProfiles;i++) {
            this->profileNames.push_back(in.readLine());
        }

    } else { //If profileList failed to load correctly
        this->profileNames.push_back("ERROR-LOADING-PROFILES");
    }

    profileList.close();

    if (nrOfProfiles > 0)
        this->changeCurrentProfile(currentProfileName);
}

System::~System() {
    delete this->currentProfile;
}

void System::changeCurrentProfile(QString name)
{
    // Calling update info function
    this->updateProfileInfo(name);

    // Calling update interfaces function
    this->updateProfileInterfaces(name);

    // Calling update services function
    this->updateProfileServices(name);

    //Rebuilding the profileList so that the current one is on top
    QString pathString;
    QTextStream pathStream(&pathString);
    pathStream << this->systemPath << "/profileList.txt";
    QFile profileList(pathString);
    profileList.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&profileList);
    out << this->profileNames.size() << "\n";
    out << this->currentProfile->getName() << "\n";

    for (unsigned int i=0;i<this->profileNames.size();i++)
        out << this->profileNames.at(i) << "\n";

    profileList.close();
}

void System::updateProfileInfo(QString name)
{
    QString pathString;
    QTextStream pathStream(&pathString);
    pathStream << this->systemPath << "/profiles/" << name << ".txt";
    QFile profileList(pathString);
    profileList.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&profileList);
    //if (this->currentProfile == NULL)
        delete this->currentProfile;
    this->currentProfile = new Profile(in.readLine(), in.readLine(), in.readLine(), in.readLine(), in.readLine());
    profileList.close();
}

void System::updateProfileInterfaces(QString name)
{
    QString pathString2;
    QTextStream pathStream2(&pathString2);
    pathStream2 << this->systemPath << "/profiles/" << name << "-interfaces.txt";
    QFile currentProfileInterfaces(pathString2);
    currentProfileInterfaces.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in2(&currentProfileInterfaces);
    int nrOfInterfaces = in2.readLine().toInt();
    for (int i=0;i<nrOfInterfaces;i++)
        this->currentProfile->addInterface(in2.readLine());
    currentProfileInterfaces.close();
}

void System::updateProfileServices(QString name)
{
    QString pathString3;
    QTextStream pathStream3(&pathString3);
    pathStream3 << this->systemPath << "/profiles/" << name << "-services.txt";
    QFile currentProfileServices(pathString3);
    currentProfileServices.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in3(&currentProfileServices);

    QString serviceName, serviceProtocol, serviceAction, servicePort;
    int varCount=0;
    while (in3.atEnd() == false) {
        QString line = in3.readLine(); // contains several lines of text
        QStringList service = line.split(",", QString::SkipEmptyParts);
        foreach (QString var, service) {
            if ((varCount % 4) == 0)
                serviceName = var;
            else if ((varCount % 4) == 1)
                servicePort = var;
            else if ((varCount % 4) == 2)
                serviceProtocol = var;
            else if ((varCount % 4) == 3)
                serviceAction = var;
            varCount++;
        }
        this->currentProfile->addService(serviceName, servicePort.toInt(), serviceProtocol, serviceAction);
    }
    currentProfileServices.close();
}

Profile* System::getCurrentProfile() const
{
    return this->currentProfile;
}

std::vector<QString> System::getProfileNames() const
{
    return this->profileNames;
}

void System::saveCurrentProfile(std::vector<QString> interfaces, QString defaultPolicyIN, QString defaultPolicyOUT)
{
    //Writing the changes to the profile file
    QString pathString;
    QTextStream pathStream(&pathString);
    pathStream << this->systemPath << "/profiles/" << this->currentProfile->getName() << ".txt";
    QFile currentProfile(pathString);
    currentProfile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out1(&currentProfile);
    out1 << this->currentProfile->getName() << "\n";
    out1 << defaultPolicyIN << "\n";
    out1 << defaultPolicyOUT << "\n";
    out1 << this->currentProfile->getCreationDate() << "\n";
    out1 << this->currentProfile->getSystemTime() << "\n";
    currentProfile.close();

    //Writing the changes of interfaces to the profile's interface file
    QString pathString2;
    QTextStream pathStream2(&pathString2);
    pathStream2 << this->systemPath << "/profiles/" << this->currentProfile->getName() << "-interfaces.txt";
    QFile currentProfileInterfaces(pathString2);
    currentProfileInterfaces.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out2(&currentProfileInterfaces);
    out2 << interfaces.size() << "\n";
    for (unsigned int i=0;i<interfaces.size();i++)
        out2 << interfaces.at(i) << "\n";
    currentProfileInterfaces.close();

    //Reload the currentProfile with the new changes
    this->updateProfileInfo(this->currentProfile->getName());
}

void System::saveServices()
{
    QString pathString;
    QTextStream pathStream(&pathString);
    pathStream << this->getSystemPath() << "/profiles/" << this->currentProfile->getName() << "-services.txt";
    QFile serviceRulesFile(pathString);
    serviceRulesFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&serviceRulesFile);
    for (int i=0;i<this->getCurrentProfile()->getServiceCount();i++)
        out << this->getCurrentProfile()->serviceToString(i);
    serviceRulesFile.close();
}

void System::start()
{
    //do the start thingie
    this->status = true;
}

void System::stop()
{
    //do the stop thingie
    this->status = false;
}

bool System::getStatus() const
{
    return this->status;
}

bool System::getFirstTimeUse() const
{
    return this->firstTimeUse;
}

QString System::getSystemPath() const
{
    return this->systemPath;
}

void System::createProfile(QString name, std::vector<QString> interfaces, QString defaultPolicyIN, QString defaultPolicyOUT)
{
    //Writing the profile info to the profile file
    QString pathString;
    QTextStream pathStream(&pathString);
    pathStream << this->systemPath << "/profiles/" << name << ".txt";
    QFile newProfile(pathString);
    newProfile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out1(&newProfile);
    out1 << name << "\n";
    out1 << defaultPolicyIN << "\n";
    out1 << defaultPolicyOUT << "\n";
    out1 << this->currentProfile->getSystemTime() << "\n";
    out1 << this->currentProfile->getSystemTime() << "\n";
    newProfile.close();

    //Adding it to profileNames vector
    this->profileNames.push_back(name);

    //Writing the interfaces to the profile's interface file
    QString pathString2;
    QTextStream pathStream2(&pathString2);
    pathStream2 << this->systemPath << "/profiles/" << name << "-interfaces.txt";
    QFile newProfileInterfaces(pathString2);
    newProfileInterfaces.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out2(&newProfileInterfaces);

    out2 << interfaces.size() << "\n";
    for (unsigned int i=0;i<interfaces.size();i++)
        out2 << interfaces.at(i) << "\n";

    newProfileInterfaces.close();

    //Writing the default services to profile's services file
    QString pathString3;
    QTextStream pathStream3(&pathString3);
    pathStream3 << this->systemPath << "/profiles/" << name << "-services.txt";
    QFile newProfileServices(pathString3);
    newProfileServices.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out3(&newProfileServices);

    out3 << "FTP,21,tcp,drop\n";
    out3 << "FTP,21,tcp,drop\n";
    out3 << "SSH,22,tcp,accept\n";
    out3 << "Telnet,23,tcp,drop\n";
    out3 << "SMTP,25,tcp,drop\n";
    out3 << "DNS,53,tcp/udp,drop\n";
    out3 << "HTTP,80,tcp,accept\n";
    out3 << "HTTPS,443,tcp,drop\n";
    out3 << "NTP,123,udp,drop\n";
    out3 << "IMAP,143,tcp,drop\n";
    out3 << "SNMP,161,udp,drop\n";
    out3 << "SMB,445,tcp,drop\n";
    out3 << "PPTP,1723,tcp,drop\n";
    out3 << "MySQL,3306,tcp,drop\n";

    newProfileServices.close();

    //Wring profile list to profileList.txt
    QString pathString4;
    QTextStream pathStream4(&pathString4);
    pathStream4 << this->systemPath << "/profileList.txt";
    QFile profileList(pathString4);
    profileList.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out4(&profileList);

    int nrOfProfiles = this->profileNames.size();

    out4 << nrOfProfiles << "\n";

    if (this->firstTimeUse == true)
        out4 << name << "\n";
    else
        out4 << this->currentProfile->getName() << "\n";

    for (int i=0;i<nrOfProfiles;i++)
        out4 << this->profileNames.at(i) << "\n";

    profileList.close();

    if (this->firstTimeUse == true) {
        this->changeCurrentProfile(name);
        this->firstTimeUse = false;
    }
}
