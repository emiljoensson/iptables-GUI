#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadCurrentProfile();
    void firstTimeUse(bool trueOrFalse="true");
    void fillServiceRuleTable();
    void fillDefineRulesTale();
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
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
