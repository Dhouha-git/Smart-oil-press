#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include <QSqlQueryModel>

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

    // CRUD Agriculteur
    bool ajouterAgriculteur(int cin, QString nom, QString prenom, int telephone, QString email, QString region, QString type_olive, QDate date_inscription, double volume_livraison, QDate date_livraison);
    bool modifierAgriculteur(int cin, QString nom, QString prenom, int telephone, QString email, QString region, QString type_olive, QDate date_inscription, double volume_livraison, QDate date_livraison);
    bool supprimerAgriculteur(int cin);
    QSqlQueryModel* afficherAgriculteurs();
    QSqlQueryModel* rechercherAgriculteur(QString queryStr);
    QSqlQueryModel* trierAgriculteurs(QString critere);

private slots:
    void on_btn_login_3_clicked();

    void on_btn_home_2_clicked();

    void on_btn_employes_2_clicked();

    void on_btn_clients_2_clicked();

    void on_btn_production_2_clicked();

    void on_btn_ventes_2_clicked();

    void on_btn_ajouter_clicked();

    void on_btn_ajouter_2_clicked();

    void on_btn_modifier_agri_clicked();
    void on_btn_supprimer_agri_clicked();
    void on_btn_recherche_agri_clicked();
    void on_btn_tri_agri_clicked();

    void on_TABLEAG_clicked(const QModelIndex &index);
    void on_TABLEAG_doubleClicked(const QModelIndex &index);

    void on_btn_modifier_clicked();

    void on_metier_clicked();

    void on_analyse_clicked();

    void on_chatbot_clicked();

    void on_pdf_clicked();

    void on_btn_annuler_3_clicked();

    void on_btn_retour_3_clicked();

    void on_btn_agriculteurs_2_clicked();

    void on_btn_agriculteurs_13_clicked();

    void on_btn_clients_13_clicked();

    void on_btn_employes_13_clicked();

    void on_btn_home_13_clicked();

    void on_btn_production_13_clicked();

    void on_btn_ventes_13_clicked();

    void on_btn_agriculteurs_5_clicked();

    void on_btn_clients_5_clicked();

    void on_btn_employes_5_clicked();

    void on_btn_home_5_clicked();

    void on_btn_production_5_clicked();

    void on_btn_ventes_5_clicked();

    void on_btn_agriculteurs_8_clicked();

    void on_btn_clients_8_clicked();

    void on_btn_employes_8_clicked();

    void on_btn_home_8_clicked();

    void on_btn_production_8_clicked();

    void on_btn_ventes_8_clicked();

    void on_btn_agriculteurs_4_clicked();

    void on_btn_clients_4_clicked();

    void on_btn_employes_4_clicked();

    void on_btn_home_4_clicked();

    void on_btn_production_4_clicked();

    void on_btn_ventes_4_clicked();

    void on_btn_agriculteurs_10_clicked();

    void on_btn_clients_10_clicked();

    void on_btn_employes_10_clicked();

    void on_btn_home_10_clicked();

    void on_btn_production_10_clicked();

    void on_btn_ventes_10_clicked();

    void on_btn_agriculteurs_7_clicked();

    void on_btn_clients_7_clicked();

    void on_btn_employes_7_clicked();

    void on_btn_home_7_clicked();

    void on_btn_production_7_clicked();

    void on_btn_ventes_7_clicked();

    void on_btn_agriculteurs_3_clicked();

    void on_btn_clients_3_clicked();

    void on_btn_employes_3_clicked();

    void on_btn_home_3_clicked();

    void on_btn_production_3_clicked();

    void on_btn_ventes_3_clicked();

    void on_btn_agriculteurs_9_clicked();

    void on_btn_clients_9_clicked();

    void on_btn_employes_9_clicked();

    void on_btn_home_9_clicked();

    void on_btn_production_9_clicked();

    void on_btn_ventes_9_clicked();

    void on_btn_agriculteurs_6_clicked();

    void on_btn_clients_6_clicked();

    void on_btn_employes_6_clicked();

    void on_btn_home_6_clicked();

    void on_btn_production_6_clicked();

    void on_btn_ventes_6_clicked();

    void on_btn_agriculteurs_11_clicked();

    void on_btn_clients_11_clicked();

    void on_btn_employes_11_clicked();

    void on_btn_home_11_clicked();

    void on_btn_production_11_clicked();

    void on_btn_ventes_11_clicked();

    void on_btn_agriculteurs_12_clicked();

    void on_btn_clients_12_clicked();

    void on_btn_employes_12_clicked();

    void on_btn_home_12_clicked();

    void on_btn_production_12_clicked();

    void on_btn_ventes_12_clicked();

    void on_connecter_clicked();
    void on_historique_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_21_clicked();

    void on_btn_retour_5_clicked();

    void on_btn_retour_6_clicked();

    void on_btn_retour_7_clicked();

    void on_btn_retour_8_clicked();

    void on_btn_retour_9_clicked();

    void on_btn_retour_10_clicked();

    void on_btn_retour_4_clicked();

    void on_btn_annuler_4_clicked();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
