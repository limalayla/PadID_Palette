#include "colorwindowadd.h"
#include "ui_colorwindowadd.h"

ColorWindowAdd::ColorWindowAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorWindowAdd), r(0), g(0), b(0), modif(false)
{
    ui->setupUi(this);
    QObject::connect(ui->btn_ok, SIGNAL(clicked()), parent, SLOT(getCouleur()));
    init();
    majCouleur();
}

ColorWindowAdd::ColorWindowAdd(const Couleur& c, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorWindowAdd), r(c.col.red()), g(c.col.green()), b(c.col.blue()), modif(true)
{
    ui->setupUi(this);
    QObject::connect(ui->btn_ok, SIGNAL(clicked()), parent, SLOT(modCol()));
    init();

    ui->in_titre->setText(c.getNom());
    ui->in_desc->setText(c.getDesc());

    ui->in_rouge->setText(QString::number(r));
    ui->in_vert->setText(QString::number(g));
    ui->in_bleu->setText(QString::number(b));

    ui->slider_rouge->setValue(r);
    ui->slider_vert->setValue(g);
    ui->slider_bleu->setValue(b);

    QWidget::setWindowTitle("Modification Couleur");

    majCouleur();
}

ColorWindowAdd::~ColorWindowAdd()
{
    qDebug() << "FERME";
    delete ui;
}

void ColorWindowAdd::init()
{
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

    QObject::connect(ui->btn_ok, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(ui->btn_annuler, SIGNAL(clicked()), this, SLOT(quit()));

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
}

Couleur* ColorWindowAdd::getCouleur()
{
    return new Couleur(ui->in_titre->text(), ui->in_desc->toPlainText(), r, g, b);
}

void ColorWindowAdd::quit()
{
    this->close();
    this->parentWidget()->setEnabled(true);
}
