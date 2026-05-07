#include "vente.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QPrinter>
#include <QTextDocument>
#include <QPageSize>
#include <cmath>

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
    QSqlQuery query(QSqlDatabase::database());

    // Générer un ID unique via MAX+1 (fallback si pas de trigger Oracle)
    QSqlQuery idQuery(QSqlDatabase::database());
    idQuery.exec("SELECT NVL(MAX(ID_VENTE), 0) + 1 FROM SMART.VENTE");
    int newId = 1;
    if (idQuery.next())
        newId = idQuery.value(0).toInt();

    query.prepare("INSERT INTO SMART.VENTE "
                  "(ID_VENTE, DATE_VENTE, QUANTITE_VENDUE, PRIX_UNITAIRE, MONTANT_PAYE, ID_CLIENT) "
                  "VALUES (:id, :date, :qte, :prix, :montant, :client)");

    query.bindValue(":id",      newId);
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
                             "Aucune vente trouvée avec l'ID : " + QString::number(id_vente));
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
                                 "Aucune vente trouvée avec l'ID : " + QString::number(idVente));
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
                             "Aucune vente trouvée avec l'ID : " + QString::number(idVente));
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

// ─── afficher() → QSqlQueryModel* (pour tableView) ───────────────────────────

QSqlQueryModel* Vente::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT ID_VENTE, ID_CLIENT, PRIX_UNITAIRE, "
                  "TO_CHAR(DATE_VENTE,'DD/MM/YYYY') AS DATE_VENTE, "
                  "QUANTITE_VENDUE, MONTANT_PAYE "
                  "FROM SMART.VENTE ORDER BY ID_VENTE");
    query.exec();
    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, "ID Vente");
    model->setHeaderData(1, Qt::Horizontal, "ID Client");
    model->setHeaderData(2, Qt::Horizontal, "Prix Unitaire");
    model->setHeaderData(3, Qt::Horizontal, "Date Vente");
    model->setHeaderData(4, Qt::Horizontal, "Quantité");
    model->setHeaderData(5, Qt::Horizontal, "Montant Payé");
    return model;
}

// ─────────────────────────────────────────────────────────────────────────────
// 🔍 RECHERCHE PAR CLIENT
// ─────────────────────────────────────────────────────────────────────────────

QSqlQueryModel* Vente::rechercherParClient(int idClient)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT ID_VENTE, ID_CLIENT, PRIX_UNITAIRE, "
                  "TO_CHAR(DATE_VENTE,'DD/MM/YYYY') AS DATE_VENTE, "
                  "QUANTITE_VENDUE, MONTANT_PAYE "
                  "FROM SMART.VENTE WHERE ID_CLIENT = :id ORDER BY DATE_VENTE DESC");
    query.bindValue(":id", idClient);
    query.exec();
    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, "ID Vente");
    model->setHeaderData(1, Qt::Horizontal, "ID Client");
    model->setHeaderData(2, Qt::Horizontal, "Prix Unitaire");
    model->setHeaderData(3, Qt::Horizontal, "Date Vente");
    model->setHeaderData(4, Qt::Horizontal, "Quantité");
    model->setHeaderData(5, Qt::Horizontal, "Montant Payé");
    return model;
}

// ─────────────────────────────────────────────────────────────────────────────
// 🔍 RECHERCHE PAR DATE
// ─────────────────────────────────────────────────────────────────────────────

QSqlQueryModel* Vente::rechercherParDate(const QDate& date)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT ID_VENTE, ID_CLIENT, PRIX_UNITAIRE, "
                  "TO_CHAR(DATE_VENTE,'DD/MM/YYYY') AS DATE_VENTE, "
                  "QUANTITE_VENDUE, MONTANT_PAYE "
                  "FROM SMART.VENTE WHERE TRUNC(DATE_VENTE) = TO_DATE(:d,'YYYY-MM-DD') "
                  "ORDER BY ID_VENTE");
    query.bindValue(":d", date.toString("yyyy-MM-dd"));
    query.exec();
    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, "ID Vente");
    model->setHeaderData(1, Qt::Horizontal, "ID Client");
    model->setHeaderData(2, Qt::Horizontal, "Prix Unitaire");
    model->setHeaderData(3, Qt::Horizontal, "Date Vente");
    model->setHeaderData(4, Qt::Horizontal, "Quantité");
    model->setHeaderData(5, Qt::Horizontal, "Montant Payé");
    return model;
}

// ─────────────────────────────────────────────────────────────────────────────
// 🔃 TRI
// ─────────────────────────────────────────────────────────────────────────────

