#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPushButton>
#include <QInputDialog>
#include <QClipboard>

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
    void majCol();


public slots:
    void activateColors(QListWidgetItem*);
    void MajCodeCouleur();
    void afficher_CouleurCourante();

    void ajouterClient();
    void supprimClient();
    void modifieClient();

    void ajouterCouleur();
    void supprimCouleur();
    void modifieCouleur();

    void getCouleur();

    void setClientSelected(bool b);
    void copyColor();
    void modCol();

    void debugOut(const QString& msg);

private:
    Ui::MainWindow *ui;
    QDir* profileRep;
    QVector<Client> m_clients;

    ColorWindowAdd* winAddCol;
    QVector<QPushButton*> Grille_Couleur;

    int indexCouleur;
    int indexClient;
};

#endif // MAINWINDOW_H
