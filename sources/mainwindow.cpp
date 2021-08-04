#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "system.h"
#include <QDateTime>
#include <QStandardItemModel>
#include <QAbstractItemView>
#include <QDialog>
#include <QStringList>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>

#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QSound>

QString mediadir = "./media/";
//easier to use pkexec than running app as root for security
//system("pkexec mkdir /bin/myDir");
//run shellscript to set firewall rules
// chmod the log file to userland because its pointless without watching it anyway ?

/* Intializing the system class as "firewall" */
System firewall;

/* CONSTRUCTOR */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

        QFile stylesheet("./Resource/themes/qdarkstyle/qdarkstyle.qss");
        stylesheet.open(QFile::ReadOnly);
        this->setStyleSheet(stylesheet.readAll());
        stylesheet.close();

    QPixmap oPixmap(32,32);
    oPixmap.load ( mediadir + "smoking.png");

    QIcon oIcon( oPixmap );

    trayIcon = new QSystemTrayIcon(oIcon);

    QAction *quit_action = new QAction( "Exit", trayIcon );
    connect( quit_action, SIGNAL(triggered()), this, SLOT(on_exit()) );

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction( quit_action );

    trayIcon->setContextMenu( trayIconMenu);
    trayIcon->setVisible(true);
    //trayIcon->showMessage("Test Message", "Text", QSystemTrayIcon::Information, 1000);
    //trayIcon->show();


    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

 QSound::play( mediadir + "phone.wav");

    /* Setting status button and text accordingly */
    if (firewall.getStatus() == false) {
        ui->pushButton_startStop->setText("Start");
        ui->statusLabel->setText("Firewall is OFF");
    } else {
        ui->pushButton_startStop->setText("Stop");
        ui->statusLabel->setText("Firewall is ON");
    }

    /* Loading profiles if there are any, otherwise load firstTimeUse mode */
    if (firewall.getFirstTimeUse() == false) {
        this->refreshAllTabs();
    } else {
        this->firstTimeUse();
    }
}

/* DESTRUCTOR */
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showMessage()
{
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon();
    trayIcon->showMessage(tr("QSatisfy"), tr("Will you smoke now..."), icon, 100);
}
void MainWindow::on_exit()
{
    this->close();
    QApplication::quit();
}

void MainWindow::refreshAllTabs()
{
    this->refreshProfileTab();
    this->refreshServiceTab();
    this->refreshCustomRulesTab();
}

