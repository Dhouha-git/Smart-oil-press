#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "employe.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
   // void on_btn_employes_clicked();
    //void on_btn_ajouter_clicked();
    //void on_btn_retour_clicked();
    void on_btn_valider_clicked();
    void on_btn_supprimer_clicked();

    void on_btn_retour_3_clicked();

    void on_btn_annuler_3_clicked();

    void on_btn_employes_5_clicked();

    void on_btn_modifier_clicked();

    void on_btn_employes_3_clicked();

    void on_btn_ajouter_clicked();
   // void on_tableView_clicked(const QModelIndex &index);
   // void on_btn_modifier_valider_clicked();
    void on_btn_annuler_4_clicked();

    void on_btn_retour_4_clicked();

    void on_btn_employes_6_clicked();

private:
    Ui::MainWindow *ui;
    Employe Etmp;

    void refreshTable();
};

#endif
