#include "agriculteur.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QDebug>
#include <QDate>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QFont>
#include <QColor>
#include <QPageSize>
#include <QPageLayout>
#include <QMarginsF>

// ============================================================
// CONSTRUCTEURS
// ============================================================
Agriculteur::Agriculteur() : cin(0), telephone(0), volume_livraison(0) {}

Agriculteur::Agriculteur(int cin, QString nom, QString prenom,
                         int telephone, QString email, QString region,
                         QString type_olive, QDate date_inscription,
                         double volume_livraison, QDate date_livraison)
{
    this->cin               = cin;
    this->nom               = nom;
    this->prenom            = prenom;
    this->telephone         = telephone;
    this->email             = email;
    this->region            = region;
    this->type_olive        = type_olive;
    this->date_inscription  = date_inscription;
    this->volume_livraison  = volume_livraison;
    this->date_livraison    = date_livraison;
}

// ============================================================
// CRUD DE BASE
// ============================================================
bool Agriculteur::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO AGRICULTEUR "
                  "(CIN, NOM, PRENOM, TELEPHONE, EMAIL, REGION, TYPE_OLIVE, "
                  "DATE_INSCRIPTION, VOLUME_LIVRAISON, DATE_LIVRAISON) "
                  "VALUES (:cin, :nom, :prenom, :telephone, :email, :region, "
                  ":type_olive, :date_inscription, :volume_livraison, :date_livraison)");

    query.bindValue(":cin",              cin);
    query.bindValue(":nom",              nom);
    query.bindValue(":prenom",           prenom);
    query.bindValue(":telephone",        telephone);
    query.bindValue(":email",            email);
    query.bindValue(":region",           region);
    query.bindValue(":type_olive",       type_olive);
    query.bindValue(":date_inscription", date_inscription);
    query.bindValue(":volume_livraison", volume_livraison);
    query.bindValue(":date_livraison",   date_livraison);

    if (!query.exec()) {
        qDebug() << "[AGRICULTEUR] Erreur ajout:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Agriculteur::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE AGRICULTEUR SET "
                  "NOM=:nom, PRENOM=:prenom, TELEPHONE=:telephone, "
                  "EMAIL=:email, REGION=:region, TYPE_OLIVE=:type_olive, "
                  "DATE_INSCRIPTION=:date_inscription, "
                  "VOLUME_LIVRAISON=:volume_livraison, "
                  "DATE_LIVRAISON=:date_livraison "
                  "WHERE CIN=:cin");

    query.bindValue(":cin",              cin);
    query.bindValue(":nom",              nom);
    query.bindValue(":prenom",           prenom);
    query.bindValue(":telephone",        telephone);
    query.bindValue(":email",            email);
    query.bindValue(":region",           region);
    query.bindValue(":type_olive",       type_olive);
    query.bindValue(":date_inscription", date_inscription);
    query.bindValue(":volume_livraison", volume_livraison);
    query.bindValue(":date_livraison",   date_livraison);

    if (!query.exec()) {
        qDebug() << "[AGRICULTEUR] Erreur modifier:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Agriculteur::supprimer(int cin)
{
    QSqlQuery query;
    query.prepare("DELETE FROM AGRICULTEUR WHERE CIN=:cin");
    query.bindValue(":cin", cin);

    if (!query.exec()) {
        qDebug() << "[AGRICULTEUR] Erreur suppression:" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel* Agriculteur::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT TO_CHAR(CIN) AS CIN, NOM, PRENOM, "
                  "TO_CHAR(TELEPHONE) AS TELEPHONE, EMAIL, REGION, "
                  "TYPE_OLIVE, TO_CHAR(DATE_INSCRIPTION, 'DD/MM/YYYY') AS DATE_INSCRIPTION, "
                  "VOLUME_LIVRAISON, "
                  "TO_CHAR(DATE_LIVRAISON, 'DD/MM/YYYY') AS DATE_LIVRAISON "
                  "FROM AGRICULTEUR ORDER BY CIN");
    query.exec();
    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, "CIN");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Téléphone");
    model->setHeaderData(4, Qt::Horizontal, "Email");
    model->setHeaderData(5, Qt::Horizontal, "Région");
    model->setHeaderData(6, Qt::Horizontal, "Type Olive");
    model->setHeaderData(7, Qt::Horizontal, "Date Inscription");
    model->setHeaderData(8, Qt::Horizontal, "Volume Livraison");
    model->setHeaderData(9, Qt::Horizontal, "Date Livraison");

    return model;
}