/* Function: Initializing the current profile */
void MainWindow::refreshProfileTab()
{
    /* Importing currentProfile */
    firewall.updateProfileInfo(firewall.getCurrentProfile()->getName());
    firewall.updateProfileInterfaces(firewall.getCurrentProfile()->getName());

    /* Displaying current profile */
    ui->label_welcomeMessage->setText("Current profile:");
    ui->label_currentProfileName->setText(firewall.getCurrentProfile()->getName());

    //Set defaultPolicyIN
    if (firewall.getCurrentProfile()->getDefaultPolicyIN() == "accept")
        ui->comboBox_defaultPolicyIN_edit->setCurrentIndex(0);
    else if (firewall.getCurrentProfile()->getDefaultPolicyIN() == "drop")
        ui->comboBox_defaultPolicyIN_edit->setCurrentIndex(1);
    else if (firewall.getCurrentProfile()->getDefaultPolicyIN() == "reject")
        ui->comboBox_defaultPolicyIN_edit->setCurrentIndex(2);

    //set defaultPolicyOUT
    if (firewall.getCurrentProfile()->getDefaultPolicyOUT() == "accept")
        ui->comboBox_defaultPolicyOUT_edit->setCurrentIndex(0);
    else if (firewall.getCurrentProfile()->getDefaultPolicyOUT() == "drop")
        ui->comboBox_defaultPolicyOUT_edit->setCurrentIndex(1);
    else if (firewall.getCurrentProfile()->getDefaultPolicyOUT() == "reject")
        ui->comboBox_defaultPolicyOUT_edit->setCurrentIndex(2);

    //set createdDate and editedDate
    ui->label_createdDate->setText(firewall.getCurrentProfile()->getCreationDate());
    ui->label_editedDate->setText(firewall.getCurrentProfile()->getEditedDate());

    /* (Re)Filling interfaces list */
    ui->listWidget_interfaces_edit->clear();
    for (int i=0;i<firewall.getCurrentProfile()->getInterfaceCount();i++) {
        firewall.getCurrentProfile()->getInterface(i);
        QListWidgetItem* interface = new QListWidgetItem(QString(firewall.getCurrentProfile()->getInterface(i)));
        ui->listWidget_interfaces_edit->addItem(interface);
    }

    /* (Re)Filling the change profile comboBox */
    int nrOfProfiles = firewall.getProfileNames().size();
    ui->comboBox_changeProfileList->clear();
    for (int i=0;i<nrOfProfiles;i++)
        ui->comboBox_changeProfileList->addItem(firewall.getProfileNames().at(i));

    //..and setting the currentIndex to our currentProfile's index
    for (int i=0;i<nrOfProfiles;i++) {
        if (firewall.getProfileNames().at(i) == firewall.getCurrentProfile()->getName()) {
            ui->comboBox_changeProfileList->setCurrentIndex(i);
            break;
        }
    }

    ui->comboBox_interface->clear();
    ui->comboBox_interface->addItem("Any");
    for (int i=0;i<firewall.getCurrentProfile()->getInterfaceCount();i++)
        ui->comboBox_interface->addItem(firewall.getCurrentProfile()->getInterface(i));

    ui->pushButton_saveProfile->setEnabled(false);
    ui->pushButton_saveProfile_undo->setEnabled(false);
    ui->label_changesDetected->setText("");
}

void MainWindow::refreshServiceTab()
{
    firewall.updateProfileServices(firewall.getCurrentProfile()->getName());
    this->fillServiceRuleTable();
}

void MainWindow::refreshCustomRulesTab()
{
    QString pathString;
    QTextStream pathStream(&pathString);
    pathStream << firewall.getSystemPath() << "/profiles/" << firewall.getCurrentProfile()->getName() << "-customRules.txt";
    QFile customRules(pathString);
    customRules.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&customRules);

    ui->plainTextEdit_customRules->setPlainText(in.readAll());

    customRules.close();

    ui->label_customRulesFooter->setText("");
    ui->pushButton_reloadCustomRules->setEnabled(false);
}

/* Function: Initializing first time use (no profile loaded) */
void MainWindow::firstTimeUse(bool trueOrFalse)
{
    /* Setting title */
    if (trueOrFalse == true) {
        ui->label_welcomeMessage->setText("Welcome to iptables Made Simple!");
        ui->label_currentProfileName->setText("Please create your first profile.");
    }

    /* Disabling the tabs */
    ui->tab_services->setDisabled(trueOrFalse);
    ui->tab_rules->setDisabled(trueOrFalse);
    ui->tab_custom->setDisabled(trueOrFalse);
    ui->tab_logs->setDisabled(trueOrFalse);

    /* Enabling the "add new profile" fields */
    ui->label_editActiveProfile->setDisabled(trueOrFalse);
    ui->label_defaultPolicyIN_edit->setDisabled(trueOrFalse);
    ui->label_defaultPolicyOUT_edit->setDisabled(trueOrFalse);
    ui->comboBox_defaultPolicyIN_edit->setDisabled(trueOrFalse);
    ui->comboBox_defaultPolicyOUT_edit->setDisabled(trueOrFalse);
    ui->label_created->setDisabled(trueOrFalse);
    ui->label_createdDate->setDisabled(trueOrFalse);
    ui->label_lastEdited->setDisabled(trueOrFalse);
    ui->label_editedDate->setDisabled(trueOrFalse);
    ui->lineEdit_newInterfaceName_edit->setDisabled(trueOrFalse);
    ui->pushButton_addInterface_edit->setDisabled(trueOrFalse);
    ui->pushButton_addInterface_edit_delete->setDisabled(trueOrFalse);
    ui->pushButton_saveProfile->setDisabled(trueOrFalse);
}

