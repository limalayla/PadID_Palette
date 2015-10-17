#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QColor>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void activateColors(QListWidgetItem*);
    void MajCodeCouleur(QListWidgetItem*);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
