#ifndef VENTE_H
#define VENTE_H
#include <QSqlQueryModel>
#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
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
private:
    int id_vente;
    int id_client;
    double  prix_unitaire;
    QDate   date_vente;
    int     quantite_vendue;
    double  montant_paye;
};

#endif // VENTE_H