/* Function: Fill the service rule table */
void MainWindow::fillServiceRuleTable()
{
    /* Define number of columns and rows */
    ui->tableWidget_serviceRules->setColumnCount(4);
    ui->tableWidget_serviceRules->setRowCount(firewall.getCurrentProfile()->getServiceCount());

    /* Setting the header labels */
    QStringList columnHeaders;
    columnHeaders << "Name" << "Port" << "Protocol" << "Action";
    ui->tableWidget_serviceRules->setHorizontalHeaderLabels(columnHeaders);

    /* Filling the table */
    for(int i=0;i<firewall.getCurrentProfile()->getServiceCount();i++) {

        /* Defining the port SpinBox */
        QSpinBox *portBox = new QSpinBox;
        portBox->setMinimum(1);
        portBox->setMaximum(65535);
        portBox->setValue(firewall.getCurrentProfile()->getService(i)->getPort());

        /* Defining the protocol ComboBox */
        QComboBox *protocolBox = new QComboBox;
        QStringList protocols;
        protocols << "tcp" << "udp" << "tcp/udp";
        protocolBox->addItems(protocols);
        if (firewall.getCurrentProfile()->getService(i)->getProtocol() == "tcp")
            protocolBox->setCurrentIndex(0);
        else if (firewall.getCurrentProfile()->getService(i)->getProtocol() == "udp")
            protocolBox->setCurrentIndex(1);
        else
            protocolBox->setCurrentIndex(2);

        /* Defining the action ComboBox */
        QComboBox *actionBox = new QComboBox;
        QStringList actions;
        actions << "drop" << "accept" << "reject";
        actionBox->addItems(actions);
        if (firewall.getCurrentProfile()->getService(i)->getAction() == "drop")
            actionBox->setCurrentIndex(0);
        else if (firewall.getCurrentProfile()->getService(i)->getAction() == "accept")
            actionBox->setCurrentIndex(1);
        else
            actionBox->setCurrentIndex(2);

        /* Filling */
        QTableWidgetItem* name = new QTableWidgetItem(QString(firewall.getCurrentProfile()->getService(i)->getName()));
        QWidget* port = portBox;
        QWidget* protocol = protocolBox;
        QWidget* action = actionBox;

        ui->tableWidget_serviceRules->setItem(i,0,name);
        ui->tableWidget_serviceRules->setCellWidget(i,1,port);
        ui->tableWidget_serviceRules->setCellWidget(i,2,protocol);
        ui->tableWidget_serviceRules->setCellWidget(i,3,action);
    }
}

/* Function: fill the defineRules table */
void MainWindow::fillDefineRulesTable()
{
    ui->tableWidget_defineRules->setRowCount(0);
}

/* pushButton: Start/Stop firewall */
void MainWindow::on_pushButton_startStop_clicked()
{
    if(firewall.getStatus() == false) {
        firewall.start();
        if (firewall.getStatus() == true) {
            ui->pushButton_startStop->setText("Stop");
            ui->statusLabel->setText("Firewall is ON");
            system("pkexec ./scripts/firewall");
            //set options to install systemd script
          //  system("pkexec cp ./scripts/firewall2.service /etc/systemd/system/");
        } else
            ui->statusLabel->setText("Start FAILED!");
    } else {
        firewall.stop();
        if (firewall.getStatus() == false) {
            ui->pushButton_startStop->setText("Start");
            ui->statusLabel->setText("Firewall is OFF");
            system("pkexec ./scripts/firewall-off");
        } else
            ui->statusLabel->setText("Stop FAILED!");
    }
}

/* pushButton: Reload custom rules */
void MainWindow::on_pushButton_reloadCustomRules_clicked()
{
    QString pathString;
    QTextStream pathStream(&pathString);
    pathStream << firewall.getSystemPath() << "/profiles/" << firewall.getCurrentProfile()->getName() << "-customRules.txt";
    QFile customRulesFile(pathString);
    customRulesFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&customRulesFile);
    out << ui->plainTextEdit_customRules->toPlainText();
    customRulesFile.close();
    ui->label_customRulesFooter->setText("Custom rules has been reloaded!");
    ui->pushButton_reloadCustomRules->setEnabled(false);
}

