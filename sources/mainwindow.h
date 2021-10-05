#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QTimer>

//#define plugins
#ifdef plugins
#include "plugin/qonsole.h"
#endif
#include "ui_qonsole.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);


private slots:
    //Custom functions
    void refreshAllTabs();
    void refreshProfileTab();
    void refreshServiceTab();
    void refreshCustomRulesTab();
    void firstTimeUse(bool trueOrFalse="true");
    void fillServiceRuleTable();
    void fillDefineRulesTable();
    void on_exit();
    void showMessage();

    void on_pushButton_startStop_clicked();
    void on_pushButton_reloadCustomRules_clicked();
    void on_plainTextEdit_customRules_textChanged();
    void on_pushButton_newServiceRuleClear_clicked();
    void on_pushButton_newServiceRuleAdd_clicked();
    void on_pushButton_saveServices_clicked();
    void on_pushButton_undoServices_clicked();
    void on_pushButton_addInterface_clicked();
    void on_pushButton_createProfile_clicked();
    void on_pushButton_saveProfile_clicked();
    void on_pushButton_addInterface_edit_clicked();
    void on_comboBox_defaultPolicyIN_edit_currentIndexChanged(const QString &arg1);
    void on_comboBox_defaultPolicyOUT_edit_currentIndexChanged(const QString &arg1);
    void on_pushButton_addInterface_edit_delete_clicked();
    void on_pushButton_saveProfile_undo_clicked();
    void on_pushButton_changeProfile_clicked();
    void on_pushButton_removeSelectedServices_clicked();

    //Rules tab
    void on_checkBox_sIP_stateChanged(int arg1);
    void on_checkBox_sPort_stateChanged(int arg1);
    void on_checkBox_dIP_stateChanged(int arg1);
    void on_checkBox_dPort_stateChanged(int arg1);
    void on_checkBox_connectionAmount_stateChanged(int arg1);
    void on_checkBox_connectionRate_stateChanged(int arg1);
    void on_pushButton_addRule_clicked();

    void on_checklogbtn_clicked();

    void timerScan();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
#ifdef plugins
    Qonsole e;
#endif
    QTimer timer;
};

#endif // MAINWINDOW_H
