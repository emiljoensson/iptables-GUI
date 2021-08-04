#ifndef QONSOLE_H
#define QONSOLE_H

#include <QMainWindow>
#include <QProcess>

namespace Ui {
class Qonsole;
}

class Qonsole : public QMainWindow
{
    Q_OBJECT

public:
    explicit Qonsole(QWidget *parent = 0);
    ~Qonsole();

public slots:
    void execute();
    void showOutput();
    bool eventFilter(QObject *watched, QEvent *event);

protected:
    void updateCursor();

private:
    Ui::Qonsole *ui;

    QString m_user_input;
    QProcess* m_process;
};

#endif // QONSOLE_H
