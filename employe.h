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
    QString date_naissance;
    QString situation_familiale;
    QString email;
    QString adresse;
    QString code_postal;
    QString telephone;
    QString departement;
    QString type_contrat;
    QString poste;
    QString date_embauche;
    double salaire;
    double primes;
    QString statut;

public:
    Employe();
    Employe(QString cin, QString nom, QString prenom,
            QString date_naissance, QString situation_familiale,
            QString email, QString adresse, QString code_postal,
            QString telephone,
            QString departement, QString type_contrat,
             QString poste,
            QString date_embauche,
            double salaire, double primes, QString statut);
    /// ===== GETTERS =====
    QString getCin();
    QString getNom();
    QString getPrenom();
    QString getDateNaissance();
    QString getSituation();
    QString getEmail();
    QString getAdresse();
    QString getCodePostal();
    QString getTelephone();
    QString getDepartement();
    QString getTypeContrat();
    QString getPoste();
    QString getDateEmbauche();
    double getSalaire();
    double getPrimes();
    QString getStatut();

    /// ===== SETTERS =====
    void setCin(QString cin);
    void setNom(QString nom);
    void setPrenom(QString prenom);
    void setDateNaissance(QString d);
    void setSituation(QString s);
    void setEmail(QString e);
    void setAdresse(QString a);
    void setCodePostal(QString cp);
    void setTelephone(QString t);
    void setDepartement(QString d);
    void setTypeContrat(QString t);
    void setPoste(QString p);
    void setDateEmbauche(QString d);
    void setSalaire(double s);
    void setPrimes(double p);
    void setStatut(QString s);


    bool ajouter();
    bool supprimer(int id);
    bool modifier(int id);
    QSqlQueryModel* afficher();
    QSqlQueryModel* rechercherAvance(QString val, QString filtre, QString tri);
};

#endif
