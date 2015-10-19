#include "client.h"

QString Client::separator("~&!");

Client::Client() : m_nom(""), m_filePath("")
{}

Client::Client(const Client& c)
    : m_nom(c.m_nom), m_filePath(c.m_filePath), m_colorlist(c.m_colorlist)
{}

Client::Client(const QString& nom, const QString& filePath, const QVector<Couleur> colorList)
    : m_nom(nom), m_filePath(filePath), m_colorlist(colorList)
{}

QString Client::toSavableFormat() const
{
    QString res = m_nom + "\n";

    for(uint i= 0; i< qAbs(m_colorlist.size()); i++)
    {
        res += QString::number(m_colorlist[i].col.red())   + Client::separator;
        res += QString::number(m_colorlist[i].col.green()) + Client::separator;
        res += QString::number(m_colorlist[i].col.blue())  + Client::separator;
        res += m_colorlist[i].getNom()  + Client::separator;
        res += m_colorlist[i].getDesc() + '\n';
    }

    if(m_colorlist.size() > 0) res = res.left(res.size()-1);

    return res;
}

Client* Client::loadFromFile(const QString& filePath)
{
    Client* res = new Client();
    QFile inFile(filePath);

    if (!inFile.exists())
    {
        QMessageBox::warning(NULL, "Erreur", "Le fichier " + filePath + " n'existe pas.");
        return NULL;
    }

    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(NULL, "Erreur", "Impossible d'ouvrir " + filePath);
        return NULL;
    }

    QTextStream in(&inFile);
    res->m_nom = in.readLine();

    Couleur* tmp;
    QStringList ligne;

    while (!in.atEnd())
    {
        ligne = in.readLine().split("~&!");

        if((tmp = res->getCouleur(ligne)) == NULL)
        {
            QMessageBox::warning(NULL, "Erreur", "Fichier " + filePath + " mal formaté, abandon");
            inFile.close();
            return NULL;
        }

        res->m_colorlist.push_back(*tmp);
    }

    for(qint8 i= 0; i< res->m_colorlist.size(); i++)
    {
        qDebug() << "\ti     (" << i   << ") " ;
        qDebug() << "\tRouge (" << res->m_colorlist[i].col.red()   << ") " ;
        qDebug() << "\tVert  (" << res->m_colorlist[i].col.green() << ") " ;
        qDebug() << "\tBleu  (" << res->m_colorlist[i].col.blue()  << ") " ;
        qDebug() << "\tNom   (" << res->m_colorlist[i].getNom()    << ") " ;
        qDebug() << "\tDesc  (" << res->m_colorlist[i].getDesc()   << ") " ;
    }

    inFile.close();

    res->m_filePath = filePath;
    return res;
}

Couleur* Client::getCouleur(const QStringList& sl)
{
    bool ok;
    int tmp(0);
    Couleur* target = new Couleur();

    qDebug() << "Chargement d'une couleur (" << sl.size() << ")";

    if(sl.size() != 5) return NULL;

    tmp = sl.value(0).toInt(&ok);
    qDebug() << "\tRouge : " << sl.value(0) << " -> " << tmp << "(" << ok << ") -> " << (!ok || tmp <= 0 || tmp > 255);
    if(!ok || tmp < 0 || tmp > 255) return NULL;
    target->col.setRed(tmp);


    tmp = sl.value(1).toInt(&ok);
    qDebug() << "Bleu : " << sl.value(1) << " -> " << tmp << "(" << ok << ")";
    if(!ok || tmp < 0 || tmp > 255) return NULL;
    target->col.setBlue(tmp);

    tmp = sl.value(2).toInt(&ok);
    qDebug() << "Vert : " << sl.value(2) << " -> " << tmp << "(" << ok << ")";
    if(!ok || tmp < 0 || tmp > 255) return NULL;
    target->col.setGreen(tmp);

    target->setNom(sl.value(3));
    target->setDesc(sl.value(4));

    return target;
}

void Client::setNom(const QString nom)
{
    m_nom = nom;
}

void Client::setFP(const QString filePath)
{
    m_filePath = filePath;
}

QString Client::getNom() const
{
    return m_nom;
}

QString Client::getFP()  const
{
    return m_filePath;
}

QVector<Couleur> Client::getCol() const
{
    return m_colorlist;
}

void Client::addColor(const Couleur& c)
{
    qDebug() << "Ajout couleur : {" << c.col.red()   << "-"
                                    << c.col.green() << "-"
                                    << c.col.blue()  << "} "
                                    << c.getNom()    << "("
                                    << c.getDesc()   << ")";
    m_colorlist.push_back(c);
}


bool Client::saveToFile(const Client& c)
{
    QFile tmpFile(c.m_filePath);

    tmpFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);

    if(!tmpFile.exists())
        QMessageBox::warning(NULL, "", "Fichier impossible à créer");

    QTextStream out(&tmpFile);

    qDebug() << "Enregistrement :\n\t" << c.toSavableFormat();

    out << c.toSavableFormat();
    tmpFile.close();
}