QSqlQueryModel* Agriculteur::rechercher(QString val)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT TO_CHAR(CIN) AS CIN, NOM, PRENOM, "
                  "TO_CHAR(TELEPHONE) AS TELEPHONE, EMAIL, REGION, "
                  "TYPE_OLIVE, TO_CHAR(DATE_INSCRIPTION, 'DD/MM/YYYY') AS DATE_INSCRIPTION, "
                  "VOLUME_LIVRAISON, "
                  "TO_CHAR(DATE_LIVRAISON, 'DD/MM/YYYY') AS DATE_LIVRAISON "
                  "FROM AGRICULTEUR "
                  "WHERE UPPER(NOM) LIKE UPPER(:val) "
                  "OR UPPER(PRENOM) LIKE UPPER(:val) "
                  "OR UPPER(REGION) LIKE UPPER(:val) "
                  "OR TO_CHAR(CIN) LIKE :val");
    query.bindValue(":val", "%" + val + "%");
    query.exec();
    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, "CIN");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Téléphone");
    model->setHeaderData(4, Qt::Horizontal, "Email");
    model->setHeaderData(5, Qt::Horizontal, "Région");
    model->setHeaderData(6, Qt::Horizontal, "Type Olive");
    model->setHeaderData(7, Qt::Horizontal, "Date Inscription");
    model->setHeaderData(8, Qt::Horizontal, "Volume Livraison");
    model->setHeaderData(9, Qt::Horizontal, "Date Livraison");

    return model;
}

QSqlQueryModel* Agriculteur::trier(QString critere)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT TO_CHAR(CIN) AS CIN, NOM, PRENOM, "
                    "TO_CHAR(TELEPHONE) AS TELEPHONE, EMAIL, REGION, "
                    "TYPE_OLIVE, TO_CHAR(DATE_INSCRIPTION, 'DD/MM/YYYY') AS DATE_INSCRIPTION, "
                    "VOLUME_LIVRAISON, "
                    "TO_CHAR(DATE_LIVRAISON, 'DD/MM/YYYY') AS DATE_LIVRAISON "
                    "FROM AGRICULTEUR ORDER BY " + critere);
    model->setHeaderData(0, Qt::Horizontal, "CIN");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Téléphone");
    model->setHeaderData(4, Qt::Horizontal, "Email");
    model->setHeaderData(5, Qt::Horizontal, "Région");
    model->setHeaderData(6, Qt::Horizontal, "Type Olive");
    model->setHeaderData(7, Qt::Horizontal, "Date Inscription");
    model->setHeaderData(8, Qt::Horizontal, "Volume Livraison");
    model->setHeaderData(9, Qt::Horizontal, "Date Livraison");
    return model;
}
// ============================================================
// METIER AVANCEE 1 — Volume par Région
// ============================================================
QMap<QString, double> Agriculteur::volumeParRegion()
{
    QMap<QString, double> result;
    QSqlQuery query;
    query.exec("SELECT REGION, SUM(VOLUME_LIVRAISON) "
               "FROM AGRICULTEUR "
               "GROUP BY REGION "
               "ORDER BY SUM(VOLUME_LIVRAISON) DESC");

    while (query.next()) {
        result[query.value(0).toString()] = query.value(1).toDouble();
    }
    return result;
}

// ============================================================
// METIER AVANCEE 2 — Count par Type Olive
// ============================================================
QMap<QString, int> Agriculteur::countParTypeOlive()
{
    QMap<QString, int> result;
    QSqlQuery query;
    query.exec("SELECT TYPE_OLIVE, COUNT(*) "
               "FROM AGRICULTEUR "
               "GROUP BY TYPE_OLIVE "
               "ORDER BY COUNT(*) DESC");

    while (query.next()) {
        result[query.value(0).toString()] = query.value(1).toInt();
    }
    return result;
}

