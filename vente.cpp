#include "vente.h"
#include <QDebug>

// ─── Constructeurs ────────────────────────────────────────────────────────────

Vente::Vente() {}

Vente::Vente(int idVente, int idClient, double prixUnitaire,
             QDate dateVente, int quantiteVendue, double montantPaye)
    : id_vente(idVente), id_client(idClient), prix_unitaire(prixUnitaire),
    date_vente(dateVente), quantite_vendue(quantiteVendue), montant_paye(montantPaye)
{}

// ─── Getters ──────────────────────────────────────────────────────────────────

int Vente::getIdVente()        const { return id_vente; }
int Vente::getIdClient()       const { return id_client; }
double  Vente::getPrixUnitaire()   const { return prix_unitaire; }
QDate   Vente::getDateVente()      const { return date_vente; }
int     Vente::getQuantiteVendue() const { return quantite_vendue; }
double  Vente::getMontantPaye()    const { return montant_paye; }

// ─── Setters ──────────────────────────────────────────────────────────────────

void Vente::setIdVente(const int& id)   { id_vente = id; }
void Vente::setIdClient(const int& id)  { id_client = id; }
void Vente::setPrixUnitaire(double prix)    { prix_unitaire = prix; }
void Vente::setDateVente(const QDate& date) { date_vente = date; }
void Vente::setQuantiteVendue(int quantite) { quantite_vendue = quantite; }
void Vente::setMontantPaye(double montant)  { montant_paye = montant; }

// ─── CRUD ─────────────────────────────────────────────────────────────────────

bool Vente::ajouter()
{
    QSqlQuery query;

    // ✅ Pas d'ID_VENTE — Oracle le génère automatiquement
    query.prepare("INSERT INTO SMART.VENTE "
                  "(DATE_VENTE, QUANTITE_VENDUE, PRIX_UNITAIRE, MONTANT_PAYE, ID_CLIENT) "
                  "VALUES (:date, :qte, :prix, :montant, :client)");

    query.bindValue(":date",    date_vente);
    query.bindValue(":qte",     quantite_vendue);
    query.bindValue(":prix",    prix_unitaire);
    query.bindValue(":montant", montant_paye);
    query.bindValue(":client",  id_client);

    if (!query.exec()) {
        qDebug() << "[AJOUTER] Erreur :" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur - Ajouter",
                              "Impossible d'ajouter la vente :\n"
                                  + query.lastError().text());
        return false;
    }
    return true;
}
bool Vente::modifier()
{
    QSqlQuery query(QSqlDatabase::database());

    QString sqlDirect =
        QString("UPDATE SMART.VENTE SET "
                "DATE_VENTE      = TO_DATE('%1','DD/MM/YYYY'), "
                "QUANTITE_VENDUE = %2, "
                "PRIX_UNITAIRE   = %3, "
                "MONTANT_PAYE    = %4, "
                "ID_CLIENT       = %5 "
                "WHERE ID_VENTE  = %6")
            .arg(date_vente.toString("dd/MM/yyyy"))
            .arg(quantite_vendue)
            .arg(prix_unitaire)
            .arg(montant_paye)
            .arg(id_client)
            .arg(id_vente);

    qDebug() << "[MODIFIER] SQL :" << sqlDirect;

    if (!query.exec(sqlDirect)) {
        qDebug() << "[MODIFIER] Erreur :" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur - Modifier",
                              "Impossible de modifier la vente :\n"
                                  + query.lastError().text());
        return false;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(nullptr, "Attention",
                             "Aucune vente trouvée avec l'ID : " + id_vente);
        return false;
    }
    return true;
}

bool Vente::afficher(const int& idVente)
{
    QSqlQuery query(QSqlDatabase::database());

    QString sqlDirect =
        QString("SELECT ID_VENTE, ID_CLIENT, PRIX_UNITAIRE, "
                "TO_CHAR(DATE_VENTE,'YYYY-MM-DD') AS DATE_VENTE, "
                "QUANTITE_VENDUE, MONTANT_PAYE "
                "FROM SMART.VENTE WHERE ID_VENTE = %1")
            .arg(idVente);

    qDebug() << "[AFFICHER] SQL :" << sqlDirect;

    if (!query.exec(sqlDirect)) {
        QMessageBox::critical(nullptr, "Erreur - Afficher",
                              "Erreur lors de la recherche :\n"
                                  + query.lastError().text());
        return false;
    }

    if (!query.next()) {
        QMessageBox::information(nullptr, "Résultat",
                                 "Aucune vente trouvée avec l'ID : " + idVente);
        return false;
    }

    id_vente        = query.value("ID_VENTE").toInt();
    id_client       = query.value("ID_CLIENT").toInt();
    prix_unitaire   = query.value("PRIX_UNITAIRE").toDouble();
    date_vente      = QDate::fromString(query.value("DATE_VENTE").toString(), "yyyy-MM-dd");
    quantite_vendue = query.value("QUANTITE_VENDUE").toInt();
    montant_paye    = query.value("MONTANT_PAYE").toDouble();

    return true;
}

bool Vente::annuler(const int& idVente)
{
    QSqlQuery query(QSqlDatabase::database());

    QString sqlDirect =
        QString("DELETE FROM SMART.VENTE WHERE ID_VENTE = %1")
            .arg(idVente);

    qDebug() << "[SUPPRIMER] SQL :" << sqlDirect;

    if (!query.exec(sqlDirect)) {
        QMessageBox::critical(nullptr, "Erreur - Supprimer",
                              "Impossible de supprimer la vente :\n"
                                  + query.lastError().text());
        return false;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(nullptr, "Attention",
                             "Aucune vente trouvée avec l'ID : " + idVente);
        return false;
    }
    return true;
}

QSqlQuery Vente::listerTous()
{
    QSqlQuery query(QSqlDatabase::database());

    query.exec("SELECT ID_VENTE, ID_CLIENT, PRIX_UNITAIRE, "
               "TO_CHAR(DATE_VENTE,'YYYY-MM-DD') AS DATE_VENTE, "
               "QUANTITE_VENDUE, MONTANT_PAYE "
               "FROM SMART.VENTE ORDER BY ID_VENTE");

    return query;
}



