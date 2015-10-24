#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), winAddCol(NULL)
{
    ui->setupUi(this);

	/* Ouverture du dossier contenant les profiles */
		profileRep = new QDir("profiles");
		if (!profileRep->exists()) {
		    profileRep->mkpath(".");
		}

		profileRep->setFilter(QDir::Files);


	/* Création des évenements */
		QObject::connect(ui->listeClient, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(activateColors(QListWidgetItem*)));

		QObject::connect(ui->btn_clientAdd,  SIGNAL(clicked()), this, SLOT(ajouterClient()));
		QObject::connect(ui->btn_clientDel,  SIGNAL(clicked()), this, SLOT(supprimClient()));
		QObject::connect(ui->btn_clientProp, SIGNAL(clicked()), this, SLOT(modifieClient()));

        QObject::connect(ui->btn_colorAdd,   SIGNAL(clicked()), this, SLOT(ajouterCouleur()));
        QObject::connect(ui->btn_colorDel,   SIGNAL(clicked()), this, SLOT(supprimCouleur()));
        QObject::connect(ui->btn_colorProp,  SIGNAL(clicked()), this, SLOT(modifieCouleur()));

        QObject::connect(ui->btn_validation, SIGNAL(clicked()), this, SLOT(copyColor()));


    /* Chargement des clients */
        Client* c(NULL);

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
	/* On supprime le contenu du dossier profiles/ pour avoir une base saine à coup sur */
		foreach(QString dirFile, profileRep->entryList())
		{
		    profileRep->remove(dirFile);
		}

	/* Enregistrement de chaques clients */
		foreach(Client c, m_clients)
		{
		    Client::saveToFile(c);
		}

    delete ui;
    if(winAddCol != NULL) delete winAddCol;
}

void MainWindow::activateColors(QListWidgetItem* item)
{
    setClientSelected(true);
    MajCodeCouleur(item);

    actuGrilleCouleur();
}

void MainWindow::MajCodeCouleur(QListWidgetItem*)
{
    int index = ui->listeClient->currentRow();

    QVector<Couleur> listecouleur = m_clients[index].getCol();
    QColor couleur;

    ui->listeEncodage->setEnabled(true);
    ui->listeEncodage->clear();
    if(listecouleur.size() > 0)
    {
    couleur = listecouleur[0].col;
    int r,g,b,n,c,m,y;
    couleur.getRgb(&r,&g,&b);
    //QString temps= "RGB :"+QString::number(couleur.green());


    ui->listeEncodage->addItem("RGB :"+QString::number(r)+" "+QString::number(g)+" "+QString::number(b));
    ui->listeEncodage->addItem("Hex :"+QString::number(r,16)+" "+QString::number(g,16)+" "+QString::number(b,16));
    n = qMin(255-b, 255-g);
    n = qMin(n, 255-b);
    c = (100*(255-r-n) / (255-n));
    m = (100*(255-g-n) / (255-n));
    y = (100*(255-b-n) / (255-n));
    ui->listeEncodage->addItem("CMY :"+QString::number(c)+" "+QString::number(m)+" "+QString::number(y)+" "+QString::number(n));
    ui->listeEncodage->addItem("TSL :");
    }

}

void MainWindow::afficher_CouleurCourante()
{
    int i=0;
    bool trouve=false;
    Client curClient = m_clients[ui->listeClient->currentIndex().row()];
    QVector<Couleur> ListeCouleur = curClient.getCol();

    while((i<Grille_Couleur.size()) && (trouve==false))
    {
        if(sender()==Grille_Couleur.value(i))
            trouve=true;
        else
            i++;
    }
    indexCouleurCur=i;
    ui->widget_CouleurCourante->setStyleSheet(Grille_Couleur.value(indexCouleurCur)->styleSheet());
    ui->nomCouleur->setText(ListeCouleur.value(indexCouleurCur).getNom());
    ui->descCouleur->setText(ListeCouleur.value(indexCouleurCur).getDesc());
}


void MainWindow::ajouterCouleur()
{
    if(winAddCol != NULL) { delete winAddCol; winAddCol = NULL; }

    winAddCol = new ColorWindowAdd(this);
    winAddCol->show();
}

void MainWindow::supprimCouleur()
{
    /* Récuperer couleur courante à la place */
        //indexCouleur = m_clients[ui->listeClient->currentRow()].getCol()
    m_clients[ui->listeClient->currentRow()].getCol().pop_back();
}

void MainWindow::modifieCouleur()
{
    if(winAddCol != NULL) { delete winAddCol; winAddCol = NULL; }

    /* Récuperer couleur courante à la place */
    Couleur* c = new Couleur("Titre", "Description", 100, 250, 80);
    if(c != NULL)
    {
        winAddCol = new ColorWindowAdd(*c, this);
        winAddCol->show();
    }
}


void MainWindow::ajouterClient()
{
    QString nom = QInputDialog::getText(this, "Nouveau Client", "Nom du nouveau client : ");
    if(nom != "")
    {
        QString filePath = profileRep->path() + "/" + nom.toLower();

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

    ui->nomCouleur->setEnabled(b);
    ui->descCouleur->setEnabled(b);
    ui->widget_CouleurCourante->setEnabled(b);
    ui->btn_validation->setEnabled(b);

    ui->listeEncodage->clear();
    ui->listeEncodage->setEnabled(b);
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
        actuGrilleCouleur();
        delete newCol;
    }
}

void MainWindow::modCol()
{
    Couleur* newCol = NULL;
    if(winAddCol != NULL) newCol = winAddCol->getCouleur();

    if(newCol != NULL)
    {
        /* Remplacer la couleur actuelle par la nouvelle */
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
        Grille_Couleur.value(i)->deleteLater();
    }
    Grille_Couleur.clear();
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
        QObject::connect(Grille_Couleur.value(i), SIGNAL(clicked()), this, SLOT(afficher_CouleurCourante()));
    }
}

void MainWindow::copyColor() const
{
    if(ui->listeEncodage->currentItem() != NULL)
        QApplication::clipboard()->setText(ui->listeEncodage->currentItem()->text());
}
