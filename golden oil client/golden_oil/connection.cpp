#include "connection.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QtSql/QSqlQueryModel>
#include <QString>  // pour QString
#include <QDate>
#include <QSqlTableModel>
#include <QSqlError>
connection* connection::p_instance = nullptr;

connection* connection::instance()
{
    if (!p_instance)
        p_instance = new connection();
    return p_instance;
}

connection::connection()
{
}

connection::~connection()
{
    closeConnection();
}

bool connection::createConnect()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DSN=smart");
    db.setUserName("TASNIM");
    db.setPassword("tasnim34");

    // Affiche le PATH que Qt voit
    qDebug() << "PATH Qt :" << qgetenv("PATH");

    // Liste les drivers disponibles
    qDebug() << "Drivers disponibles :" << QSqlDatabase::drivers();

    // Tente d’ouvrir la base
    if (!db.open()) {
        qDebug() << "Erreur complète :" << db.lastError().text();
        return false;
    } else {
        qDebug() << "Connexion réussie";
        return true;
    }
}

void connection::closeConnection()
{
    if (db.isOpen()) {
        db.close();
        qDebug() << "Connexion fermée";
    }
}