QSqlQueryModel* Vente::listerTries(TriType tri, bool ascendant)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query(QSqlDatabase::database());
    QString ordre = ascendant ? "ASC" : "DESC";
    QString champ = (tri == ParDate) ? "DATE_VENTE" : "MONTANT_PAYE";
    QString sql = QString("SELECT ID_VENTE, ID_CLIENT, PRIX_UNITAIRE, "
                          "TO_CHAR(DATE_VENTE,'DD/MM/YYYY') AS DATE_VENTE, "
                          "QUANTITE_VENDUE, MONTANT_PAYE "
                          "FROM SMART.VENTE ORDER BY %1 %2").arg(champ, ordre);
    query.exec(sql);
    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, "ID Vente");
    model->setHeaderData(1, Qt::Horizontal, "ID Client");
    model->setHeaderData(2, Qt::Horizontal, "Prix Unitaire");
    model->setHeaderData(3, Qt::Horizontal, "Date Vente");
    model->setHeaderData(4, Qt::Horizontal, "Quantité");
    model->setHeaderData(5, Qt::Horizontal, "Montant Payé");
    return model;
}

// ─────────────────────────────────────────────────────────────────────────────
// 📊 ANALYSE DU CHIFFRE D'AFFAIRES
// ─────────────────────────────────────────────────────────────────────────────

Vente::StatsCA Vente::calculerChiffreAffaires(const QDate& debut, const QDate& fin)
{
    StatsCA stats = {0, 0, 0, 0, 0};
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT SUM(MONTANT_PAYE) AS TOTAL, AVG(MONTANT_PAYE) AS MOYENNE, "
                  "MIN(MONTANT_PAYE) AS MIN_V, MAX(MONTANT_PAYE) AS MAX_V, COUNT(*) AS NB "
                  "FROM SMART.VENTE WHERE DATE_VENTE BETWEEN TO_DATE(:d,'YYYY-MM-DD') "
                  "AND TO_DATE(:f,'YYYY-MM-DD')");
    query.bindValue(":d", debut.toString("yyyy-MM-dd"));
    query.bindValue(":f", fin.toString("yyyy-MM-dd"));
    if (query.exec() && query.next()) {
        stats.total    = query.value("TOTAL").toDouble();
        stats.moyenne  = query.value("MOYENNE").toDouble();
        stats.min      = query.value("MIN_V").toDouble();
        stats.max      = query.value("MAX_V").toDouble();
        stats.nbVentes = query.value("NB").toInt();
    }
    return stats;
}

QVector<QPair<QDate, double>> Vente::chiffreAffairesParJour(const QDate& debut, const QDate& fin)
{
    QVector<QPair<QDate, double>> result;
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT TO_CHAR(TRUNC(DATE_VENTE),'YYYY-MM-DD') AS JOUR, "
                  "SUM(MONTANT_PAYE) AS CA "
                  "FROM SMART.VENTE WHERE DATE_VENTE BETWEEN TO_DATE(:d,'YYYY-MM-DD') "
                  "AND TO_DATE(:f,'YYYY-MM-DD') GROUP BY TRUNC(DATE_VENTE) ORDER BY TRUNC(DATE_VENTE)");
    query.bindValue(":d", debut.toString("yyyy-MM-dd"));
    query.bindValue(":f", fin.toString("yyyy-MM-dd"));
    if (query.exec()) {
        while (query.next()) {
            // TO_CHAR garantit un string "YYYY-MM-DD" lisible par fromString
            QDate d = QDate::fromString(query.value("JOUR").toString(), "yyyy-MM-dd");
            double ca = query.value("CA").toDouble();
            if (d.isValid())
                result.append(qMakePair(d, ca));
        }
    }
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// ⚠️ DÉTECTION DES VENTES ANORMALES (écart-type)
// ─────────────────────────────────────────────────────────────────────────────

QVector<Vente> Vente::detecterAnomalies(const QDate& debut, const QDate& fin)
{
    QVector<Vente> anomalies;
    StatsCA stats = calculerChiffreAffaires(debut, fin);
    if (stats.nbVentes < 2) return anomalies;

    QSqlQuery varQuery(QSqlDatabase::database());
    varQuery.prepare("SELECT STDDEV(MONTANT_PAYE) AS ECART_TYPE FROM SMART.VENTE "
                     "WHERE DATE_VENTE BETWEEN TO_DATE(:d,'YYYY-MM-DD') AND TO_DATE(:f,'YYYY-MM-DD')");
    varQuery.bindValue(":d", debut.toString("yyyy-MM-dd"));
    varQuery.bindValue(":f", fin.toString("yyyy-MM-dd"));
    varQuery.exec();
    varQuery.next();
    double ecartType = varQuery.value("ECART_TYPE").toDouble();
    double seuilBas  = stats.moyenne - 2 * ecartType;
    double seuilHaut = stats.moyenne + 2 * ecartType;

    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT ID_VENTE, ID_CLIENT, PRIX_UNITAIRE, DATE_VENTE, QUANTITE_VENDUE, MONTANT_PAYE "
                  "FROM SMART.VENTE WHERE DATE_VENTE BETWEEN TO_DATE(:d,'YYYY-MM-DD') "
                  "AND TO_DATE(:f,'YYYY-MM-DD') AND (MONTANT_PAYE < :bas OR MONTANT_PAYE > :haut)");
    query.bindValue(":d",    debut.toString("yyyy-MM-dd"));
    query.bindValue(":f",    fin.toString("yyyy-MM-dd"));
    query.bindValue(":bas",  seuilBas);
    query.bindValue(":haut", seuilHaut);
    if (query.exec()) {
        while (query.next()) {
            Vente v;
            v.id_vente        = query.value("ID_VENTE").toInt();
            v.id_client       = query.value("ID_CLIENT").toInt();
            v.prix_unitaire   = query.value("PRIX_UNITAIRE").toDouble();
            v.date_vente      = query.value("DATE_VENTE").toDate();
            v.quantite_vendue = query.value("QUANTITE_VENDUE").toInt();
            v.montant_paye    = query.value("MONTANT_PAYE").toDouble();
            anomalies.append(v);
        }
    }
    return anomalies;
}

