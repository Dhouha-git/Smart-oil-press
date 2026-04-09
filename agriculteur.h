#ifndef AGRICULTEUR_H
#define AGRICULTEUR_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Agriculteur
{
private:
    int     cin;
    QString nom;
    QString prenom;
    int     telephone;
    QString email;
    QString region;
    QString type_olive;
    QDate   date_inscription;
    double  volume_livraison;
    QDate   date_livraison;

public:
    Agriculteur();
    Agriculteur(int cin, QString nom, QString prenom,
                int telephone, QString email, QString region,
                QString type_olive, QDate date_inscription,
                double volume_livraison, QDate date_livraison);

    // Getters
    int     getCin()              const { return cin; }
    QString getNom()              const { return nom; }
    QString getPrenom()           const { return prenom; }
    int     getTelephone()        const { return telephone; }
    QString getEmail()            const { return email; }
    QString getRegion()           const { return region; }
    QString getTypeOlive()        const { return type_olive; }
    QDate   getDateInscription()  const { return date_inscription; }
    double  getVolumeLivraison()  const { return volume_livraison; }
    QDate   getDateLivraison()    const { return date_livraison; }

    // CRUD
    bool ajouter();
    bool modifier();
    bool supprimer();
    static bool supprimer(int cin);
    QSqlQueryModel* afficher();
    QSqlQueryModel* rechercher(QString val);
    QSqlQueryModel* trier(QString critere);
};

#endif // AGRICULTEUR_H
