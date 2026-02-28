#include "client.h"
#include "ui_client.h"
#include "connection.h"
client::client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::client)
{
    ui->setupUi(this);

    // Vérifie si la connexion est ouverte
    if (!connection::instance()->createConnect()) {
        qDebug() << "Erreur: Impossible de se connecter à la base!";
    } else {
        qDebug() << "Base ouverte avec succès";
    }
}
bool client::ajouter()
{
    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "Database is not open! Cannot execute insert.";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO CLIENT(ID,NOM,PRENOM,TELEPHONE,EMAIL,ADRESSE,DATE_INSCRIPTION,"
                  "TYPE_CLIENT,TYPE_HUILE_PREFERE,EMBALLAGE_PREFERE,TOTAL_ACHAT,POINTS_FIDELITE) "
                  "VALUES (:id,:nom,:prenom,:telephone,:email,:adresse,:date_inscription,"
                  ":type_client,:type_huile_prefere,:emballage_prefere,:total_achat,:points_fidelite)");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":telephone", telephone);
    query.bindValue(":email", email);
    query.bindValue(":adresse", adresse);
    query.bindValue(":date_inscription", date_inscription);
    query.bindValue(":type_client", type_client);
    query.bindValue(":type_huile_prefere", type_huile_prefere);
    query.bindValue(":emballage_prefere", emballage_prefere);
    query.bindValue(":total_achat", total_achat);
    query.bindValue(":points_fidelite", points_fidelite);

    if (!query.exec()) {
        qDebug() << "Erreur dans l'ajout:" << query.lastError().text();
        return false;
    } else {
        qDebug() << "Ajout avec succès";
        return true;
    }
}

// ===================== MODIFIER =====================
bool client::modifier()
{
    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "Database is not open! Cannot execute update.";
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE CLIENT SET NOM=:nom, PRENOM=:prenom, TELEPHONE=:telephone, EMAIL=:email, "
                  "ADRESSE=:adresse, DATE_INSCRIPTION=:date_inscription, TYPE_CLIENT=:type_client, "
                  "TYPE_HUILE_PREFERE=:type_huile_prefere, EMBALLAGE_PREFERE=:emballage_prefere, "
                  "TOTAL_ACHAT=:total_achat, POINTS_FIDELITE=:points_fidelite "
                  "WHERE ID=:id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":telephone", telephone);
    query.bindValue(":email", email);
    query.bindValue(":adresse", adresse);
    query.bindValue(":date_inscription", date_inscription);
    query.bindValue(":type_client", type_client);
    query.bindValue(":type_huile_prefere", type_huile_prefere);
    query.bindValue(":emballage_prefere", emballage_prefere);
    query.bindValue(":total_achat", total_achat);
    query.bindValue(":points_fidelite", points_fidelite);

    if (!query.exec()) {
        qDebug() << "Erreur dans la modification:" << query.lastError().text();
        return false;
    } else {
        qDebug() << "Modification avec succès";
        return true;
    }
}

// ===================== SUPPRIMER =====================
bool client::supprimer(int id)
{
    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "Database is not open! Cannot execute delete.";
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM CLIENT WHERE ID=:id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur dans la suppression:" << query.lastError().text();
        return false;
    } else {
        qDebug() << "Suppression avec succès";
        return true;
    }
}

// ===================== AFFICHER =====================
void client::afficher(QSqlTableModel *model)
{
    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "Database is not open! Cannot load data.";
        return;
    }

    model->setTable("CLIENT");
    if (!model->select()) {
        qDebug() << "Erreur lors du chargement des données:" << model->lastError().text();
    } else {
        qDebug() << "Chargement de données réussi. Nombre de lignes:" << model->rowCount();
    }
}

client::~client()
{
    delete ui;
}

void client::on_analyse_clicked()
{
    ui->stacked->setCurrentWidget(ui->page_2);
}


void client::on_chatbot_clicked()
{
    ui->stacked->setCurrentWidget(ui->page_3);
}


void client::on_retour_clicked()
{

}


void client::on_historique_clicked()
{
    ui->stacked->setCurrentWidget(ui->page_4);
}


void client::on_classement_clicked()
{
    ui->stacked->setCurrentWidget(ui->page_5);
}


void client::on_pushButton_3_clicked()
{
    ui->stacked->setCurrentWidget(ui->page);
}


void client::on_pushButton_5_clicked()
{
      ui->stacked->setCurrentWidget(ui->page_2);
}


void client::on_pushButton_6_clicked()
{
    ui->stacked->setCurrentWidget(ui->page_2);

}