// ============================================================
// METIER AVANCEE 3 — Top N Agriculteurs
// ============================================================
QList<QMap<QString,QString>> Agriculteur::topAgriculteurs(int n)
{
    QList<QMap<QString,QString>> result;
    QSqlQuery query;
    query.prepare("SELECT CIN, NOM, PRENOM, REGION, TYPE_OLIVE, VOLUME_LIVRAISON "
                  "FROM AGRICULTEUR "
                  "ORDER BY VOLUME_LIVRAISON DESC "
                  "FETCH FIRST :n ROWS ONLY");
    query.bindValue(":n", n);
    query.exec();

    while (query.next()) {
        QMap<QString,QString> row;
        row["cin"]    = query.value(0).toString();
        row["nom"]    = query.value(1).toString();
        row["prenom"] = query.value(2).toString();
        row["region"] = query.value(3).toString();
        row["type"]   = query.value(4).toString();
        row["volume"] = QString::number(query.value(5).toDouble(), 'f', 2);
        result.append(row);
    }
    return result;
}

// ============================================================
// METIER AVANCEE 4 — Alertes Volume Faible
// ============================================================
QSqlQueryModel* Agriculteur::alertesVolumefaible(double seuil)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT CIN, NOM, PRENOM, REGION, TYPE_OLIVE, "
                  "VOLUME_LIVRAISON, DATE_LIVRAISON "
                  "FROM AGRICULTEUR "
                  "WHERE VOLUME_LIVRAISON < :seuil "
                  "ORDER BY VOLUME_LIVRAISON ASC");
    query.bindValue(":seuil", seuil);
    query.exec();
    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, "CIN");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Région");
    model->setHeaderData(4, Qt::Horizontal, "Type Olive");
    model->setHeaderData(5, Qt::Horizontal, "Volume (L)");
    model->setHeaderData(6, Qt::Horizontal, "Date Livraison");

    return model;
}

// ============================================================
// METIER AVANCEE 5 — Alertes Date Dépassée
// ============================================================
QSqlQueryModel* Agriculteur::alertesDateDepassee()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.exec("SELECT CIN, NOM, PRENOM, REGION, "
               "DATE_LIVRAISON, "
               "TRUNC(SYSDATE) - TRUNC(DATE_LIVRAISON) AS JOURS_RETARD "
               "FROM AGRICULTEUR "
               "WHERE DATE_LIVRAISON < SYSDATE "
               "ORDER BY DATE_LIVRAISON ASC");
    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, "CIN");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Région");
    model->setHeaderData(4, Qt::Horizontal, "Date Livraison");
    model->setHeaderData(5, Qt::Horizontal, "Jours de Retard");

    return model;
}

// ============================================================
// METIER AVANCEE 6 — Alertes Agriculteurs Inactifs
// ============================================================
QSqlQueryModel* Agriculteur::alertesInactifs(int joursInactivite)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT CIN, NOM, PRENOM, REGION, EMAIL, TELEPHONE, "
                  "DATE_INSCRIPTION, "
                  "TRUNC(SYSDATE) - TRUNC(DATE_INSCRIPTION) AS JOURS_DEPUIS_INSCRIPTION "
                  "FROM AGRICULTEUR "
                  "WHERE TRUNC(SYSDATE) - TRUNC(DATE_INSCRIPTION) > :jours "
                  "AND VOLUME_LIVRAISON = 0 "
                  "ORDER BY DATE_INSCRIPTION ASC");
    query.bindValue(":jours", joursInactivite);
    query.exec();
    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, "CIN");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Région");
    model->setHeaderData(4, Qt::Horizontal, "Email");
    model->setHeaderData(5, Qt::Horizontal, "Téléphone");
    model->setHeaderData(6, Qt::Horizontal, "Date Inscription");
    model->setHeaderData(7, Qt::Horizontal, "Jours Inactif");

    return model;
}

