#ifndef EMPLOYE_H
#define EMPLOYE_H
#include <QSqlQuery>
#include <QString>
#include <QDate>
#include <QSqlQueryModel>

class Employe
{
private:
    int id;
    QString cin;
    QString nom;
    QString prenom;
    QDate date_naissance;
    QString situation_familiale;
    QString email;
    QString adresse;
    QString code_postal;
    QString departement;
    QString type_contrat;
    QString telephone;
    QString poste;
    QDate date_embauche;
    double salaire;
    double primes;
    QString statut;

public:
    Employe();
    Employe(QString cin, QString nom, QString prenom,
            QDate date_naissance, QString situation_familiale,
            QString email, QString adresse, QString code_postal,
            QString departement, QString type_contrat,
            QString telephone, QString poste,
            QDate date_embauche,
            double salaire, double primes, QString statut);

    bool ajouter();
    bool supprimer(int id);
    bool modifier(int id);
    QSqlQueryModel* afficher();
};

#endif
