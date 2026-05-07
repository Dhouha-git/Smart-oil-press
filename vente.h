#ifndef VENTE_H
#define VENTE_H
#include <QSqlQueryModel>
#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QMap>
#include <QVector>
#include <QPair>
#include "connection.h"

class Vente
{
public:
    Vente();
    Vente(int idVente, int idClient, double prixUnitaire,
          QDate dateVente, int quantiteVendue, double montantPaye);

    // Getters
    int getIdVente() const;
    int getIdClient() const;
    double getPrixUnitaire() const;
    QDate getDateVente() const;
    int getQuantiteVendue() const;
    double getMontantPaye() const;

    // Setters
    void setIdVente(const int& idVente);
    void setIdClient(const int& idClient);
    void setPrixUnitaire(double prix);
    void setDateVente(const QDate& date);
    void setQuantiteVendue(int quantite);
    void setMontantPaye(double montant);

    // Opérations CRUD
    bool ajouter();
    bool modifier();
    bool afficher(const int& idVente); // Charge une vente dans l'objet
    bool annuler(const int& idVente);  // Supprime une vente

    // Retourne toutes les ventes sous forme de QSqlQuery
    QSqlQuery listerTous();
    QSqlQueryModel* afficher();

    // ── Recherche & Tri ──────────────────────────────────────────────────────
    static QSqlQueryModel* rechercherParClient(int idClient);
    static QSqlQueryModel* rechercherParDate(const QDate& date);

    enum TriType { ParDate, ParMontant };
    static QSqlQueryModel* listerTries(TriType tri, bool ascendant = true);

    // ── Analyse du chiffre d'affaires ────────────────────────────────────────
    struct StatsCA {
        double total;
        double moyenne;
        double min;
        double max;
        int    nbVentes;
    };
    static StatsCA calculerChiffreAffaires(const QDate& debut, const QDate& fin);
    static QVector<QPair<QDate, double>> chiffreAffairesParJour(const QDate& debut, const QDate& fin);

    // ── Détection des anomalies (écart-type) ─────────────────────────────────
    static QVector<Vente> detecterAnomalies(const QDate& debut, const QDate& fin);

    // ── Prévision (régression linéaire simple) ───────────────────────────────
    static double prevoirVentesMoisProchain();

    // ── Exports ──────────────────────────────────────────────────────────────
    static bool exporterCSV(const QString& fichier);
    static bool exporterPDF(const QString& fichier, const QString& titre = "Liste des ventes");

private:
    int     id_vente;
    int     id_client;
    double  prix_unitaire;
    QDate   date_vente;
    int     quantite_vendue;
    double  montant_paye;
};

#endif // VENTE_H
