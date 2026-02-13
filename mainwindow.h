#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_login_clicked();
    void on_btn_agriculteurs_clicked();
    void on_btn_home_clicked();

    void on_btn_employes_clicked();


    void on_btn_ajouter_clicked();

    void on_btn_retour_clicked();

    void on_btn_valider_clicked();


private:
    Ui::MainWindow *ui;

    void createChart();
    void updateStats();
};

#endif // MAINWINDOW_H
