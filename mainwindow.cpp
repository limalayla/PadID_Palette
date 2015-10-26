#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), winAddCol(NULL), indexCouleur(-1), indexClient(-1)
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

void MainWindow::debugOut(const QString& msg)
{
    qDebug() << "Debug : " << msg;

    if(indexClient >= 0)
    {
        qDebug() << "\t" << "Selection du client #" << indexClient << " {" << m_clients[indexClient].getNom() << ", " << m_clients[indexClient].getCol().size() << "}";

        if(indexCouleur >= 0)
        {
            if(indexCouleur < m_clients[indexClient].getCol().size())
            {
                Couleur* c = &m_clients[indexClient].getCol()[indexCouleur];
                qDebug() << "\t" << "\t" << "Selection de la couleur #" << indexClient << "." << indexCouleur << " {" << c->getNom() << ", " << c->getDesc() << "}";
            }
            else qDebug() << "\t" << "\t" << "Index de couleur trop grand (" << indexCouleur << ")";
        }
        else qDebug() << "\t" << "\t" << "Aucune couleur selectionnee";
    }
    else qDebug() << "\t" << "Aucun client selectionne";

    qDebug();

}

void MainWindow::activateColors(QListWidgetItem*)
{
    setClientSelected(true);

    indexClient  = ui->listeClient->currentRow();
    indexCouleur = (m_clients[indexClient].getCol().size() > 0) ? 0 : -1;

    MajCodeCouleur();

    debugOut("Activate Color");
    actuGrilleCouleur();
}

void MainWindow::MajCodeCouleur()
{
    debugOut("MajCodeCouleur");
    ui->listeEncodage->clear();

    if(indexCouleur >= 0)
    {
        int r(0), g(0), b(0);
        int n(0), c(0), m(0), y(0);


        QVector<Couleur> listecouleur = m_clients[indexClient].getCol();
        QColor* couleur = &listecouleur[indexCouleur].col;
        couleur->getRgb(&r, &g, &b);

        n = qMin(255-b, 255-g);
        n = qMin(n, 255-b);
        c = (100*(255-r-n) / (255-n));
        m = (100*(255-g-n) / (255-n));
        y = (100*(255-b-n) / (255-n));

        ui->listeEncodage->addItem("RGB : " + QString::number(r)     + " " + QString::number(g)     + " " + QString::number(b));
        ui->listeEncodage->addItem("Hex : " + QString::number(r, 16) + " " + QString::number(g, 16) + " " + QString::number(b, 16));
        ui->listeEncodage->addItem("CMY : " + QString::number(c)     + " " + QString::number(m)     + " " + QString::number(y)     + " " + QString::number(n));
        ui->listeEncodage->addItem("TSL : ");
    }

}

void MainWindow::afficher_CouleurCourante()
{
    int i(0);
    bool trouve=false;

    QVector<Couleur> ListeCouleur = m_clients[indexClient].getCol();

    while((i< Grille_Couleur.size()) && (trouve == false))
    {
        if(sender() == Grille_Couleur[i])
            trouve=true;
        else
            i++;
    }

    indexCouleur = (trouve) ? i : -1;

    debugOut("afficher_CouleurCourante");
    majCol();
}

void MainWindow::majCol()
{
    debugOut("MajCol");

    if(indexClient >= 0)
    {
        if(indexCouleur >= 0)
        {
            ui->widget_CouleurCourante->setStyleSheet(Grille_Couleur[indexCouleur]->styleSheet());
            ui->nomCouleur->setText(m_clients[indexClient].getCol()[indexCouleur].getNom());
            ui->descCouleur->setText(m_clients[indexClient].getCol()[indexCouleur].getDesc());
            MajCodeCouleur();
        }

        else
        {
            ui->widget_CouleurCourante->setStyleSheet("background: rgb(0, 0, 0)");
            ui->nomCouleur->setText("");
            ui->descCouleur->setText("");
            setClientSelected(false);
            ui->btn_clientDel->setEnabled(true);
            ui->btn_clientProp->setEnabled(true);
            ui->listeEncodage->clear();
        }
    }
    else setClientSelected(false);
}


void MainWindow::ajouterCouleur()
{
    if(winAddCol != NULL) { delete winAddCol; winAddCol = NULL; }

    winAddCol = new ColorWindowAdd(this);
    winAddCol->show();
}

