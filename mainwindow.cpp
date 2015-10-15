#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QColor>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->listeClient, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(MajCodeCouleur(QListWidgetItem*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::MajCodeCouleur(QListWidgetItem* ii ) {

    ui->listeEncodage->setEnabled(true);
    ui->listeEncodage->clear();
    ui->listeEncodage->addItem("RGB :");
    ui->listeEncodage->addItem("Hex :");
    ui->listeEncodage->addItem("CMY :");
    ui->listeEncodage->addItem("TSL :");
}
