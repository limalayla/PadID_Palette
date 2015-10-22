#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    winAddCol = NULL;

    profileRep = new QDir("profiles");
    if (!profileRep->exists()) {
        profileRep->mkpath(".");
    }
/*
    winAddCol = new ColorWindowAdd(this);
    winAddCol->show();*/


    QObject::connect(ui->listeClient, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(activateColors(QListWidgetItem*)));
    QObject::connect(ui->btn_couleur, SIGNAL(clicked()), this, SLOT(afficher_CouleurCourante()));

    QObject::connect(ui->btn_clientAdd, SIGNAL(clicked()), this, SLOT(ajouterClient()));
    QObject::connect(ui->btn_clientDel, SIGNAL(clicked()), this, SLOT(supprimClient()));

    QObject::connect(ui->btn_colorAdd, SIGNAL(clicked()), this, SLOT(ajouterCouleur()));


    Client* c(NULL);

    profileRep->setFilter(QDir::Files);
    for(uint i= 0; i< profileRep->count(); i++)
    {
        QString filePath = profileRep->absoluteFilePath(profileRep->entryList().at(i));

        c = Client::loadFromFile(filePath);
        if(c != NULL)
        {
            m_clients.push_back(*c);
            ui->listeClient->addItem(m_clients.last().getNom());

            delete c;
            c = NULL;
        }
    }



}

MainWindow::~MainWindow()
{
    foreach(QString dirFile, profileRep->entryList())
    {
        profileRep->remove(dirFile);
    }

    foreach(Client c, m_clients)
    {
        Client::saveToFile(c);
    }

    delete ui;
    //if(winAddCol != NULL) delete winAddCol;
}

void MainWindow::activateColors(QListWidgetItem* item)
{
    ui->btn_clientDel->setEnabled(true);
    ui->btn_clientProp->setEnabled(true);
    ui->btn_colorAdd->setEnabled(true);
    ui->btn_colorDel->setEnabled(true);
    ui->btn_colorProp->setEnabled(true);
    ui->btn_validation->setEnabled(true);
    MajCodeCouleur(item);

    actuGrilleCouleur();
}

void MainWindow::MajCodeCouleur(QListWidgetItem*)
{
    ui->listeEncodage->setEnabled(true);
    ui->listeEncodage->clear();
    ui->listeEncodage->addItem("RGB :");
    ui->listeEncodage->addItem("Hex :");
    ui->listeEncodage->addItem("CMY :");
    ui->listeEncodage->addItem("TSL :");
}

void MainWindow::afficher_CouleurCourante()
{
    QString StyleSheetParent = ui->btn_couleur->styleSheet();
    ui->widget_CouleurCourante->setStyleSheet(StyleSheetParent);
}


void MainWindow::ajouterCouleur()
{
    winAddCol = new ColorWindowAdd(this);
    winAddCol->show();
}

void MainWindow::ajouterClient()
{
    QString nom = QInputDialog::getText(this, "Nouveau Client", "Nom du nouveau client : ");
    if(nom != "")
    {
        QString filePath = profileRep->path() + "/" + nom.toLower();
        QMessageBox::information(this, "", nom + " " + filePath);

        addClient(Client(nom, filePath));
    }
}

void MainWindow::supprimClient()
{
    if(QMessageBox::question(this, "Supprimer Client", "Etes vous sur ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        int index = ui->listeClient->currentIndex().row();

        m_clients.remove(index);
        delete ui->listeClient->item(index);

        if(m_clients.size() == 0)
        {
            ui->btn_clientDel->setEnabled(false);
            ui->btn_clientProp->setEnabled(false);
        }
    }
}

void MainWindow::addClient(const Client& c)
{
    m_clients.push_back(c);
    ui->listeClient->addItem(c.getNom());
}

void MainWindow::getCouleur()
{
    qDebug() << "Byebye";
    Couleur* newCol = NULL;
    if(winAddCol != NULL) newCol = winAddCol->getCouleur();

    if(newCol != NULL)
    {
        int indexClientSelectionne = ui->listeClient->currentIndex().row();
        m_clients[indexClientSelectionne].addColor(*newCol);
        actuGrilleCouleur();
        delete newCol;
    }

}

void MainWindow::actuGrilleCouleur()
{
    Client curClient = m_clients[ui->listeClient->currentIndex().row()];
    QVector<Couleur> ListeCouleur = curClient.getCol();
    int nbCouleur= ListeCouleur.size();
    QColor couleurTmp;
    int r,g,b;
    int posX=0, posY=0;
    QString Style;
    for(int i=0; i<nbCouleur; i++)
    {
        QPushButton *boutonTmp = new QPushButton("",ui->grpb_listeCouleur);
        couleurTmp = ListeCouleur.value(i).col;
        couleurTmp.getRgb(&r,&g,&b);
        Style ="background: rgb("+QString::number(r)+","+QString::number(g)+","+QString::number(b)+")";
        boutonTmp->setStyleSheet(Style);

        boutonTmp->setGeometry(posX,posY,160/(nbCouleur*0.5),150/(nbCouleur*0.5));
        if(posX+2*(160/(nbCouleur*0.5))>=160)
        {
            posX=0;
            posY+=150/(nbCouleur*0.5);
        }
        else
            posX=posX+160/(nbCouleur*0.5);




        Grille_Couleur.push_back(boutonTmp);
    }
    for(int i=0; i<nbCouleur; i++)
    {
        Grille_Couleur.value(i)->show();
    }
}
