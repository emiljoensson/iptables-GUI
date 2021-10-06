#include "palert.h"
#include "ui_palert.h"
#include <QProcess>

palert::palert(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::palert)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
}

palert::~palert()
{
    delete ui;
}

void palert::setPacketInfo(PacketInfo info)
{
    mInfo = info;

    ui->label->setText("Application: " + info.app);
    ui->label_2->setText("Protocol: " + info.proto);
    ui->label_3->setText("Source: " + info.source);
    ui->label_4->setText("Destination: " + info.dest);
    ui->label_6->setText("UID: " + info.uid);
}

void palert::accept()
{
   QString text = QString("sudo iptables -A INPUT -p %1 --dport %2 -j ACCEPT")
                    .arg(mInfo.proto.toLower(), mInfo.dpt);

    QProcess::execute("/bin/sh", QStringList() << text);

    close();
}
