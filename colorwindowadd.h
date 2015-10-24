#ifndef COLORWINDOWADD_H
#define COLORWINDOWADD_H

#include <QDialog>
#include <QDebug>
#include <QValidator>

#include "couleur.h"
namespace Ui {
class ColorWindowAdd;
}

class ColorWindowAdd : public QDialog
{
    Q_OBJECT

public:
    explicit ColorWindowAdd(QWidget *parent = 0);
    explicit ColorWindowAdd(const Couleur& c, QWidget *parent = 0);
    ~ColorWindowAdd();
    void majCouleur();

public slots:
    void setTextRed(int);
    void setTextGreen(int);
    void setTextBlue(int);

    void setSlidRed(QString);
    void setSlidGreen(QString);
    void setSlidBlue(QString);

    void quit();

    Couleur* getCouleur();

private:
    Ui::ColorWindowAdd *ui;
    void init();
    quint8 r, g, b;
    bool modif;
};

#endif // COLORWINDOWADD_H
