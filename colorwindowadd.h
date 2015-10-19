#ifndef COLORWINDOWADD_H
#define COLORWINDOWADD_H

#include <QDialog>

namespace Ui {
class ColorWindowAdd;
}

class ColorWindowAdd : public QDialog
{
    Q_OBJECT

public:
    explicit ColorWindowAdd(QWidget *parent = 0);
    ~ColorWindowAdd();

private:
    Ui::ColorWindowAdd *ui;
};

#endif // COLORWINDOWADD_H