/* plainTextEdit: Custom rules ON textChanged */
void MainWindow::on_plainTextEdit_customRules_textChanged()
{
    ui->label_customRulesFooter->setText("Field has been modified. Press reload to activate changes.");
    ui->pushButton_reloadCustomRules->setEnabled(true);
}

/* pushButton: Clear new service rule */
void MainWindow::on_pushButton_newServiceRuleClear_clicked() //
{
    ui->lineEdit_newServiceRuleName->setText("Name");
    ui->spinBox_newServiceRulePort->setValue(0);
    ui->comboBox_newServiceRuleProtocol->setCurrentIndex(0);
    ui->comboBox_newServiceRuleAction->setCurrentIndex(0);
    ui->label_newServiceRuleFooter->clear();
}

/* pushButton: Add new service rule */
void MainWindow::on_pushButton_newServiceRuleAdd_clicked()
{
    if (ui->lineEdit_newServiceRuleName->text() == "Name")
        ui->label_newServiceRuleFooter->setText("ERROR: Please enter a valid service name.");
    else if (ui->spinBox_newServiceRulePort->value() == 0)
        ui->label_newServiceRuleFooter->setText("ERROR: Please enter a valid port number.");
    else if (ui->comboBox_newServiceRuleProtocol->currentIndex() == 0)
        ui->label_newServiceRuleFooter->setText("ERROR: Please choose a protocol.");
    else if (ui->comboBox_newServiceRuleAction->currentIndex() == 0)
        ui->label_newServiceRuleFooter->setText("ERROR: Please choose an action.");
    else { // Adding it to the tableWidget
        /* Defining the port SpinBox */
        QSpinBox *portBox = new QSpinBox;
        portBox->setMinimum(1);
        portBox->setMaximum(65535);
        portBox->setValue(ui->spinBox_newServiceRulePort->value());

        /* Defining the protocol ComboBox */
        QComboBox *protocolBox = new QComboBox;
        QStringList protocols;
        protocols << "tcp" << "udp" << "tcp/udp";
        protocolBox->addItems(protocols);
        protocolBox->setCurrentIndex(ui->comboBox_newServiceRuleProtocol->currentIndex()-1);

        /* Defining the action ComboBox */
        QComboBox *actionBox = new QComboBox;
        QStringList actions;
        actions << "drop" << "accept" << "reject";
        actionBox->addItems(actions);
        actionBox->setCurrentIndex(ui->comboBox_newServiceRuleAction->currentIndex()-1);

        /* Filling */
        QTableWidgetItem* name = new QTableWidgetItem(QString(ui->lineEdit_newServiceRuleName->text()));
        QWidget* port = portBox;
        QWidget* protocol = protocolBox;
        QWidget* action = actionBox;
        int rowNr = ui->tableWidget_serviceRules->rowCount();
        ui->tableWidget_serviceRules->insertRow(rowNr);
        ui->tableWidget_serviceRules->setItem(rowNr,0,name);
        ui->tableWidget_serviceRules->setCellWidget(rowNr,1,port);
        ui->tableWidget_serviceRules->setCellWidget(rowNr,2,protocol);
        ui->tableWidget_serviceRules->setCellWidget(rowNr,3,action);
    }
}

/* pushButton: Save services */
void MainWindow::on_pushButton_saveServices_clicked()
{
    firewall.getCurrentProfile()->flushServices();
    for (int i=0;i<ui->tableWidget_serviceRules->rowCount();i++) {
        QString name = ui->tableWidget_serviceRules->item(i,0)->text();
        /* Casting our objects from QTableWidgetItem to QComboBox/QSpinBox */
        QSpinBox* port = qobject_cast<QSpinBox*>(ui->tableWidget_serviceRules->cellWidget(i,1));
        QComboBox* protocol = qobject_cast<QComboBox*>(ui->tableWidget_serviceRules->cellWidget(i,2));
        QComboBox* action = qobject_cast<QComboBox*>(ui->tableWidget_serviceRules->cellWidget(i,3));

        /* Passing our typecasted objects as parameters to editService function */
        firewall.getCurrentProfile()->addService(name, port->value(), protocol->currentText(), action->currentText());
    }
    firewall.saveServices();
    ui->label_editStatus->setText("Changes has been saved.");
}

