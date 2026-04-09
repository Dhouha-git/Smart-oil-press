#include "connection.h"
#include <QSqlError>
#include <QDebug>

Connection* Connection::instance = nullptr;

Connection::Connection()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A");
    db.setUserName("SMART");
    db.setPassword("smart123");
}

Connection* Connection::getInstance()
{
    if(instance == nullptr)
        instance = new Connection();

    return instance;
}

bool Connection::createconnect()
{
    if(db.open())
    {
        qDebug() << "Connexion réussie";
        return true;
    }
    else
    {
        qDebug() << "Erreur connexion :" << db.lastError();
        return false;
    }
}

QSqlDatabase Connection::getDatabase()
{
    return db;
}
