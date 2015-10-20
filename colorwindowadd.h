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
    explicit ColorWindowAdd(Couleur& c, QWidget *parent = 0);
    ~ColorWindowAdd();
    void majCouleur(int, uint);

public slots:
    void setTextRed(int);
    void setTextGreen(int);
    void setTextBlue(int);

    void setSlidRed(QString);
    void setSlidGreen(QString);
    void setSlidBlue(QString);

    Couleur* getCouleur();

private:
    Ui::ColorWindowAdd *ui;
    Couleur* reponse;
    void init(QWidget *parent);
};

#endif // COLORWINDOWADD_H