/* pushButton: Undo service changes - refill table */
void MainWindow::on_pushButton_undoServices_clicked()
{
    this->fillServiceRuleTable();
    ui->label_editStatus->setText("");
}

/* pushButton: Create Profile */
void MainWindow::on_pushButton_createProfile_clicked()
{
    if (ui->lineEdit_newProfileName->text() != "" && ui->lineEdit_newProfileName->text() != "Enter name here") {
        std::vector<QString> interfaces;
        int nrOfInterfaces = ui->listWidget_interfaces->count();
        for (int i=0;i<nrOfInterfaces;i++)
            interfaces.push_back(ui->listWidget_interfaces->item(i)->text());

        bool firstProfile = false;
        if (firewall.getFirstTimeUse() == true)
            firstProfile = true;

        firewall.createProfile(
            ui->lineEdit_newProfileName->text(),
            interfaces,
            ui->comboBox_defaultPolicyIN->currentText(),
            ui->comboBox_defaultPolicyOUT->currentText()
        );

        ui->lineEdit_newProfileName->setText("");
        ui->comboBox_defaultPolicyIN->setCurrentIndex(0);
        ui->comboBox_defaultPolicyOUT->setCurrentIndex(0);
        ui->listWidget_interfaces->clear();

        if (firstProfile == true) {
            this->refreshAllTabs();
            this->firstTimeUse(false);
        } else {
            //Re-filling the change profile comboBox
            ui->comboBox_changeProfileList->clear();
            int nrOfProfiles = firewall.getProfileNames().size();
            for (int i=0;i<nrOfProfiles;i++)
                ui->comboBox_changeProfileList->addItem(firewall.getProfileNames().at(i));
        }

    } else {
        ui->lineEdit_newProfileName->setText("Enter name here");
    }
}

/* pushButton: Save Current Profile */
void MainWindow::on_pushButton_saveProfile_clicked()
{
    std::vector<QString> interfaces;
    int nrOfInterfaces = ui->listWidget_interfaces_edit->count();
    for (int i=0;i<nrOfInterfaces;i++)
        interfaces.push_back(ui->listWidget_interfaces_edit->item(i)->text());

    firewall.saveCurrentProfile(
        interfaces,
        ui->comboBox_defaultPolicyIN_edit->currentText(),
        ui->comboBox_defaultPolicyOUT_edit->currentText()
    );

    ui->pushButton_saveProfile->setEnabled(false);
    ui->pushButton_saveProfile_undo->setEnabled(false);
    ui->label_changesDetected->setText("");
    ui->listWidget_interfaces_edit->clear();
    this->refreshProfileTab();
}

/* pushButton: Add interface - create */
void MainWindow::on_pushButton_addInterface_clicked()
{
    if (ui->lineEdit_newInterfaceName->text() != "") {
        QListWidgetItem* interface = new QListWidgetItem(QString(ui->lineEdit_newInterfaceName->text()));
        ui->listWidget_interfaces->addItem(interface);
        ui->lineEdit_newInterfaceName->setText("");
    }
}

/* pushButton: Add interface - edit current */
void MainWindow::on_pushButton_addInterface_edit_clicked()
{
    if (ui->lineEdit_newInterfaceName_edit->text() != "") {
        QListWidgetItem* interface = new QListWidgetItem(QString(ui->lineEdit_newInterfaceName_edit->text()));
        ui->listWidget_interfaces_edit->addItem(interface);
        ui->lineEdit_newInterfaceName_edit->setText("");
    }
    ui->pushButton_saveProfile->setEnabled(true);
    ui->pushButton_saveProfile_undo->setEnabled(true);
    ui->label_changesDetected->setText("Changes detected!");
}

