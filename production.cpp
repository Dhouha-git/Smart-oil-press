#include "production.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QDebug>
#include <QComboBox>
#include <QDateTime>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlDatabase>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QSerialPortInfo>

// ============================================================
// CONSTRUCTEUR PAR DÉFAUT
// ============================================================
Production::Production()
    : id_operation(0), id_agri(0), id_emp(0),
    quantite_olives(0), quantite_huile(0),
    rendement(0), duree_pressage(0)
// ✅ SUPPRIMÉ : temperature_moyenne (n'existe pas dans la BD)
{}

// ============================================================
// CONSTRUCTEUR PARAMÉTRÉ
// ✅ CORRIGÉ : duree_pressage est int (plus de .toInt() risqué)
// ============================================================
Production::Production(QString date, double quantite_olives, double quantite_huile,
                       double rendement, QString type_huile,
                       int id_agri, int id_emp,
                       QString observation, int duree_pressage)
{
    this->id_operation    = 0;
    this->date_production = QDate::fromString(date, "dd/MM/yyyy");
    if (!this->date_production.isValid())
        this->date_production = QDate::fromString(date, "yyyy-MM-dd");
    this->quantite_olives = quantite_olives;
    this->quantite_huile  = quantite_huile;
    this->rendement       = rendement;
    this->type_huile      = type_huile;
    this->id_agri         = id_agri;
    this->id_emp          = id_emp;
    this->observation     = observation;
    this->duree_pressage  = duree_pressage;
}

// ============================================================
// AJOUTER
// ✅ OK : utilise bien CIN (= id_agri) — conforme à la BD
// ============================================================
bool Production::ajouter()
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO PRODUCTION "
        "(ID_OPERATION, DATE_PRODUCTION, QUANTITE_OLIVES, QUANTITE_HUILE, "
        "TYPE_HUILE, RENDEMENT, ID_EMP, OBSERVATION, DUREE_PRESSAGE, CIN) "
        "VALUES (:id, :date, :qto, :qth, :type, :rend, :id_emp, :obs, :duree, :cin)"
        );

    query.bindValue(":id",     id_operation);
    query.bindValue(":date",   date_production);
    query.bindValue(":qto",    quantite_olives);
    query.bindValue(":qth",    quantite_huile);
    query.bindValue(":type",   type_huile);
    query.bindValue(":rend",   rendement);
    query.bindValue(":id_emp", id_emp);
    query.bindValue(":obs",    observation);
    query.bindValue(":duree",  duree_pressage);
    query.bindValue(":cin",    id_agri);  // CIN = clé de l'agriculteur

    if (!query.exec()) {
        lastError = query.lastError().text();
        qDebug() << "[PRODUCTION] Erreur ajouter :" << lastError;
        return false;
    }
    return true;
}

// ============================================================
// MODIFIER
// ✅ CORRIGÉ : ID_AGRI → CIN  (colonne réelle dans la BD)
// ============================================================
bool Production::modifier()
{
    QSqlQuery query;
    query.prepare(
        "UPDATE PRODUCTION SET "
        "DATE_PRODUCTION=:date, CIN=:cin, ID_EMP=:id_emp, "
        "QUANTITE_OLIVES=:qto, QUANTITE_HUILE=:qth, "
        "RENDEMENT=:rend, TYPE_HUILE=:type, "
        "DUREE_PRESSAGE=:duree, OBSERVATION=:obs "
        "WHERE ID_OPERATION=:id_op"
        );

    query.bindValue(":id_op", id_operation);
    query.bindValue(":date",  date_production);
    query.bindValue(":cin",   id_agri);   // ✅ CIN (pas ID_AGRI)
    query.bindValue(":id_emp", id_emp);
    query.bindValue(":qto",   quantite_olives);
    query.bindValue(":qth",   quantite_huile);
    query.bindValue(":rend",  rendement);
    query.bindValue(":type",  type_huile);
    query.bindValue(":duree", duree_pressage);
    query.bindValue(":obs",   observation);

    if (!query.exec()) {
        lastError = query.lastError().text();
        qDebug() << "[PRODUCTION] Erreur modifier :" << lastError;
        return false;
    }
    return true;
}

// ============================================================
// SUPPRIMER
// ✅ OK — aucun changement nécessaire
// ============================================================
bool Production::supprimer(int id_operation)
{
    QSqlQuery query;
    query.prepare("DELETE FROM PRODUCTION WHERE ID_OPERATION=:id_op");
    query.bindValue(":id_op", id_operation);

    if (!query.exec()) {
        qDebug() << "[PRODUCTION] Erreur supprimer :" << query.lastError().text();
        return false;
    }
    return true;
}

