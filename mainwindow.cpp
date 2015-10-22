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
    QObject::connect(ui->listeClient, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(activateColors(QListWidgetItem*)));
    QObject::connect(ui->btn_couleur, SIGNAL(clicked()), this, SLOT(afficher_CouleurCourante()));

    QObject::connect(ui->btn_clientAdd,  SIGNAL(clicked()), this, SLOT(ajouterClient()));
    QObject::connect(ui->btn_clientDel,  SIGNAL(clicked()), this, SLOT(supprimClient()));
    QObject::connect(ui->btn_clientProp, SIGNAL(clicked()), this, SLOT(modifieClient()));

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
    setClientSelected(true);
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
            setClientSelected(false);
        }
    }
}

void MainWindow::setClientSelected(bool b)
{
    ui->btn_clientDel->setEnabled(b);
    ui->btn_clientProp->setEnabled(b);

    ui->btn_colorAdd->setEnabled(b);
    ui->btn_colorDel->setEnabled(b);
    ui->btn_colorProp->setEnabled(b);

    ui->btn_validation->setEnabled(b);
}

void MainWindow::addClient(const Client& c)
{
    m_clients.push_back(c);
    ui->listeClient->addItem(c.getNom());
}

void MainWindow::modifieClient()
{
    int index = ui->listeClient->currentIndex().row();

    QString nvNom = QInputDialog::getText(this, "Modifier Client", "Nouveau nom : ", QLineEdit::Normal, m_clients[index].getNom());

    m_clients[index].setNom(nvNom);
    ui->listeClient->currentItem()->setText(nvNom);
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
