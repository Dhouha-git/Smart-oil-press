#ifndef AGRICULTEUR_H
#define AGRICULTEUR_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMap>
#include <QList>
#include <QPair>

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

    // ===== GETTERS =====
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

    // ===== CRUD =====
    bool ajouter();
    bool modifier();
    bool supprimer();
    static bool supprimer(int cin);
    QSqlQueryModel* afficher();
    QSqlQueryModel* rechercher(QString val);
    QSqlQueryModel* trier(QString critere);

    // ===== METIERS AVANCEES =====

    // 1. Statistiques : volume total par région
    // Retourne map<region, volume_total>
    static QMap<QString, double> volumeParRegion();

    // 2. Statistiques : nombre d'agriculteurs par type d'olive
    // Retourne map<type_olive, count>
    static QMap<QString, int> countParTypeOlive();

    // 3. Classement : top N agriculteurs par volume livré
    // Retourne liste de (cin, nom, prenom, region, volume)
    static QList<QMap<QString,QString>> topAgriculteurs(int n = 10);

    // 4. Alertes : agriculteurs avec volume < seuil
    // Retourne QSqlQueryModel filtré
    static QSqlQueryModel* alertesVolumefaible(double seuil = 100.0);

    // 5. Alertes : agriculteurs dont la date de livraison est dépassée
    static QSqlQueryModel* alertesDateDepassee();

    // 6. Alertes : agriculteurs inscrits depuis plus de X jours sans livraison récente
    static QSqlQueryModel* alertesInactifs(int joursInactivite = 180);

    // 7. Statistiques globales (pour dashboard)
    // Retourne map avec: total, volume_total, volume_moyen, nb_regions
    static QMap<QString, double> statsGlobales();

    // 8. Export PDF complet
    // Retourne true si succès
    static bool exporterPDF(const QString& cheminFichier);

    // 9. Agriculteurs par région (pour filtre)
    static QSqlQueryModel* filtrerParRegion(const QString& region);

    // 10. Liste des régions distinctes
    static QStringList listeRegions();
};

#endif // AGRICULTEUR_H
