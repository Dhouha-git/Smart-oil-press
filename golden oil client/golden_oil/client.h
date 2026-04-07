#ifndef CLIENT_H
#define CLIENT_H


#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>
#include <QString>  // pour QString
#include <QDate>
#include <QSqlTableModel>
#include <QSqlError>
QT_BEGIN_NAMESPACE
namespace Ui {
class client;

}
QT_END_NAMESPACE

class client : public QMainWindow
{
    Q_OBJECT

public:
    client(QWidget *parent = nullptr);
    ~client();

private slots:
    void on_analyse_clicked();

    void on_chatbot_clicked();

    void on_retour_clicked();

    void on_historique_clicked();

    void on_classement_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();
private slots:
    void on_ajouter_clicked();
    void on_modifier_clicked();
    void on_recherche1_2_clicked();
void on_tri_2_clicked();
    void on_annuler_clicked();
void on_annuler1_clicked();


    void on_supprimer_clicked();







private:
    Ui::client *ui;
private:
    void verifierSaisie();

    // ---- Attributs du client ----
    int id;
    QString nom;
    QString prenom;
    QString telephone;
    QString email;
    QString adresse;
    QDate date_inscription;
    QString type_client;
    QString type_huile_prefere;
    QString emballage_prefere;
    QString total_achat;
    QString points_fidelite;
    QSqlTableModel *model;

public:
    client(    int id,
           QString nom,
           QString prenom,
           QString telephone,
           QString email,
           QString adresse,
           QDate date,
           QString type,
           QString huile,
           QString emballage,
           QString total,
           QString point);
    bool ajouter();
    bool modifier();
    static bool supprimer(int id);
    static void afficher(QSqlTableModel *model);
      void remplirTableau();

};
#endif // CLIENT_H
