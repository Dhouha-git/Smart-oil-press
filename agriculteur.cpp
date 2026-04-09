#include "agriculteur.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QDebug>

Agriculteur::Agriculteur() : cin(0), telephone(0), volume_livraison(0) {}

Agriculteur::Agriculteur(int cin, QString nom, QString prenom,
                         int telephone, QString email, QString region,
                         QString type_olive, QDate date_inscription,
                         double volume_livraison, QDate date_livraison)
{
    this->cin               = cin;
    this->nom               = nom;
    this->prenom            = prenom;
    this->telephone         = telephone;
    this->email             = email;
    this->region            = region;
    this->type_olive        = type_olive;
    this->date_inscription  = date_inscription;
    this->volume_livraison  = volume_livraison;
    this->date_livraison    = date_livraison;
}

bool Agriculteur::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO AGRICULTEUR "
                  "(CIN, NOM, PRENOM, TELEPHONE, EMAIL, REGION, TYPE_OLIVE, "
                  "DATE_INSCRIPTION, VOLUME_LIVRAISON, DATE_LIVRAISON) "
                  "VALUES (:cin, :nom, :prenom, :telephone, :email, :region, "
                  ":type_olive, :date_inscription, :volume_livraison, :date_livraison)");

    query.bindValue(":cin",              cin);
    query.bindValue(":nom",              nom);
    query.bindValue(":prenom",           prenom);
    query.bindValue(":telephone",        telephone);
    query.bindValue(":email",            email);
    query.bindValue(":region",           region);
    query.bindValue(":type_olive",       type_olive);
    query.bindValue(":date_inscription", date_inscription);
    query.bindValue(":volume_livraison", volume_livraison);
    query.bindValue(":date_livraison",   date_livraison);

    if (!query.exec()) {
        qDebug() << "[AGRICULTEUR] Erreur ajout:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Agriculteur::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE AGRICULTEUR SET "
                  "NOM=:nom, PRENOM=:prenom, TELEPHONE=:telephone, "
                  "EMAIL=:email, REGION=:region, TYPE_OLIVE=:type_olive, "
                  "DATE_INSCRIPTION=:date_inscription, "
                  "VOLUME_LIVRAISON=:volume_livraison, "
                  "DATE_LIVRAISON=:date_livraison "
                  "WHERE CIN=:cin");

    query.bindValue(":cin",              cin);
    query.bindValue(":nom",              nom);
    query.bindValue(":prenom",           prenom);
    query.bindValue(":telephone",        telephone);
    query.bindValue(":email",            email);
    query.bindValue(":region",           region);
    query.bindValue(":type_olive",       type_olive);
    query.bindValue(":date_inscription", date_inscription);
    query.bindValue(":volume_livraison", volume_livraison);
    query.bindValue(":date_livraison",   date_livraison);

    if (!query.exec()) {
        qDebug() << "[AGRICULTEUR] Erreur modifier:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Agriculteur::supprimer(int cin)
{
    QSqlQuery query;
    query.prepare("DELETE FROM AGRICULTEUR WHERE CIN=:cin");
    query.bindValue(":cin", cin);

    if (!query.exec()) {
        qDebug() << "[AGRICULTEUR] Erreur suppression:" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel* Agriculteur::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT CIN, NOM, PRENOM, TELEPHONE, EMAIL, REGION, "
                  "TYPE_OLIVE, DATE_INSCRIPTION, VOLUME_LIVRAISON, DATE_LIVRAISON "
                  "FROM AGRICULTEUR ORDER BY CIN");
    query.exec();
    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, "CIN");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Téléphone");
    model->setHeaderData(4, Qt::Horizontal, "Email");
    model->setHeaderData(5, Qt::Horizontal, "Région");
    model->setHeaderData(6, Qt::Horizontal, "Type Olive");
    model->setHeaderData(7, Qt::Horizontal, "Date Inscription");
    model->setHeaderData(8, Qt::Horizontal, "Volume Livraison");
    model->setHeaderData(9, Qt::Horizontal, "Date Livraison");

    return model;
}

QSqlQueryModel* Agriculteur::rechercher(QString val)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT CIN, NOM, PRENOM, TELEPHONE, EMAIL, REGION, "
                  "TYPE_OLIVE, DATE_INSCRIPTION, VOLUME_LIVRAISON, DATE_LIVRAISON "
                  "FROM AGRICULTEUR "
                  "WHERE UPPER(NOM) LIKE UPPER(:val) "
                  "OR UPPER(PRENOM) LIKE UPPER(:val) "
                  "OR UPPER(REGION) LIKE UPPER(:val) "
                  "OR TO_CHAR(CIN) LIKE :val");
    query.bindValue(":val", "%" + val + "%");
    query.exec();
    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, "CIN");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Téléphone");
    model->setHeaderData(4, Qt::Horizontal, "Email");
    model->setHeaderData(5, Qt::Horizontal, "Région");
    model->setHeaderData(6, Qt::Horizontal, "Type Olive");
    model->setHeaderData(7, Qt::Horizontal, "Date Inscription");
    model->setHeaderData(8, Qt::Horizontal, "Volume Livraison");
    model->setHeaderData(9, Qt::Horizontal, "Date Livraison");

    return model;
}

QSqlQueryModel* Agriculteur::trier(QString critere)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT CIN, NOM, PRENOM, TELEPHONE, EMAIL, REGION, "
                    "TYPE_OLIVE, DATE_INSCRIPTION, VOLUME_LIVRAISON, DATE_LIVRAISON "
                    "FROM AGRICULTEUR ORDER BY " + critere);

    model->setHeaderData(0, Qt::Horizontal, "CIN");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Téléphone");
    model->setHeaderData(4, Qt::Horizontal, "Email");
    model->setHeaderData(5, Qt::Horizontal, "Région");
    model->setHeaderData(6, Qt::Horizontal, "Type Olive");
    model->setHeaderData(7, Qt::Horizontal, "Date Inscription");
    model->setHeaderData(8, Qt::Horizontal, "Volume Livraison");
    model->setHeaderData(9, Qt::Horizontal, "Date Livraison");

    return model;
}
