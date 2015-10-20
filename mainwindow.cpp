#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
/*
    winAddCol = new ColorWindowAdd(this);
    winAddCol->show();*/

    /*On prend le nombre de couleur dont dispose le client pour faire un tableau de meme taille
    QPushButton btn_couleur[nbCouleurClient];
    QPushButton btn_couleur[2];
    for(int i=0; i<2; i++) // on devrait aller jusqu'a nbCouleurClient
    {
        QPushButton btn_tmp(""); // pour le test on ne prend que un seul bouton
        btn_couleur[i]=btn_tmp;
        //Ici on place les boutons dans la groupBox
        // On definit l'evenement pour tous les boutons que l'on a cree
        ui->btn_couleur[i]->setStyleSheet(//Mettre le background avec la couleur Corrsepondante)
        QObject::connect(ui->btn_couleur[i],SIGNAL(clicked(QPushButton)), this, SLOT(afficher_CouleurCourante(QPushButton*))); // Il faut envoyer le Bouton
        sur qui on a cliquer au slot

    }

    */
    QObject::connect(ui->listeClient, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(activateColors(QListWidgetItem*)));
    QObject::connect(ui->btn_couleur, SIGNAL(clicked()), this, SLOT(afficher_CouleurCourante()));
    QObject::connect(ui->btn_clientAdd, SIGNAL(clicked()), this, SLOT(ajouterClient()));
    QObject::connect(ui->btn_colorAdd, SIGNAL(clicked()), this, SLOT(ajouterCouleur()));

    profileRep = new QDir("profiles");
    if (!profileRep->exists()) {
        profileRep->mkpath(".");
    }
    else qDebug() << profileRep->absolutePath() << "existe et contient " << profileRep->count()-2 << " fichiers";

    Client* c;
    /* c->setNom("Jesuis Untest");
     c->setFP(profileRep->path() + "test");
     c->addColor(Couleur("first", "desc", 0, 0, 0));

     m_clients.push_back(*c);
     ui->listeClient->addItem(m_clients[qMax(0, m_clients.size()-1)].getNom());*/

    profileRep->setFilter(QDir::Files);
    for(uint i= 0; i< profileRep->count(); i++)
    {
        QString filePath = profileRep->entryList().at(i);
        qDebug() << i << " : " << filePath;

        //if(filePath != "." && filePath != "..")
        //{
            c = Client::loadFromFile(profileRep->absoluteFilePath(filePath));
            if(c != NULL)
            {
                m_clients.push_back(*c);
                ui->listeClient->addItem(m_clients.last().getNom());
                delete c;
            }
        //}
    }
}

MainWindow::~MainWindow()
{
    //profileRep.setNameFilters(QStringList() << "*.*");

    foreach(QString dirFile, profileRep->entryList())
    {
        profileRep->remove(dirFile);
    }

    for(int i= 0; i< qAbs(m_clients.size()); i++)
    {
        Client::saveToFile(m_clients[i]);
    }

    delete ui;
    //if(winAddCol != NULL) delete winAddCol;
}

void MainWindow::activateColors(QListWidgetItem* item)
{
    ui->btn_colorAdd->setEnabled(true);
    ui->btn_colorDel->setEnabled(true);
    ui->btn_colorProp->setEnabled(true);
    ui->btn_validation->setEnabled(true);
    MajCodeCouleur(item);
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

    QString filePath = profileRep->path() + "/" + nom.toLower();
    QMessageBox::information(this, "", nom + " " + filePath);

    addClient(Client(nom, filePath));
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

        delete newCol;
    }
}