void MainWindow::supprimCouleur()
{
    debugOut("Supression couleur");

    if(indexClient >= 0)
    {
        if(indexCouleur >= 0)
        {
            if(QMessageBox::question(this, "Supprimer Couleur", "Etes vous sur ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
            {
                m_clients[indexClient].delCol(indexCouleur);
                indexCouleur = (m_clients[indexClient].getCol().size() > 0) ? 0 : -1;
                actuGrilleCouleur();
            }
        }
        else QMessageBox::warning(this, "", "Pas de couleur selectionnee");
    }
    else QMessageBox::warning(this, "", "Pas de client selectionne");
}

void MainWindow::modifieCouleur()
{
    if(winAddCol != NULL) { delete winAddCol; winAddCol = NULL; }

    debugOut("Modification couleur");

    if(indexClient >= 0)
    {
        if(indexCouleur >= 0)
        {
            winAddCol = new ColorWindowAdd(m_clients[indexClient].getCol()[indexCouleur], this);
            this->setEnabled(false);
            winAddCol->show();
            winAddCol->setEnabled(true);
            actuGrilleCouleur();
        }
        else QMessageBox::warning(this, "", "Pas de couleur selectionnee");
    }
    else QMessageBox::warning(this, "", "Pas de client selectionne");

}


void MainWindow::ajouterClient()
{
    QString nom = QInputDialog::getText(this, "Nouveau Client", "Nom du nouveau client : ");
    if(nom != "")
    {
        QString filePath = profileRep->path() + "/" + nom.toLower();

        addClient(Client(nom, filePath));
    }

    debugOut("AjoutClient");
}

void MainWindow::supprimClient()
{
    debugOut("Suppression Client");

    if(indexClient >= 0)
    {
        if(QMessageBox::question(this, "Supprimer Client", "Etes vous sur ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            m_clients.remove(indexClient);
            delete ui->listeClient->item(indexClient);

            if(m_clients.size() == 0)
            {
                setClientSelected(false);
                indexCouleur = -1;
            }

            else
            {
                ui->listeClient->setCurrentRow(0);
                indexClient = 0;
            }

        }
    }
    else QMessageBox::warning(this, "", "Pas de client selectionne");


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
    QString nvNom = QInputDialog::getText(this, "Modifier Client", "Nouveau nom : ", QLineEdit::Normal, m_clients[indexClient].getNom());
    if(nvNom != "")
    {
        m_clients[indexClient].setNom(nvNom);
        ui->listeClient->currentItem()->setText(nvNom);
    }
}

void MainWindow::getCouleur()
{
    Couleur* newCol(NULL);
    if(winAddCol != NULL)
    {
        newCol = winAddCol->getCouleur();
        delete winAddCol;
        winAddCol = NULL;
    }

    if(newCol != NULL)
    {
        m_clients[indexClient].addColor(*newCol);
        actuGrilleCouleur();
        delete newCol;
    }
}

void MainWindow::modCol()
{
    Couleur* newCol(NULL);
    if(winAddCol != NULL)
        newCol = winAddCol->getCouleur();

    this->setEnabled(true);

    if(newCol != NULL)
    {
        if(indexClient >= 0)
        {
            if(indexCouleur >= 0)
            {
                m_clients[indexClient].modCol(indexCouleur, *newCol);
            }
            else QMessageBox::warning(this, "", "Pas de couleur selectionnee");
        }
        else QMessageBox::warning(this, "", "Pas de client selectionne");

        debugOut("Reception de modification couleur");

        actuGrilleCouleur();
        delete newCol;
    }
}

void MainWindow::actuGrilleCouleur()
{
    QVector<Couleur> listeCouleur = m_clients[indexClient].getCol();
    QPushButton* but;

    int n(listeCouleur.size());
    QColor couleurTmp;
    int r,g,b;
    int x(0), y(0), w(ui->grpb_listeCouleur->width()), h(ui->grpb_listeCouleur->height());
    int i;

    /* Nettoyage de la grille d'affichage précedente */
        for(i= 0; i< Grille_Couleur.size(); i++)
        {
            Grille_Couleur[i]->setHidden(true);
            QObject::disconnect(Grille_Couleur[i], SIGNAL(clicked()), this, SLOT(afficher_CouleurCourante()));
            delete Grille_Couleur[i];
        }
        Grille_Couleur.clear();


    for(i= 0; i< n; i++)
    {
        /* Création d'un nouveau bouton associé à la grille de couleur */
            but = new QPushButton("", ui->grpb_listeCouleur);

        /* Récuperation de la couleur correspondante dans la liste de couleur du client choisi */
            couleurTmp = listeCouleur[i].col;
            couleurTmp.getRgb(&r,&g,&b);

        /* Affectation de cette couleur au bouton */
            but->setStyleSheet("background: rgb(" + QString::number(r) + ","
                                                  + QString::number(g) + ","
                                                  + QString::number(b) + ")"
                              );

        /* Algo pour afficher les boutons ainsi obtenus sous forme de grille (de taille variable) */
            //but->setGeometry(x, y, w/(n*0.5), h/(n*0.5));
            but->setGeometry(x, y, 15, 15);
            // Si on atteind le bord droite
            if(x + 2*but->width() >= w)
            {
                // Retour à la ligne
                    x = 0;
                    y += but->height();
            }
            else
                // Sinon on continue la ligne
                x += but->width();

        Grille_Couleur.push_back(but);

        Grille_Couleur.last()->show();
        QObject::connect(Grille_Couleur.last(), SIGNAL(clicked()), this, SLOT(afficher_CouleurCourante()));
    }

    majCol();
}

void MainWindow::copyColor() const
{
    if(ui->listeEncodage->currentItem() != NULL)
        QApplication::clipboard()->setText(ui->listeEncodage->currentItem()->text());
}
