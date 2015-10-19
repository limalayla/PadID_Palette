#include "couleur.h"

Couleur::Couleur() : m_nom(""), m_desc(""), col(0, 0 , 0)
{

}

Couleur::Couleur(const Couleur& c)
    : m_nom(c.m_nom), m_desc(c.m_desc), col(c.col)
{

}

Couleur::Couleur(const QString& nom, const QString& desc, quint8 r, quint8 g, quint8 b)
    : m_nom(nom), m_desc(desc), col(r, g, b)
{

}

QString Couleur::getNom() const               { return m_nom;  }
QString Couleur::getDesc() const              { return m_desc; }
void    Couleur::setNom(const QString& nom)   { m_nom  = nom;  }
void    Couleur::setDesc(const QString& desc) { m_desc = desc; }
