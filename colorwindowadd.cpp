#include "colorwindowadd.h"
#include "ui_colorwindowadd.h"

ColorWindowAdd::ColorWindowAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorWindowAdd)
{
    init(parent);
    majCouleur();
}

ColorWindowAdd::ColorWindowAdd(Couleur& c, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorWindowAdd)
{
    init(parent);

    ui->in_titre->setText(c.getNom());
    ui->in_desc->setText(c.getDesc());

    ui->in_rouge->setText(QString::number(c.col.red()));
    ui->in_vert->setText(QString::number(c.col.green()));
    ui->in_bleu->setText(QString::number(c.col.blue()));

    ui->slider_rouge->setValue(c.col.red());
    ui->slider_vert->setValue(c.col.green());
    ui->slider_bleu->setValue(c.col.blue());

    majCouleur();
}

ColorWindowAdd::~ColorWindowAdd()
{
    qDebug() << "FERME";
    delete ui;
}

void ColorWindowAdd::init(QWidget *parent)
{
    ui->setupUi(this);
    qDebug() << "OUVERT";

    ui->in_rouge->setValidator(new QIntValidator(0, 255, this));
    ui->in_vert->setValidator(new QIntValidator(0, 255, this));
    ui->in_bleu->setValidator(new QIntValidator(0, 255, this));

    QObject::connect(ui->slider_rouge, SIGNAL(valueChanged(int)), this, SLOT(setTextRed(int)));
    QObject::connect(ui->slider_vert , SIGNAL(valueChanged(int)), this, SLOT(setTextGreen(int)));
    QObject::connect(ui->slider_bleu , SIGNAL(valueChanged(int)), this, SLOT(setTextBlue(int)));

    QObject::connect(ui->in_rouge, SIGNAL(textEdited(QString)), this, SLOT(setSlidRed(QString)));
    QObject::connect(ui->in_vert,  SIGNAL(textEdited(QString)), this, SLOT(setSlidGreen(QString)));
    QObject::connect(ui->in_bleu,  SIGNAL(textEdited(QString)), this, SLOT(setSlidBlue(QString)));

    QObject::connect(ui->btn_ok, SIGNAL(clicked()), parent, SLOT(getCouleur()));
    QObject::connect(ui->btn_ok, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(ui->btn_annuler, SIGNAL(clicked()), this, SLOT(close()));

    majCouleur();
}

void ColorWindowAdd::setTextRed  (int value) { r = value; ui->in_rouge->setText(QString::number(r)); majCouleur(); }
void ColorWindowAdd::setTextGreen(int value) { g = value; ui->in_vert->setText(QString::number(g));  majCouleur(); }
void ColorWindowAdd::setTextBlue (int value) { b = value; ui->in_bleu->setText(QString::number(b));  majCouleur(); }

void ColorWindowAdd::setSlidRed  (QString value) { r = value.toInt(); ui->slider_rouge->setValue(r); majCouleur(); }
void ColorWindowAdd::setSlidGreen(QString value) { g = value.toInt(); ui->slider_vert->setValue(g);  majCouleur(); }
void ColorWindowAdd::setSlidBlue (QString value) { b = value.toInt(); ui->slider_bleu->setValue(b);  majCouleur(); }

void ColorWindowAdd::majCouleur()
{
    QPalette Pal(palette());

    Pal.setColor(QPalette::Background, QColor(r, g, b));
    ui->widget->setAutoFillBackground(true);
    ui->widget->setPalette(Pal);
    ui->widget->show();
}

Couleur* ColorWindowAdd::getCouleur()
{
    return new Couleur(ui->in_titre->text(), ui->in_desc->toPlainText(), r, g, b);
}

