#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->listeClient, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(activateColors(QListWidgetItem*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::activateColors(QListWidgetItem* item)
{
    ui->btn_colorAdd->setEnabled(true);
    ui->btn_colorDel->setEnabled(true);
    ui->btn_colorProp->setEnabled(true);
    ui->btn_validation->setEnabled(true);
    MajCodeCouleur(item);
}

void MainWindow::MajCodeCouleur(QListWidgetItem* item)
{
    ui->listeEncodage->setEnabled(true);
    ui->listeEncodage->clear();
    ui->listeEncodage->addItem("RGB :");
    ui->listeEncodage->addItem("Hex :");
    ui->listeEncodage->addItem("CMY :");
    ui->listeEncodage->addItem("TSL :");
}
