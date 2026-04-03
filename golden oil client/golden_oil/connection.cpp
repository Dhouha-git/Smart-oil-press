#include "connection.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>


// Initialisation du singleton
connection* connection::instance = nullptr;
// Constructeur privé
connection::connection()
{
    // Si Qt contient déjà une connexion par défaut
    if (QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QODBC"); // ou "QMYSQL" si MySQL

    db.setDatabaseName("Source_Projet2A"); // DSN ODBC ou nom DB
    db.setUserName("SMART");
    db.setPassword("smart123");
}

// Singleton
connection* connection::getInstance()
{
    if (!instance)
        instance = new connection();
    return instance;
}

// Créer / ouvrir la connexion
bool connection::createConnect()
{
    if (db.isOpen())  // évite de rouvrir
        return true;

    if (db.open())
    {
        qDebug() << "Connexion réussie";
        return true;
    }
    else
    {
        qDebug() << "Erreur connexion :" << db.lastError().text();
        return false;
    }
}

// Accès à la base
QSqlDatabase connection::getDatabase()
{
    return db;
}
