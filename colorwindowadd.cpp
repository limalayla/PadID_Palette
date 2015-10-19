#include "colorwindowadd.h"
#include "ui_colorwindowadd.h"

ColorWindowAdd::ColorWindowAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorWindowAdd)
{
    ui->setupUi(this);
}

ColorWindowAdd::~ColorWindowAdd()
{
    delete ui;
}