// ─────────────────────────────────────────────────────────────────────────────
// 📈 PRÉVISION (régression linéaire simple sur les 12 derniers mois)
// ─────────────────────────────────────────────────────────────────────────────

double Vente::prevoirVentesMoisProchain()
{
    QDate aujourd_hui = QDate::currentDate();
    QDate debut = aujourd_hui.addMonths(-12);
    QVector<QPair<QDate, double>> caJournalier = chiffreAffairesParJour(debut, aujourd_hui);

    if (caJournalier.size() < 2) return 0.0;

    int n = caJournalier.size();
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    for (int i = 0; i < n; ++i) {
        double x = i + 1;
        double y = caJournalier[i].second;
        sumX  += x;
        sumY  += y;
        sumXY += x * y;
        sumX2 += x * x;
    }
    double a = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    double b = (sumY - a * sumX) / n;

    double prevision = a * (n + 30) + b;
    return prevision > 0 ? prevision : 0.0;
}

// ─────────────────────────────────────────────────────────────────────────────
// 📄 EXPORT CSV
// ─────────────────────────────────────────────────────────────────────────────

bool Vente::exporterCSV(const QString& fichier)
{
    QFile file(fichier);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    out << "ID_VENTE;ID_CLIENT;PRIX_UNITAIRE;DATE_VENTE;QUANTITE_VENDUE;MONTANT_PAYE\n";

    QSqlQuery query(QSqlDatabase::database());
    query.exec("SELECT ID_VENTE, ID_CLIENT, PRIX_UNITAIRE, "
               "TO_CHAR(DATE_VENTE,'DD/MM/YYYY') AS DATE_VENTE, "
               "QUANTITE_VENDUE, MONTANT_PAYE FROM SMART.VENTE ORDER BY ID_VENTE");
    while (query.next()) {
        out << query.value("ID_VENTE").toString()       << ";"
            << query.value("ID_CLIENT").toString()      << ";"
            << query.value("PRIX_UNITAIRE").toDouble()  << ";"
            << query.value("DATE_VENTE").toString()     << ";"
            << query.value("QUANTITE_VENDUE").toInt()   << ";"
            << query.value("MONTANT_PAYE").toDouble()   << "\n";
    }
    file.close();
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// 📄 EXPORT PDF
// ─────────────────────────────────────────────────────────────────────────────

bool Vente::exporterPDF(const QString& fichier, const QString& titre)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fichier);
    printer.setPageSize(QPageSize::A4);

    QTextDocument doc;
    QString html = "<h1 style='color:#556B2F;'>" + titre + "</h1>";
    html += "<table border='1' cellspacing='0' cellpadding='4' "
            "style='border-collapse:collapse; width:100%;'>";
    html += "<tr style='background-color:#556B2F; color:white;'>"
            "<th>ID Vente</th><th>ID Client</th><th>Prix Unit.</th>"
            "<th>Date</th><th>Qté</th><th>Montant</th></tr>";

    QSqlQuery query(QSqlDatabase::database());
    query.exec("SELECT ID_VENTE, ID_CLIENT, PRIX_UNITAIRE, "
               "TO_CHAR(DATE_VENTE,'DD/MM/YYYY') AS DATE_VENTE, "
               "QUANTITE_VENDUE, MONTANT_PAYE FROM SMART.VENTE ORDER BY ID_VENTE");
    int row = 0;
    while (query.next()) {
        QString bg = (row++ % 2 == 0) ? "#ffffff" : "#f5f5e8";
        html += "<tr style='background-color:" + bg + ";'>";
        html += "<td>" + query.value("ID_VENTE").toString()                              + "</td>";
        html += "<td>" + query.value("ID_CLIENT").toString()                             + "</td>";
        html += "<td>" + QString::number(query.value("PRIX_UNITAIRE").toDouble(),'f',2)  + "</td>";
        html += "<td>" + query.value("DATE_VENTE").toString()                            + "</td>";
        html += "<td>" + QString::number(query.value("QUANTITE_VENDUE").toInt())         + "</td>";
        html += "<td>" + QString::number(query.value("MONTANT_PAYE").toDouble(),'f',2)   + "</td>";
        html += "</tr>";
    }
    html += "</table>";
    doc.setHtml(html);
    doc.print(&printer);
    return true;
}



