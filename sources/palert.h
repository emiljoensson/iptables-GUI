#ifndef PALERT_H
#define PALERT_H

#include <QDialog>

namespace Ui {
class palert;
}

struct PacketInfo
{
    QString app;
    QString proto;
    QString source;
    QString dest;
    QString uid;
    QString dpt;
};

class palert : public QDialog
{
    Q_OBJECT

public:
    explicit palert(QWidget *parent = nullptr);
    ~palert();

    void setPacketInfo(PacketInfo info);

public slots:
    void accept();

private:
    Ui::palert *ui;

    PacketInfo mInfo;
};

#endif // PALERT_H
