#include "client.h"
#include "ui_client.h"
#include "connection.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTableWidgetItem>
#include <QDebug>
#include <QSqlRecord>
// ================= CONSTRUCTEUR UI =================
client::client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::client)
    , model(nullptr)
{
    ui->setupUi(this);

    if (!connection::getInstance()->createConnect()) {
        qDebug() << "Erreur connexion";
    } else {
        qDebug() << "Connexion OK";
    }

    // Remplir la table au démarrage
    remplirTableau();
}

// ================= CONSTRUCTEUR AVEC PARAM =================
client::client(int id,
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
               QString point)
{
    this->id = id;
    this->nom = nom;
    this->prenom = prenom;
    this->telephone = telephone;
    this->email = email;
    this->adresse = adresse;
    this->date_inscription = date;
    this->type_client = type;
    this->type_huile_prefere = huile;
    this->emballage_prefere = emballage;
    this->total_achat = total;
    this->points_fidelite = point;
    model = nullptr;
}

// ================= AJOUTER =================
bool client::ajouter()
{
    QSqlQuery query;

    query.prepare("INSERT INTO CLIENT "
                  "(ID,NOM,PRENOM,TELEPHONE,EMAIL,ADRESSE,DATE_INSCRIPTION,"
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
        qDebug() << "Erreur ajout:" << query.lastError().text();
        return false;
    }

    // Rafraîchir la table
    remplirTableau();
    return true;
}

// ================= MODIFIER =================
bool client::modifier()
{
    QSqlQuery query;

    query.prepare("UPDATE CLIENT SET "
                  "NOM=:nom, PRENOM=:prenom, TELEPHONE=:telephone, EMAIL=:email, "
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
        qDebug() << "Erreur modification:" << query.lastError().text();
        return false;
    }

    remplirTableau();
    return true;
}

// ================= SUPPRIMER =================
bool client::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM CLIENT WHERE ID=:id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur suppression:" << query.lastError().text();
        return false;
    }

    return true;
}

// ================= REMPLIR TABLE =================
void client::remplirTableau()
{
    if (!model)
        model = new QSqlTableModel(this);

    model->setTable("CLIENT");
    if (!model->select()) {
        qDebug() << model->lastError().text();
        return;
    }

    // Définir les en-têtes de QTableWidget
    ui->table->setColumnCount(model->columnCount());
    QStringList headers;
    headers << "ID" << "Nom" << "Prénom" << "Téléphone" << "Email"
            << "Adresse" << "Date inscription" << "Type client"
            << "Huile préférée" << "Emballage" << "Total achat" << "Points fidélité";
    ui->table->setHorizontalHeaderLabels(headers);

    // Remplir les lignes
    ui->table->setRowCount(model->rowCount());
    for (int i = 0; i < model->rowCount(); ++i) {
        for (int j = 0; j < model->columnCount(); ++j) {
            QTableWidgetItem *item = new QTableWidgetItem(
                model->record(i).value(j).toString()
                );
            ui->table->setItem(i, j, item);
        }
    }

    ui->table->resizeColumnsToContents();
}

// ================= DESTRUCTEUR =================
client::~client()
{
    delete ui;
}

// ================= NAVIGATION =================
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
    ui->stacked->setCurrentWidget(ui->page);
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
