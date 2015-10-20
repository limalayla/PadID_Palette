#include "colorwindowadd.h"
#include "ui_colorwindowadd.h"

ColorWindowAdd::ColorWindowAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorWindowAdd), reponse(new Couleur())
{
    init(parent);
    majCouleur(0, 3);
}

ColorWindowAdd::ColorWindowAdd(Couleur& c, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorWindowAdd), reponse(new Couleur())
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

    majCouleur(0, 3);
}

ColorWindowAdd::~ColorWindowAdd()
{
    qDebug() << "FERME";
    delete ui;
    delete reponse;
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

    ui->widget->setPalette(QPalette(reponse->col));
}

void ColorWindowAdd::setTextRed  (int value) { ui->in_rouge->setText(QString::number(value)); majCouleur(value, 0); }
void ColorWindowAdd::setTextGreen(int value) { ui->in_vert->setText(QString::number(value));  majCouleur(value, 1); }
void ColorWindowAdd::setTextBlue (int value) { ui->in_bleu->setText(QString::number(value));  majCouleur(value, 2); }

void ColorWindowAdd::setSlidRed  (QString value) { ui->slider_rouge->setValue(value.toInt()); majCouleur(value.toInt(), 0); }
void ColorWindowAdd::setSlidGreen(QString value) { ui->slider_vert->setValue(value.toInt());  majCouleur(value.toInt(), 1); }
void ColorWindowAdd::setSlidBlue (QString value) { ui->slider_bleu->setValue(value.toInt());  majCouleur(value.toInt(), 2); }

void ColorWindowAdd::majCouleur(int value, uint rgb)
{
    if(rgb == 0) reponse->col.setRed(value);
    if(rgb == 1) reponse->col.setGreen(value);
    if(rgb == 2) reponse->col.setBlue(value);

    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, reponse->col);
    ui->widget->setAutoFillBackground(true);
    ui->widget->setPalette(Pal);
    ui->widget->show();
}

Couleur* ColorWindowAdd::getCouleur()
{
    reponse->setNom(ui->in_titre->text());
    reponse->setDesc(ui->in_desc->toPlainText());

    return reponse;
}

