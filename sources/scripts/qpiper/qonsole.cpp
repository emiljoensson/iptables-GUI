#include "qonsole.h"
#include "ui_qonsole.h"

#include <QKeyEvent>

Qonsole::Qonsole(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Qonsole)
{
    ui->setupUi(this);

    ui->edt_log->installEventFilter(this);
    ui->edt_log->setLineWrapMode(QTextEdit::WidgetWidth);

    ui->edt_input->setVisible(false);

    m_process = new QProcess(this);
    m_process->setReadChannelMode(QProcess::MergedChannels);
    connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(showOutput()));

    QString program = "myscript.sh";
    m_process->start("/bin/sh", QStringList() << program, QIODevice::ReadWrite);
   // m_process->start("python", QIODevice::ReadWrite);

    //m_process->start("cmd", QStringList() << program, QIODevice::ReadWrite);
}

Qonsole::~Qonsole()
{
    delete ui;
}

void Qonsole::execute()
{
    QString cmd_str = ui->edt_input->toPlainText();

    ui->edt_input->clear();
    ui->edt_log->append(cmd_str);

    QByteArray bytes = cmd_str.toUtf8();
    m_process->write(bytes);
}

void Qonsole::showOutput()
{
    QByteArray bytes = m_process->readAllStandardOutput();

    QStringList lines = QString(bytes).split("\n");

    foreach (QString line, lines) {
        ui->edt_log->append(line);
    }

}

bool Qonsole::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent* key_event = static_cast<QKeyEvent*>(event);
        int key = key_event->key();
        QString key_str = key_event->text();

        ui->edt_input->insertPlainText(key_str);
        ui->edt_log->insertPlainText(key_str);

        if ((key == Qt::Key_Return) || (key == Qt::Key_Enter) )
        {
            execute();
            return true;
        }
        else
        {
            //ui->edt_log->insertPlainText(key_str);
            return true;
        }
    }

    return QMainWindow::eventFilter(watched, event);
}