/* comboBox: defaultPolicyIN_edit - indexChanged */
void MainWindow::on_comboBox_defaultPolicyIN_edit_currentIndexChanged(const QString &arg1)
{
    (void)arg1;
    ui->pushButton_saveProfile->setEnabled(true);
    ui->pushButton_saveProfile_undo->setEnabled(true);
    ui->label_changesDetected->setText("Changes detected!");
}

/* comboBox: defaultPolicyOUT_edit - indexChanged */
void MainWindow::on_comboBox_defaultPolicyOUT_edit_currentIndexChanged(const QString &arg1)
{
    (void)arg1;
    ui->pushButton_saveProfile->setEnabled(true);
    ui->pushButton_saveProfile_undo->setEnabled(true);
    ui->label_changesDetected->setText("Changes detected!");
}

/* pushButton: Delete interface - edit current profile */
void MainWindow::on_pushButton_addInterface_edit_delete_clicked()
{
    delete ui->listWidget_interfaces_edit->currentItem();
    ui->pushButton_saveProfile->setEnabled(true);
    ui->pushButton_saveProfile_undo->setEnabled(true);
    ui->label_changesDetected->setText("Changes detected!");
}

/* pushButton: Undo changes to current profile */
void MainWindow::on_pushButton_saveProfile_undo_clicked()
{
    ui->pushButton_saveProfile->setEnabled(false);
    ui->pushButton_saveProfile_undo->setEnabled(false);
    ui->label_changesDetected->setText("");
    ui->listWidget_interfaces_edit->clear();
    this->refreshProfileTab();
}

void MainWindow::on_pushButton_changeProfile_clicked()
{
    int tmpIndex = ui->comboBox_changeProfileList->currentIndex();
    firewall.changeCurrentProfile(ui->comboBox_changeProfileList->currentText());
    ui->comboBox_changeProfileList->setCurrentIndex(tmpIndex);

    this->refreshProfileTab();
    this->refreshServiceTab();
}

void MainWindow::on_pushButton_removeSelectedServices_clicked()
{
    ui->tableWidget_serviceRules->removeRow(ui->tableWidget_serviceRules->currentRow());
}

void MainWindow::on_checkBox_sIP_stateChanged(int arg1)
{
    if (arg1 == 2)
        arg1 = 1;
    ui->lineEdit_sIP->setEnabled(arg1);
}

void MainWindow::on_checkBox_sPort_stateChanged(int arg1)
{
    if (arg1 == 2)
        arg1 = 1;
    ui->spinBox_sPort->setEnabled(arg1);
}

void MainWindow::on_checkBox_dIP_stateChanged(int arg1)
{
    if (arg1 == 2)
        arg1 = 1;
    ui->lineEdit_dIP->setEnabled(arg1);
}

void MainWindow::on_checkBox_dPort_stateChanged(int arg1)
{
    if (arg1 == 2)
        arg1 = 1;
    ui->spinBox_dPort->setEnabled(arg1);
}

void MainWindow::on_checkBox_connectionAmount_stateChanged(int arg1)
{
    if (arg1 == 2)
        arg1 = 1;
    ui->label_maxActive->setEnabled(arg1);
    ui->spinBox_maxActive->setEnabled(arg1);
}

void MainWindow::on_checkBox_connectionRate_stateChanged(int arg1)
{
    if (arg1 == 2)
        arg1 = 1;
    ui->label_perSecond->setEnabled(arg1);
    ui->spinBox_perSecond->setEnabled(arg1);
}

void MainWindow::on_pushButton_addRule_clicked()
{
    if (ui->lineEdit_ruleAlias->text() == "" || ui->lineEdit_ruleAlias->text() == "Alias")
        ui->label_addRuleError->setText("ERROR: Invalid alias");
    else {
        ui->tableWidget_defineRules->setRowCount(ui->tableWidget_defineRules->rowCount()+1);
        QTableWidgetItem* name = new QTableWidgetItem(QString("asd"));
        ui->tableWidget_defineRules->setItem(0,0,name);
        ui->label_addRuleError->setText("");
    }
}

void MainWindow::on_checklogbtn_clicked()
{
    //system("pkexec 'cat /var/log/firewall | '");
}
