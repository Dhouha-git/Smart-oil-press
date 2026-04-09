#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Client
{
private:
    int     id;
    QString nom;
    QString prenom;
    QString telephone;
    QString email;
    QString adresse;
    QDate   date_inscription;
    QString type_client;
    QString type_huile_prefere;
    QString emballage_prefere;
    QString total_achat;
    QString points_fidelite;

public:
    // Constructeur vide
    Client();

    // Constructeur sans ID (auto-increment)
    Client(QString nom, QString prenom,
           QString telephone, QString email, QString adresse,
           QDate date, QString type, QString huile,
           QString emballage, QString total, QString point);

    // Constructeur avec ID (pour modifier/supprimer)
    Client(int id, QString nom, QString prenom,
           QString telephone, QString email, QString adresse,
           QDate date, QString type, QString huile,
           QString emballage, QString total, QString point);

    // Getters
    int     getId()              const { return id; }
    QString getNom()             const { return nom; }
    QString getPrenom()          const { return prenom; }
    QString getTelephone()       const { return telephone; }
    QString getEmail()           const { return email; }
    QString getAdresse()         const { return adresse; }
    QDate   getDateInscription() const { return date_inscription; }
    QString getTypeClient()      const { return type_client; }
    QString getTypeHuile()       const { return type_huile_prefere; }
    QString getEmballage()       const { return emballage_prefere; }
    QString getTotalAchat()      const { return total_achat; }
    QString getPointsFidelite()  const { return points_fidelite; }

    // CRUD
    bool ajouter();
    bool modifier();
    static bool supprimer(int id);
    QSqlQueryModel* afficher();
};

#endif // CLIENT_H
