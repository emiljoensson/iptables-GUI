#include "palert.h"
#include "ui_palert.h"

palert::palert(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::palert)
{
    ui->setupUi(this);
}

palert::~palert()
{
    delete ui;
}
