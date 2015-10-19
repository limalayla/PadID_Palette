#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPushButton>
#include <QInputDialog>

#include "client.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool addClient(const Client&);

public slots:
    void activateColors(QListWidgetItem*);
    void MajCodeCouleur(QListWidgetItem*);
    void afficher_CouleurCourante();
    void ajouterClient();

private:
    Ui::MainWindow *ui;
    QDir* profileRep;
    QVector<Client> m_clients;
};

#endif // MAINWINDOW_H
