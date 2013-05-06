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

/* Intializing the system class as "firewall" */
System firewall;

/* CONSTRUCTOR */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Setting status button and text accordingly */
    if (firewall.getStatus() == false) {
        ui->pushButton_startStop->setText("Start");
        ui->statusLabel->setText("Firewall is OFF");
    } else {
        ui->pushButton_startStop->setText("Stop");
        ui->statusLabel->setText("Firewall is ON");
    }

    /* Loading profiles if there are any, otherwise load firstTimeUse mode */
    if (firewall.getFirstTimeUse() == FALSE) {
        this->loadCurrentProfile();
    } else {
        this->firstTimeUse();
    }
}

/* DESTRUCTOR */
MainWindow::~MainWindow()
{
    delete ui;
}

/* Function: Initializing the current profile */
void MainWindow::loadCurrentProfile()
{
    /* Importing currentProfile */
    firewall.importCurrentProfile(firewall.getCurrentProfile()->getName());

    /* Filling the service rule and defineRules table */
    this->fillServiceRuleTable();
    this->fillDefineRulesTale();

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
    ui->comboBox_changeProfileList->clear();
    for (int i=0;i<firewall.getProfileNames().size();i++)
        ui->comboBox_changeProfileList->addItem(firewall.getProfileNames().at(i));

    //..and setting the currentIndex to our currentProfile's index
    for (int i=0;i<firewall.getProfileNames().size();i++) {
        if (firewall.getProfileNames().at(i) == firewall.getCurrentProfile()->getName()) {
            ui->comboBox_changeProfileList->setCurrentIndex(i);
            break;
        }
    }

    ui->pushButton_saveProfile->setEnabled(FALSE);
    ui->pushButton_saveProfile_undo->setEnabled(FALSE);
    ui->label_changesDetected->setText("");
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
    columnHeaders << "Name" << "Port" << "Protocol" << "Action" << "";
    ui->tableWidget_serviceRules->setHorizontalHeaderLabels(columnHeaders);

    /* Filling the table */
    for(int i=0;i<firewall.getCurrentProfile()->getServiceCount();i++) {

        /* Defining the port SpinBox */
        QSpinBox *portBox = new QSpinBox;
        portBox->setMinimum(1);
        portBox->setMaximum(65535);
        portBox->setValue(firewall.getCurrentProfile()->getService(i).getPort());

        /* Defining the protocol ComboBox */
        QComboBox *protocolBox = new QComboBox;
        QStringList protocols;
        protocols << "tcp" << "udp" << "tcp/udp";
        protocolBox->addItems(protocols);
        if (firewall.getCurrentProfile()->getService(i).getProtocol() == "tcp")
            protocolBox->setCurrentIndex(0);
        else if (firewall.getCurrentProfile()->getService(i).getProtocol() == "udp")
            protocolBox->setCurrentIndex(1);
        else
            protocolBox->setCurrentIndex(2);

        /* Defining the action ComboBox */
        QComboBox *actionBox = new QComboBox;
        QStringList actions;
        actions << "drop" << "accept" << "reject";
        actionBox->addItems(actions);
        if (firewall.getCurrentProfile()->getService(i).getAction() == "drop")
            actionBox->setCurrentIndex(0);
        else if (firewall.getCurrentProfile()->getService(i).getAction() == "accept")
            actionBox->setCurrentIndex(1);
        else
            actionBox->setCurrentIndex(2);

        QTableWidgetItem* name = new QTableWidgetItem(QString(firewall.getCurrentProfile()->getService(i).getName()));
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
void MainWindow::fillDefineRulesTale()
{
    //do the fill thingie
}

/* pushButton: Start/Stop firewall */
void MainWindow::on_pushButton_startStop_clicked()
{
    if(firewall.getStatus() == false) {
        firewall.start();
        if (firewall.getStatus() == true) {
            ui->pushButton_startStop->setText("Stop");
            ui->statusLabel->setText("Firewall is ON");
        } else
            ui->statusLabel->setText("Start FAILED!");
    } else {
        firewall.stop();
        if (firewall.getStatus() == false) {
            ui->pushButton_startStop->setText("Start");
            ui->statusLabel->setText("Firewall is OFF");
        } else
            ui->statusLabel->setText("Stop FAILED!");
    }
}

/* pushButton: Reload custom rules */
void MainWindow::on_pushButton_reloadCustomRules_clicked()
{
    QFile customRulesFile("/home/emil/Desktop/customRules.txt");
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
    else {
        firewall.getCurrentProfile()->addService(ui->lineEdit_newServiceRuleName->text(), ui->spinBox_newServiceRulePort->value(), ui->comboBox_newServiceRuleProtocol->currentText(), ui->comboBox_newServiceRuleAction->currentText());
        this->fillServiceRuleTable();
    }
}

/* pushButton: Save services */
void MainWindow::on_pushButton_saveServices_clicked()
{
    for (int i=0;i<firewall.getCurrentProfile()->getServiceCount();i++) {
        QString name = ui->tableWidget_serviceRules->item(i,0)->text();
        /* Casting our objects from QTableWidgetItem to QComboBox/QSpinBox */
        QSpinBox* port = qobject_cast<QSpinBox*>(ui->tableWidget_serviceRules->cellWidget(i,1));
        QComboBox* protocol = qobject_cast<QComboBox*>(ui->tableWidget_serviceRules->cellWidget(i,2));
        QComboBox* action = qobject_cast<QComboBox*>(ui->tableWidget_serviceRules->cellWidget(i,3));

        /* Passing our typecasted objects as parameters to editService function */
        firewall.getCurrentProfile()->editService(i, name, port->value(), protocol->currentText(), action->currentText());
    }
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

        bool firstProfile = FALSE;
        if (firewall.getFirstTimeUse() == TRUE)
            firstProfile = TRUE;

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

        if (firstProfile == TRUE) {
            this->loadCurrentProfile();
            this->firstTimeUse(FALSE);
        } else {
            //Re-filling the change profile comboBox
            ui->comboBox_changeProfileList->clear();
            for (int i=0;i<firewall.getProfileNames().size();i++)
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

    ui->pushButton_saveProfile->setEnabled(FALSE);
    ui->pushButton_saveProfile_undo->setEnabled(FALSE);
    ui->label_changesDetected->setText("");
    ui->listWidget_interfaces_edit->clear();
    this->loadCurrentProfile();
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
    ui->pushButton_saveProfile->setEnabled(TRUE);
    ui->pushButton_saveProfile_undo->setEnabled(TRUE);
    ui->label_changesDetected->setText("Changes detected!");
}

/* comboBox: defaultPolicyIN_edit - indexChanged */
void MainWindow::on_comboBox_defaultPolicyIN_edit_currentIndexChanged(const QString &arg1)
{
    ui->pushButton_saveProfile->setEnabled(TRUE);
    ui->pushButton_saveProfile_undo->setEnabled(TRUE);
    ui->label_changesDetected->setText("Changes detected!");
}

/* comboBox: defaultPolicyOUT_edit - indexChanged */
void MainWindow::on_comboBox_defaultPolicyOUT_edit_currentIndexChanged(const QString &arg1)
{
    ui->pushButton_saveProfile->setEnabled(TRUE);
    ui->pushButton_saveProfile_undo->setEnabled(TRUE);
    ui->label_changesDetected->setText("Changes detected!");
}

/* pushButton: Delete interface - edit current profile */
void MainWindow::on_pushButton_addInterface_edit_delete_clicked()
{
    delete ui->listWidget_interfaces_edit->currentItem();
    ui->pushButton_saveProfile->setEnabled(TRUE);
    ui->pushButton_saveProfile_undo->setEnabled(TRUE);
    ui->label_changesDetected->setText("Changes detected!");
}

/* pushButton: Undo changes to current profile */
void MainWindow::on_pushButton_saveProfile_undo_clicked()
{
    ui->pushButton_saveProfile->setEnabled(FALSE);
    ui->pushButton_saveProfile_undo->setEnabled(FALSE);
    ui->label_changesDetected->setText("");
    ui->listWidget_interfaces_edit->clear();
    this->loadCurrentProfile();
}

void MainWindow::on_pushButton_changeProfile_clicked()
{
    int tmpIndex = ui->comboBox_changeProfileList->currentIndex();
    firewall.changeCurrentProfile(ui->comboBox_changeProfileList->currentText());
    this->loadCurrentProfile();
    ui->comboBox_changeProfileList->setCurrentIndex(tmpIndex);
}
