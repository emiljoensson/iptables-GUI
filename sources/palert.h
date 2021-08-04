#ifndef PALERT_H
#define PALERT_H

#include <QDialog>

namespace Ui {
class palert;
}

class palert : public QDialog
{
    Q_OBJECT

public:
    explicit palert(QWidget *parent = nullptr);
    ~palert();

private:
    Ui::palert *ui;
};

#endif // PALERT_H
