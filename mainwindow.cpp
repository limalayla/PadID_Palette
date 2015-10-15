#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->listeClient, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(activateColors(QListWidgetItem*)));
    QObject::connect(ui->btn_couleur,SIGNAL(clicked()), this, SLOT(afficher_CouleurCourante()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::activateColors(QListWidgetItem* ) {}

void MainWindow::afficher_CouleurCourante()
{
    QString StyleSheetParent = ui->btn_couleur->styleSheet();
    ui->widget_CouleurCourante->setStyleSheet(StyleSheetParent);
}