// ============================================================
// METIER AVANCEE 7 — Statistiques Globales
// ============================================================
QMap<QString, double> Agriculteur::statsGlobales()
{
    QMap<QString, double> stats;
    QSqlQuery query;

    if (query.exec("SELECT COUNT(*), SUM(VOLUME_LIVRAISON), AVG(VOLUME_LIVRAISON), "
                   "COUNT(DISTINCT REGION), MAX(VOLUME_LIVRAISON), MIN(VOLUME_LIVRAISON) "
                   "FROM AGRICULTEUR")) {
        if (query.next()) {
            stats["total"]          = query.value(0).toDouble();
            stats["volume_total"]   = query.value(1).toDouble();
            stats["volume_moyen"]   = query.value(2).toDouble();
            stats["nb_regions"]     = query.value(3).toDouble();
            stats["volume_max"]     = query.value(4).toDouble();
            stats["volume_min"]     = query.value(5).toDouble();
        }
    }

    // Compter les alertes
    QSqlQuery qAlerte;
    if (qAlerte.exec("SELECT COUNT(*) FROM AGRICULTEUR WHERE VOLUME_LIVRAISON < 100")) {
        if (qAlerte.next())
            stats["alertes_volume"] = qAlerte.value(0).toDouble();
    }

    QSqlQuery qRetard;
    if (qRetard.exec("SELECT COUNT(*) FROM AGRICULTEUR WHERE DATE_LIVRAISON < SYSDATE")) {
        if (qRetard.next())
            stats["alertes_retard"] = qRetard.value(0).toDouble();
    }

    return stats;
}

