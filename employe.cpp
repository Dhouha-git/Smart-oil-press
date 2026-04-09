#include "employe.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Employe::Employe()
{
}

Employe::Employe(QString cin, QString nom, QString prenom,
                 QString date_naissance, QString situation_familiale,QString email,
                 QString adresse, QString code_postal,
                 QString telephone,
                 QString departement, QString type_contrat,
                 QString poste,
                 QString date_embauche,
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
    this->telephone = telephone;
    this->departement = departement;
    this->type_contrat = type_contrat;
    this->poste = poste;
    this->date_embauche = date_embauche;
    this->salaire = salaire;
    this->primes = primes;
    this->statut = statut;
}
//getters
QString Employe::getCin() { return cin; }
QString Employe::getNom() { return nom; }
QString Employe::getPrenom() { return prenom; }
QString Employe::getDateNaissance() { return date_naissance; }
QString Employe::getSituation() { return situation_familiale; }
QString Employe::getEmail() { return email; }
QString Employe::getAdresse() { return adresse; }
QString Employe::getCodePostal() { return code_postal; }
QString Employe::getTelephone() { return telephone; }
QString Employe::getDepartement() { return departement; }
QString Employe::getTypeContrat() { return type_contrat; }
QString Employe::getPoste() { return poste; }
QString Employe::getDateEmbauche() { return date_embauche; }
double Employe::getSalaire() { return salaire; }
double Employe::getPrimes() { return primes; }
QString Employe::getStatut() { return statut; }
//setters
void Employe::setCin(QString cin) { this->cin = cin; }
void Employe::setNom(QString nom) { this->nom = nom; }
void Employe::setPrenom(QString prenom) { this->prenom = prenom; }
void Employe::setDateNaissance(QString d) { this->date_naissance = d; }
void Employe::setSituation(QString s) { this->situation_familiale = s; }
void Employe::setEmail(QString e) { this->email = e; }
void Employe::setAdresse(QString a) { this->adresse = a; }
void Employe::setCodePostal(QString cp) { this->code_postal = cp; }
void Employe::setTelephone(QString t) { this->telephone = t; }
void Employe::setDepartement(QString d) { this->departement = d; }
void Employe::setTypeContrat(QString t) { this->type_contrat = t; }
void Employe::setPoste(QString p) { this->poste = p; }
void Employe::setDateEmbauche(QString d) { this->date_embauche = d; }
void Employe::setSalaire(double s) { this->salaire = s; }
void Employe::setPrimes(double p) { this->primes = p; }
void Employe::setStatut(QString s) { this->statut = s; }
///////////////////////////////////////////////////////////
/// AJOUTER
///////////////////////////////////////////////////////////

bool Employe::ajouter()
{
    QSqlDatabase db = Connection::getInstance()->getDatabase();

    {
        QSqlQuery query(db);
        query.prepare("INSERT INTO EMPLOYE (CIN, NOM, PRENOM, DATE_NAISSANCE, SITUATION_FAMILIALE,EMAIL, "
                      " ADRESSE, CODE_POSTAL,TELEPHONE, DEPARTEMENT, TYPE_CONTRAT,  POSTE, "
                      "DATE_EMBAUCHE, SALAIRE, PRIMES, STATUT) "
                      "VALUES (:cin, :nom, :prenom, :date_n, :sit, :email, :adr, :cp, :tel, :dep, :type, :poste, :date_e, :sal, :pri, :stat)");

        query.bindValue(":cin", cin);
        query.bindValue(":nom", nom);
        query.bindValue(":prenom", prenom);
        query.bindValue(":date_n", date_naissance);
        query.bindValue(":sit", situation_familiale);
        query.bindValue(":email", email);
        query.bindValue(":adr", adresse);
        query.bindValue(":cp", code_postal);
        query.bindValue(":tel", telephone);
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


    QSqlQuery query(db);
    query.prepare("SELECT ID_EMP, CIN, NOM, PRENOM,DATE_NAISSANCE,SITUATION_FAMILIALE,EMAIL,ADRESSE,CODE_POSTAL,TELEPHONE,DEPARTEMENT,TYPE_CONTRAT, POSTE,DATE_EMBAUCHE, SALAIRE,PRIMES,STATUT FROM EMPLOYE");
    query.exec();

    model->setQuery(std::move(query));

    // Personnalisation des colonnes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date Naissance"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Situation familiale"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Code postal"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Telephone"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("Departement"));
    model->setHeaderData(11, Qt::Horizontal, QObject::tr("Type contrat"));
    model->setHeaderData(12, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(13, Qt::Horizontal, QObject::tr("Date d'embauche"));
    model->setHeaderData(14, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(15, Qt::Horizontal, QObject::tr("Primes"));
    model->setHeaderData(16, Qt::Horizontal, QObject::tr("Statut"));
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

bool Employe::modifier(int id)
{
    QSqlQuery query;

    query.prepare("UPDATE EMPLOYE SET "
                  "CIN=:cin, NOM=:nom, PRENOM=:prenom, "
                  "DATE_NAISSANCE=:date_n, SITUATION_FAMILIALE=:sit, "
                  "EMAIL=:email, ADRESSE=:adr, CODE_POSTAL=:cp, TELEPHONE=:tel, "
                  "DEPARTEMENT=:dep, TYPE_CONTRAT=:type, POSTE=:poste, "
                  "DATE_EMBAUCHE=:date_e, SALAIRE=:sal, PRIMES=:pri, STATUT=:stat "
                  "WHERE ID_EMP=:id");

    query.bindValue(":id", id);
    query.bindValue(":cin", cin);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":date_n", date_naissance);
    query.bindValue(":sit", situation_familiale);
    query.bindValue(":email", email);
    query.bindValue(":adr", adresse);
    query.bindValue(":cp", code_postal);
    query.bindValue(":tel", telephone);
    query.bindValue(":dep", departement);
    query.bindValue(":type", type_contrat);
    query.bindValue(":poste", poste);
    query.bindValue(":date_e", date_embauche);
    query.bindValue(":sal", salaire);
    query.bindValue(":pri", primes);
    query.bindValue(":stat", statut);

    return query.exec();
}
QSqlQueryModel* Employe::rechercherAvance(QString val, QString filtre, QString tri)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    /// 🔄 mapping filtre
    if(filtre == "NOM") filtre = "NOM";
    else if(filtre == "CIN") filtre = "CIN";
    else if(filtre == "PRENOM") filtre = "PRENOM";
    else if(filtre == "DEPARTEMENT") filtre = "DEPARTEMENT";
    else if(filtre == "POSTE") filtre = "POSTE";

    /// 🔄 mapping tri
    if(tri == "ID ASC") tri = "ID_EMP ASC";
    else if(tri == "ID DESC") tri = "ID_EMP DESC";
    else if(tri == "NOM ASC") tri = "NOM ASC";
    else if(tri == "NOM DESC") tri = "NOM DESC";
    else if(tri == "SALAIRE ASC") tri = "SALAIRE ASC";
    else if(tri == "SALAIRE DESC") tri = "SALAIRE DESC";

    /// 🧠 query
    QString query = "SELECT * FROM EMPLOYE WHERE " + filtre +
                    " LIKE '%" + val + "%' ORDER BY " + tri;

    model->setQuery(query);

    return model;
}
