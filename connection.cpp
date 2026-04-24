#include "connection.h"
#include <QDebug>
#include <QMessageBox>

Connection::Connection()
{

}

QString Connection::lastError = "";

bool Connection::createconnect()
{
    bool test = false;

    // Remove any existing connection to avoid "already exists" error
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A"); // nom de la source de données ODBC
    db.setUserName("SMART");              // nom de l'utilisateur Oracle
    db.setPassword("smart123");           // mot de passe Oracle

    if (db.open()) {
        test = true;
        qDebug() << "[Connection] Connexion réussie à la base de données Oracle.";
    } else {
        lastError = db.lastError().text();
        qDebug() << "[Connection] Échec de connexion:" << lastError;
        QMessageBox::critical(nullptr, "Erreur de Connexion",
            "Impossible de se connecter à la base de données Oracle.\n\n"
            "Source : Source_Projet2A\n"
            "Utilisateur : SMART\n\n"
            "Détail de l'erreur :\n" + lastError);
    }


    return  test;
}