// ============================================================
// METIER AVANCEE 8 — Export PDF Complet
// ============================================================
bool Agriculteur::exporterPDF(const QString& cheminFichier)
{
    QPrinter printer(QPrinter::ScreenResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(cheminFichier);
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setPageSize(QPageSize::A4);
    printer.setPageMargins(QMarginsF(10, 10, 10, 10), QPageLayout::Millimeter);

    QPainter painter;
    if (!painter.begin(&printer)) return false;

    QRect page   = painter.viewport();
    int W        = page.width();
    int H        = page.height();
    int margeG   = 20;
    int margeD   = 20;
    int totalW   = W - margeG - margeD;

    // ── Couleurs ──────────────────────────────────
    QColor vertFonce(85, 107, 47);
    QColor orCouleur(212, 175, 55);
    QColor blanc(255, 255, 255);
    QColor grisClair(240, 244, 235);
    QColor texteSombre(40, 40, 40);
    QColor bordureGrise(180, 180, 180);
    QColor rouge(200, 50, 50);

    // ── EN-TÊTE ───────────────────────────────────
    int hauteurHeader = 80;
    painter.fillRect(0, 0, W, hauteurHeader, vertFonce);

    painter.setPen(blanc);
    painter.setFont(QFont("Arial", 20, QFont::Bold));
    painter.drawText(QRect(0, 8, W, 35), Qt::AlignCenter, "Smart Oil Press");

    painter.setFont(QFont("Arial", 11));
    painter.drawText(QRect(0, 40, W, 25), Qt::AlignCenter, "Rapport des Agriculteurs");

    painter.setFont(QFont("Arial", 8));
    painter.drawText(QRect(margeG, 62, totalW, 18),
                     Qt::AlignRight,
                     "Généré le : " + QDateTime::currentDateTime().toString("dd/MM/yyyy à HH:mm"));

    painter.setPen(QPen(orCouleur, 3));
    painter.drawLine(0, hauteurHeader, W, hauteurHeader);

    int y = hauteurHeader + 15;

    // ── SECTION STATISTIQUES ─────────────────────
    QMap<QString, double> stats = Agriculteur::statsGlobales();

    painter.setFont(QFont("Arial", 11, QFont::Bold));
    painter.setPen(vertFonce);
    painter.drawText(margeG, y + 15, "Statistiques Générales");
    y += 25;

    painter.setPen(QPen(orCouleur, 2));
    painter.drawLine(margeG, y, margeG + totalW, y);
    y += 10;

    // KPI Cards
    QStringList kpiLabels = {"Total Agriculteurs", "Volume Total (L)", "Volume Moyen (L)",
                             "Nb Régions", "Alertes Volume Faible", "Alertes Retard"};
    QStringList kpiKeys   = {"total", "volume_total", "volume_moyen",
                           "nb_regions", "alertes_volume", "alertes_retard"};
    QList<QColor> kpiColors = {vertFonce, QColor("#2874A6"), QColor("#1E8449"),
                               QColor("#D35400"), rouge, rouge};

    int kpiW     = totalW / 6;
    int kpiH     = 55;
    int kpiX     = margeG;

    for (int i = 0; i < kpiLabels.size(); i++) {
        QColor bg = kpiColors[i].lighter(190);
        painter.fillRect(kpiX, y, kpiW - 4, kpiH, bg);
        painter.setPen(QPen(kpiColors[i], 2));
        painter.drawRect(kpiX, y, kpiW - 4, kpiH);

        painter.setFont(QFont("Arial", 7));
        painter.setPen(QColor("#555555"));
        painter.drawText(QRect(kpiX + 4, y + 4, kpiW - 8, 16),
                         Qt::AlignCenter | Qt::TextWordWrap, kpiLabels[i]);

        double val = stats.value(kpiKeys[i], 0.0);
        QString valStr = (kpiKeys[i].contains("volume"))
                             ? QString::number(val, 'f', 1)
                             : QString::number((int)val);

        painter.setFont(QFont("Arial", 12, QFont::Bold));
        painter.setPen(kpiColors[i]);
        painter.drawText(QRect(kpiX + 4, y + 22, kpiW - 8, 25),
                         Qt::AlignCenter, valStr);

        kpiX += kpiW;
    }
    y += kpiH + 20;

    // ── TABLEAU PRINCIPAL ─────────────────────────
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.setPen(vertFonce);
    painter.drawText(margeG, y + 12, "Liste des Agriculteurs");
    y += 22;

    painter.setPen(QPen(orCouleur, 2));
    painter.drawLine(margeG, y, margeG + totalW, y);
    y += 8;

    // Colonnes
    QStringList headers = {"CIN", "Nom", "Prénom", "Région", "Type Olive",
                           "Volume (L)", "Date Inscription", "Date Livraison"};
    QList<double> ratios = {0.10, 0.12, 0.12, 0.14, 0.12, 0.12, 0.14, 0.14};
    QList<int> colW;
    for (double r : ratios) colW << int(totalW * r);

    int rowH = 24;

    // En-tête tableau
    painter.fillRect(margeG, y, totalW, rowH, vertFonce);
    painter.setFont(QFont("Arial", 8, QFont::Bold));
    int xCol = margeG;
    for (int i = 0; i < headers.size(); i++) {
        if (i > 0) {
            painter.setPen(QPen(QColor(255,255,255,80), 1));
            painter.drawLine(xCol, y, xCol, y + rowH);
        }
        painter.setPen(blanc);
        painter.drawText(QRect(xCol + 3, y, colW[i] - 6, rowH),
                         Qt::AlignVCenter | Qt::AlignHCenter, headers[i]);
        xCol += colW[i];
    }
    y += rowH;

    // Données
    QSqlQuery query;
    query.exec("SELECT CIN, NOM, PRENOM, REGION, TYPE_OLIVE, "
               "VOLUME_LIVRAISON, DATE_INSCRIPTION, DATE_LIVRAISON "
               "FROM AGRICULTEUR ORDER BY VOLUME_LIVRAISON DESC");

    int nbLigne      = 0;
    int lignesParPage = (H - hauteurHeader - 15 - kpiH - 120) / rowH - 1;

    while (query.next()) {
        // Saut de page si nécessaire
        if (nbLigne > 0 && nbLigne % lignesParPage == 0) {
            printer.newPage();
            painter.fillRect(0, 0, W, 35, vertFonce);
            painter.setPen(blanc);
            painter.setFont(QFont("Arial", 10, QFont::Bold));
            painter.drawText(QRect(0, 0, W, 35), Qt::AlignCenter,
                             "Smart Oil Press — Rapport Agriculteurs (suite)");
            painter.setPen(QPen(orCouleur, 2));
            painter.drawLine(0, 35, W, 35);
            y = 45;

            // Ré-afficher en-tête tableau
            painter.fillRect(margeG, y, totalW, rowH, vertFonce);
            painter.setFont(QFont("Arial", 8, QFont::Bold));
            xCol = margeG;
            for (int i = 0; i < headers.size(); i++) {
                if (i > 0) {
                    painter.setPen(QPen(QColor(255,255,255,80), 1));
                    painter.drawLine(xCol, y, xCol, y + rowH);
                }
                painter.setPen(blanc);
                painter.drawText(QRect(xCol + 3, y, colW[i] - 6, rowH),
                                 Qt::AlignVCenter | Qt::AlignHCenter, headers[i]);
                xCol += colW[i];
            }
            y += rowH;
        }

        // Fond alterné
        QColor bg = (nbLigne % 2 == 0) ? blanc : grisClair;
        painter.fillRect(margeG, y, totalW, rowH, bg);

        // Vérifier alertes pour colorer la ligne
        double volume        = query.value(5).toDouble();
        QDate  dateLivraison = query.value(7).toDate();
        bool   alerteVol     = (volume < 100.0);
        bool   alerteDate    = (dateLivraison < QDate::currentDate());

        QStringList vals;
        vals << query.value(0).toString()
             << query.value(1).toString()
             << query.value(2).toString()
             << query.value(3).toString()
             << query.value(4).toString()
             << QString::number(volume, 'f', 1)
             << query.value(6).toDate().toString("dd/MM/yyyy")
             << dateLivraison.toString("dd/MM/yyyy");

        xCol = margeG;
        for (int i = 0; i < vals.size(); i++) {
            painter.setPen(QPen(bordureGrise, 1));
            if (i > 0) painter.drawLine(xCol, y, xCol, y + rowH);

            // Couleur spéciale pour alertes
            if (i == 5 && alerteVol) {
                painter.setPen(rouge);
                painter.setFont(QFont("Arial", 8, QFont::Bold));
            } else if (i == 7 && alerteDate) {
                painter.setPen(rouge);
                painter.setFont(QFont("Arial", 8, QFont::Bold));
            } else {
                painter.setPen(texteSombre);
                painter.setFont(QFont("Arial", 8));
            }

            painter.drawText(QRect(xCol + 3, y, colW[i] - 6, rowH),
                             Qt::AlignVCenter | Qt::AlignHCenter, vals[i]);
            xCol += colW[i];
        }

        // Bordure bas
        painter.setPen(QPen(bordureGrise, 1));
        painter.drawLine(margeG, y + rowH, margeG + totalW, y + rowH);

        y += rowH;
        nbLigne++;
    }

    // Bordure extérieure
    painter.setPen(QPen(vertFonce, 2));
    painter.drawRect(margeG, hauteurHeader + 15 + kpiH + 60,
                     totalW, y - (hauteurHeader + 15 + kpiH + 60));

    // ── PIED DE PAGE ─────────────────────────────
    painter.fillRect(0, H - 35, W, 35, grisClair);
    painter.setPen(QPen(orCouleur, 2));
    painter.drawLine(0, H - 35, W, H - 35);

    painter.setPen(texteSombre);
    painter.setFont(QFont("Arial", 8));
    painter.drawText(
        QRect(margeG, H - 30, totalW, 25),
        Qt::AlignCenter,
        QString("Smart Oil Press  |  Total : %1 agriculteur(s)  |  %2")
            .arg(nbLigne)
            .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy"))
        );

    painter.end();
    return true;
}

// ============================================================
// METIER AVANCEE 9 — Filtrer par Région
// ============================================================
QSqlQueryModel* Agriculteur::filtrerParRegion(const QString& region)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT CIN, NOM, PRENOM, TELEPHONE, EMAIL, REGION, "
                  "TYPE_OLIVE, DATE_INSCRIPTION, VOLUME_LIVRAISON, DATE_LIVRAISON "
                  "FROM AGRICULTEUR "
                  "WHERE UPPER(REGION) = UPPER(:region) "
                  "ORDER BY VOLUME_LIVRAISON DESC");
    query.bindValue(":region", region);
    query.exec();
    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, "CIN");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Téléphone");
    model->setHeaderData(4, Qt::Horizontal, "Email");
    model->setHeaderData(5, Qt::Horizontal, "Région");
    model->setHeaderData(6, Qt::Horizontal, "Type Olive");
    model->setHeaderData(7, Qt::Horizontal, "Date Inscription");
    model->setHeaderData(8, Qt::Horizontal, "Volume Livraison");
    model->setHeaderData(9, Qt::Horizontal, "Date Livraison");

    return model;
}

// ============================================================
// METIER AVANCEE 10 — Liste des Régions
// ============================================================
QStringList Agriculteur::listeRegions()
{
    QStringList regions;
    QSqlQuery query;
    query.exec("SELECT DISTINCT REGION FROM AGRICULTEUR ORDER BY REGION");
    while (query.next()) {
        regions << query.value(0).toString();
    }
    return regions;
}
