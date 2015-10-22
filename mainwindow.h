#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPushButton>
#include <QInputDialog>

#include "client.h"
#include "colorwindowadd.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addClient(const Client&);
    void actuGrilleCouleur();

public slots:
    void activateColors(QListWidgetItem*);
    void MajCodeCouleur(QListWidgetItem*);
    void afficher_CouleurCourante();

    void ajouterClient();
    void supprimClient();

    void ajouterCouleur();
    void getCouleur();

private:
    Ui::MainWindow *ui;
    QDir* profileRep;
    QVector<Client> m_clients;

    ColorWindowAdd* winAddCol;
    QVector<QPushButton*> Grille_Couleur;
};

#endif // MAINWINDOW_H
