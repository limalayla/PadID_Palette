#ifndef COULEUR_H
#define COULEUR_H

#include <QColor>

class Couleur
{
public:
    Couleur();
    Couleur(const QString&, const QString&, quint8, quint8, quint8);
    Couleur(const Couleur&);

    QColor  col;
    QString getNom () const;
    QString getDesc() const;
    void    setNom (const QString&);
    void    setDesc(const QString&);

private:
    QString m_nom;
    QString m_desc;

};

#endif // COULEUR_H
