
#include "connection.h"
#include <QSqlError>
#include <QDebug>
#include <QSqlDatabase>
// Initialisation du pointeur d'instance
Connection* Connection::p_instance = nullptr;

// Constructeur privé
Connection::Connection()
{
    // Initialisation de la base de données

}

// Méthode statique pour obtenir l'instance unique
Connection* Connection::instance()
{
    if (!p_instance)
        p_instance = new Connection();
    return p_instance;
}

// Méthode pour établir la connexion
bool Connection::createConnect()
{
    bool test = false;
 db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("smart_oil");
    db.setUserName("TASNIM");
    db.setPassword("tasnim34");
    if (db.open()) {
        test = true;
        qDebug() << "Connexion à la base de données réussie";
    } else {
        qDebug() << "Erreur de connexion:" << db.lastError().text();
    }

    return test;
}

// Fermer la connexion
void Connection::closeConnection()
{
    if (db.isOpen()) {
        db.close();
    }
}

// Destructeur privé
Connection::~Connection()
{
    closeConnection();
}
