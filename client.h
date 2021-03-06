#ifndef CLIENT_H
#define CLIENT_H

#include <QColor>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QVector>
#include <QMessageBox>
#include <QDebug>

#include "couleur.h"

class Client
{
public:
    Client();
    Client(const Client&);
    Client(const QString&, const QString&, const QVector<Couleur> = QVector<Couleur>());

    QString toSavableFormat() const;

    void setNom(const QString);
    void setFP(const QString);

    QString getNom() const;
    QString getFP()  const;
    QVector<Couleur> getCol() const;
    void modCol(int index, const Couleur& c);
    void delCol(int index);


    static Client* loadFromFile(const QString&);
    static bool    saveToFile  (const Client&);

    void addColor(const Couleur&);

    static QString separator;

private:
    QString m_nom;
    QString m_filePath;
    QVector<Couleur> m_colorlist;
    Couleur* getCouleur(const QStringList&);
};

#endif // CLIENT_H
