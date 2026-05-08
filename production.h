#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVector>
#include <QPair>


class Production
{
private:
    int     id_operation;
    QDate   date_production;
    int     id_agri;        // correspond à la colonne CIN dans la BD
    int     id_emp;
    double  quantite_olives;
    double  quantite_huile;
    double  rendement;
    QString type_huile;
    int     duree_pressage; // ✅ CORRIGÉ : int au lieu de QString
    QString observation;
    QString lastError;

    // ✅ SUPPRIMÉ : temperature_moyenne (colonne inexistante dans la BD)

public:
    Production();

    // ✅ CORRIGÉ : duree_pressage est int (pas QString)
    Production(QString date, double quantite_olives, double quantite_huile,
               double rendement, QString type_huile,
               int id_agri, int id_emp,
               QString observation, int duree_pressage);

    // Getters
    int     getIdOperation()    const { return id_operation; }
    QDate   getDateProduction() const { return date_production; }
    int     getIdAgri()         const { return id_agri; }
    int     getIdEmp()          const { return id_emp; }
    double  getQuantiteOlives() const { return quantite_olives; }
    double  getQuantiteHuile()  const { return quantite_huile; }
    double  getRendement()      const { return rendement; }
    QString getTypeHuile()      const { return type_huile; }
    int     getDureePressage()  const { return duree_pressage; }
    QString getObservation()    const { return observation; }
    QString getLastError()      const { return lastError; }

    // Setters
    void setIdOperation(int id)      { id_operation = id; }
    void setDateProduction(QDate d)  { date_production = d; }
    void setIdAgri(int id)           { id_agri = id; }
    void setIdEmp(int id)            { id_emp = id; }
    void setQuantiteOlives(double q) { quantite_olives = q; }
    void setQuantiteHuile(double q)  { quantite_huile = q; }
    void setRendement(double r)      { rendement = r; }
    void setTypeHuile(QString t)     { type_huile = t; }
    void setDureePressage(int d)     { duree_pressage = d; }
    void setObservation(QString o)   { observation = o; }

    // CRUD
    bool ajouter();
    bool modifier();
    static bool supprimer(int id_operation);
    QSqlQueryModel* afficher();
    static double calculerRendementMoyen();
    static QVector<QPair<QString, double>> rendementParType();
    static QVector<int> detecterAnomaliesRendement(double seuilMin, double seuilMax);
    static QVector<QPair<QDate, double>> evolutionRendement();
    static bool exporterCSV(const QString &chemin);
    QSqlQueryModel* rechercherParType(const QString &type);
    QSqlQueryModel* rechercherParPeriode(const QDate &debut, const QDate &fin);


};

#endif // PRODUCTION_H
