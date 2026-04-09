#include "client.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QDebug>

// ================= CONSTRUCTEURS =================
Client::Client() : id(0), total_achat("0"), points_fidelite("0") {}

Client::Client(QString nom, QString prenom,
               QString telephone, QString email, QString adresse,
               QDate date, QString type, QString huile,
               QString emballage, QString total, QString point)
{
    this->id                 = 0;  // auto-increment, ignoré
    this->nom                = nom;
    this->prenom             = prenom;
    this->telephone          = telephone;
    this->email              = email;
    this->adresse            = adresse;
    this->date_inscription   = date;
    this->type_client        = type;
    this->type_huile_prefere = huile;
    this->emballage_prefere  = emballage;
    this->total_achat        = total;
    this->points_fidelite    = point;
}

Client::Client(int id, QString nom, QString prenom,
               QString telephone, QString email, QString adresse,
               QDate date, QString type, QString huile,
               QString emballage, QString total, QString point)
{
    this->id                 = id;
    this->nom                = nom;
    this->prenom             = prenom;
    this->telephone          = telephone;
    this->email              = email;
    this->adresse            = adresse;
    this->date_inscription   = date;
    this->type_client        = type;
    this->type_huile_prefere = huile;
    this->emballage_prefere  = emballage;
    this->total_achat        = total;
    this->points_fidelite    = point;
}

// ================= AJOUTER =================
bool Client::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO SMART.CLIENT "
                  "(NOM, PRENOM, TELEPHONE, EMAIL, ADRESSE, DATE_INSCRIPTION, "
                  "TYPE_CLIENT, TYPE_HUILE_PREFERE, EMBALLAGE_PREFERE, "
                  "TOTAL_ACHAT, POINTS_FIDELITE) "
                  "VALUES (:nom, :prenom, :telephone, :email, :adresse, :date, "
                  ":type_client, :type_huile, :emballage, :total, :points)");

    query.bindValue(":nom",         nom);
    query.bindValue(":prenom",      prenom);
    query.bindValue(":telephone",   telephone);
    query.bindValue(":email",       email);
    query.bindValue(":adresse",     adresse);
    query.bindValue(":date",        date_inscription);
    query.bindValue(":type_client", type_client);
    query.bindValue(":type_huile",  type_huile_prefere);
    query.bindValue(":emballage",   emballage_prefere);
    query.bindValue(":total",       total_achat.isEmpty() ? "0" : total_achat);
    query.bindValue(":points",      points_fidelite.isEmpty() ? "0" : points_fidelite);

    if (!query.exec()) {
        qDebug() << "[CLIENT] Erreur ajout :" << query.lastError().text();
        return false;
    }
    return true;
}

// ================= MODIFIER =================
bool Client::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE SMART.CLIENT SET "
                  "NOM=:nom, PRENOM=:prenom, TELEPHONE=:telephone, EMAIL=:email, "
                  "ADRESSE=:adresse, DATE_INSCRIPTION=:date, TYPE_CLIENT=:type_client, "
                  "TYPE_HUILE_PREFERE=:type_huile, EMBALLAGE_PREFERE=:emballage, "
                  "TOTAL_ACHAT=:total, POINTS_FIDELITE=:points "
                  "WHERE ID=:id");

    query.bindValue(":id",          id);
    query.bindValue(":nom",         nom);
    query.bindValue(":prenom",      prenom);
    query.bindValue(":telephone",   telephone);
    query.bindValue(":email",       email);
    query.bindValue(":adresse",     adresse);
    query.bindValue(":date",        date_inscription);
    query.bindValue(":type_client", type_client);
    query.bindValue(":type_huile",  type_huile_prefere);
    query.bindValue(":emballage",   emballage_prefere);
    query.bindValue(":total",       total_achat);
    query.bindValue(":points",      points_fidelite);

    if (!query.exec()) {
        qDebug() << "[CLIENT] Erreur modifier :" << query.lastError().text();
        return false;
    }
    return true;
}

// ================= SUPPRIMER =================
bool Client::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM SMART.CLIENT WHERE ID=:id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "[CLIENT] Erreur suppression :" << query.lastError().text();
        return false;
    }
    return true;
}

// ================= AFFICHER =================
QSqlQueryModel* Client::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT ID, NOM, PRENOM, TELEPHONE, EMAIL, ADRESSE, "
                  "DATE_INSCRIPTION, TYPE_CLIENT, TYPE_HUILE_PREFERE, "
                  "EMBALLAGE_PREFERE, TOTAL_ACHAT, POINTS_FIDELITE "
                  "FROM SMART.CLIENT ORDER BY ID");
    query.exec();
    model->setQuery(std::move(query));

    model->setHeaderData(0,  Qt::Horizontal, "ID");
    model->setHeaderData(1,  Qt::Horizontal, "Nom");
    model->setHeaderData(2,  Qt::Horizontal, "Prénom");
    model->setHeaderData(3,  Qt::Horizontal, "Téléphone");
    model->setHeaderData(4,  Qt::Horizontal, "Email");
    model->setHeaderData(5,  Qt::Horizontal, "Adresse");
    model->setHeaderData(6,  Qt::Horizontal, "Date inscription");
    model->setHeaderData(7,  Qt::Horizontal, "Type client");
    model->setHeaderData(8,  Qt::Horizontal, "Huile préférée");
    model->setHeaderData(9,  Qt::Horizontal, "Emballage");
    model->setHeaderData(10, Qt::Horizontal, "Total achat");
    model->setHeaderData(11, Qt::Horizontal, "Points fidélité");

    return model;
}