// ============================================================
// AFFICHER
// ✅ CORRIGÉ :
//   - ID_AGRI → CIN
//   - TEMPERATURE_MOYENNE supprimée (inexistante dans la BD)
//   - Colonnes : 10 au lieu de 11
// ============================================================
QSqlQueryModel* Production::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare(
        "SELECT ID_OPERATION, DATE_PRODUCTION, CIN, ID_EMP, "
        "QUANTITE_OLIVES, QUANTITE_HUILE, RENDEMENT, TYPE_HUILE, "
        "DUREE_PRESSAGE, OBSERVATION "
        "FROM PRODUCTION ORDER BY DATE_PRODUCTION DESC"
        );
    query.exec();
    model->setQuery(std::move(query));

    // Index : 0=ID_OP, 1=DATE, 2=CIN, 3=ID_EMP, 4=QTE_OL,
    //         5=QTE_HU, 6=REND, 7=TYPE, 8=DUREE, 9=OBS
    model->setHeaderData(0, Qt::Horizontal, "ID Op.");
    model->setHeaderData(1, Qt::Horizontal, "Date");
    model->setHeaderData(2, Qt::Horizontal, "CIN Agri.");
    model->setHeaderData(3, Qt::Horizontal, "ID Emp.");
    model->setHeaderData(4, Qt::Horizontal, "Qté olives");
    model->setHeaderData(5, Qt::Horizontal, "Qté huile");
    model->setHeaderData(6, Qt::Horizontal, "Rendement");
    model->setHeaderData(7, Qt::Horizontal, "Type huile");
    model->setHeaderData(8, Qt::Horizontal, "Durée pressage");
    model->setHeaderData(9, Qt::Horizontal, "Observation");

    return model;
}
// ─────────────────────────────────────────────────────────
// calculerRendementMoyen
// ─────────────────────────────────────────────────────────
double Production::calculerRendementMoyen()
{
    QSqlQuery q;
    q.exec("SELECT AVG(RENDEMENT) FROM PRODUCTION");
    if (q.next()) return q.value(0).toDouble();
    return 0.0;
}

// ─────────────────────────────────────────────────────────
// rendementParType
// ─────────────────────────────────────────────────────────
QVector<QPair<QString, double>> Production::rendementParType()
{
    QVector<QPair<QString, double>> result;
    QSqlQuery q;
    q.exec("SELECT TYPE_HUILE, AVG(RENDEMENT) FROM PRODUCTION "
           "GROUP BY TYPE_HUILE ORDER BY TYPE_HUILE");
    while (q.next())
        result.append({q.value(0).toString(), q.value(1).toDouble()});
    return result;
}

// ─────────────────────────────────────────────────────────
// detecterAnomaliesRendement
// ─────────────────────────────────────────────────────────
QVector<int> Production::detecterAnomaliesRendement(double seuilMin, double seuilMax)
{
    QVector<int> ids;
    QSqlQuery q;
    q.prepare("SELECT ID_OPERATION FROM PRODUCTION "
              "WHERE RENDEMENT < :min OR RENDEMENT > :max");
    q.bindValue(":min", seuilMin);
    q.bindValue(":max", seuilMax);
    q.exec();
    while (q.next())
        ids.append(q.value(0).toInt());
    return ids;
}

// ─────────────────────────────────────────────────────────
// evolutionRendement
// ─────────────────────────────────────────────────────────
QVector<QPair<QDate, double>> Production::evolutionRendement()
{
    QVector<QPair<QDate, double>> result;
    QSqlQuery q;
    q.exec("SELECT DATE_PRODUCTION, RENDEMENT FROM PRODUCTION "
           "ORDER BY DATE_PRODUCTION ASC");
    while (q.next())
        result.append({q.value(0).toDate(), q.value(1).toDouble()});
    return result;
}

// ─────────────────────────────────────────────────────────
// exporterCSV
// ─────────────────────────────────────────────────────────
bool Production::exporterCSV(const QString &chemin)
{
    QSqlQuery q;
    q.exec("SELECT ID_OPERATION, DATE_PRODUCTION, CIN, ID_EMP, "
           "QUANTITE_OLIVES, QUANTITE_HUILE, RENDEMENT, TYPE_HUILE, "
           "DUREE_PRESSAGE, OBSERVATION FROM PRODUCTION ORDER BY DATE_PRODUCTION DESC");

    QFile file(chemin);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    out << "ID,Date,CIN,ID Emp,Qte Olives,Qte Huile,Rendement,Type Huile,Duree,Observation\n";
    while (q.next()) {
        for (int i = 0; i < 10; i++) {
            out << q.value(i).toString();
            if (i < 9) out << ",";
        }
        out << "\n";
    }
    file.close();
    return true;
}

// ─────────────────────────────────────────────────────────
// rechercherParType
// ─────────────────────────────────────────────────────────
QSqlQueryModel* Production::rechercherParType(const QString &type)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery q;
    q.prepare("SELECT ID_OPERATION, DATE_PRODUCTION, CIN, ID_EMP, "
              "QUANTITE_OLIVES, QUANTITE_HUILE, RENDEMENT, TYPE_HUILE, "
              "DUREE_PRESSAGE, OBSERVATION FROM PRODUCTION "
              "WHERE UPPER(TYPE_HUILE) LIKE UPPER(:type) "
              "ORDER BY DATE_PRODUCTION DESC");
    q.bindValue(":type", "%" + type + "%");
    q.exec();
    model->setQuery(std::move(q));
    return model;
}

// ─────────────────────────────────────────────────────────
// rechercherParPeriode
// ─────────────────────────────────────────────────────────
QSqlQueryModel* Production::rechercherParPeriode(const QDate &debut, const QDate &fin)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery q;
    q.prepare("SELECT ID_OPERATION, DATE_PRODUCTION, CIN, ID_EMP, "
              "QUANTITE_OLIVES, QUANTITE_HUILE, RENDEMENT, TYPE_HUILE, "
              "DUREE_PRESSAGE, OBSERVATION FROM PRODUCTION "
              "WHERE DATE_PRODUCTION BETWEEN :debut AND :fin "
              "ORDER BY DATE_PRODUCTION DESC");
    q.bindValue(":debut", debut);
    q.bindValue(":fin",   fin);
    q.exec();
    model->setQuery(std::move(q));
    return model;
}
