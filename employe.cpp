#include "employe.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Employe::Employe()
{
}

Employe::Employe(QString cin, QString nom, QString prenom,
                 QDate date_naissance, QString situation_familiale,
                 QString email,
                 QString adresse, QString code_postal,
                 QString departement, QString type_contrat,
                 QString telephone, QString poste,
                 QDate date_embauche,
                 double salaire, double primes, QString statut)
{
    this->cin = cin;
    this->nom = nom;
    this->prenom = prenom;
    this->date_naissance = date_naissance;
    this->situation_familiale = situation_familiale;
    this->email = email;

    this->adresse = adresse;
    this->code_postal = code_postal;
    this->departement = departement;
    this->type_contrat = type_contrat;
    this->telephone = telephone;
    this->poste = poste;
    this->date_embauche = date_embauche;
    this->salaire = salaire;
    this->primes = primes;
    this->statut = statut;
}

///////////////////////////////////////////////////////////
/// AJOUTER
///////////////////////////////////////////////////////////

bool Employe::ajouter()
{
    QSqlDatabase db = Connection::getInstance()->getDatabase();

    // N7ottou el khedma f wast bloc {} bech el query tetfasa5 mel RAM ba3d ma tikhdem
    {
        QSqlQuery query(db);
        query.prepare("INSERT INTO EMPLOYE (CIN, NOM, PRENOM, DATE_NAISSANCE, SITUATION_FAMILIALE, "
                      " ADRESSE, CODE_POSTAL,TELEPHONE,EMAIL, DEPARTEMENT, TYPE_CONTRAT,  POSTE, "
                      "DATE_EMBAUCHE, SALAIRE, PRIMES, STATUT) "
                      "VALUES (:cin, :nom, :prenom, :date_n, :sit, :email, :adr, :cp, :dep, :type, :tel, :poste, :date_e, :sal, :pri, :stat)");

        query.bindValue(":cin", cin);
        query.bindValue(":nom", nom);
        query.bindValue(":prenom", prenom);
        query.bindValue(":date_n", date_naissance);
        query.bindValue(":sit", situation_familiale);
        query.bindValue(":adr", adresse);
        query.bindValue(":cp", code_postal);
        query.bindValue(":tel", telephone);
        query.bindValue(":email", email);
        query.bindValue(":dep", departement);
        query.bindValue(":type", type_contrat);
        query.bindValue(":poste", poste);
        query.bindValue(":date_e", date_embauche);
        query.bindValue(":sal", salaire);
        query.bindValue(":pri", primes);
        query.bindValue(":stat", statut);

        if(!query.exec()) {
            qDebug() << "Erreur SQL Detaillee :" << query.lastError().text();
            return false;
        }
    }
    return true;
}
///////////////////////////////////////////////////////////
/// AFFICHER
///////////////////////////////////////////////////////////

QSqlQueryModel* Employe::afficher()
{
    QSqlDatabase db = Connection::getInstance()->getDatabase();
    QSqlQueryModel* model = new QSqlQueryModel();

    // Na3tiw el query lel model bel db s7i7a
    QSqlQuery query(db);
    query.prepare("SELECT ID_EMP, CIN, NOM, PRENOM,DATE_NAISSANCE,SITUATION_FAMILIALE,ADRESSE,CODE_POSTAL,TELEPHONE,EMAIL,DEPARTEMENT,TYPE_CONTRAT, POSTE,DATE_EMBAUCHE, SALAIRE,PRIMES,STATUT FROM EMPLOYE");
    query.exec();

    model->setQuery(std::move(query)); // Nestamlo std::move f Qt6 bech nmarerou el natija

    // Personnalisation des colonnes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date Naissance"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Situation familiale"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Code postal"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Telephone"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Departement"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Type contrat"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date d'embauche"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Primes"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Statut"));
    return model;
}

///////////////////////////////////////////////////////////
/// SUPPRIMER
///////////////////////////////////////////////////////////

bool Employe::supprimer(int id)
{
    QSqlQuery query;

    query.prepare("DELETE FROM EMPLOYE WHERE ID_EMP = :id");
    query.bindValue(":id", id);

    if(query.exec())
    {
        qDebug() << "Suppression réussie";
        return true;
    }
    else
    {
        qDebug() << "Erreur suppression :" << query.lastError();
        return false;
    }
}

///////////////////////////////////////////////////////////
/// MODIFIER
///////////////////////////////////////////////////////////

/*bool Employe::modifier(int id_a_modifier)
{
    QSqlDatabase db = Connection::getInstance()->getDatabase();
    {
        QSqlQuery query(db);
        query.prepare("UPDATE EMPLOYE SET CIN=:cin, NOM=:nom, PRENOM=:prenom, EMAIL=:email, SALAIRE=:sal WHERE ID_EMP=:id");

        query.bindValue(":cin", cin);
        query.bindValue(":nom", nom);
        query.bindValue(":prenom", prenom);
        query.bindValue(":email", email);
        query.bindValue(":sal", salaire);
        query.bindValue(":id", id_a_modifier);

        return query.exec();
    }
}*/
