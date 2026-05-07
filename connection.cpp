#include "connection.h"
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

Connection* Connection::instance = nullptr;

Connection::Connection()
{
    db = QSqlDatabase::addDatabase("QODBC");
    // Driver Oracle natif détecté : "Oracle in XE"
    db.setDatabaseName("Driver={Oracle in XE};DBQ=localhost/XE;UID=SMART;PWD=smart123;");
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
        qDebug() << "✅ Connexion Oracle réussie";
        return true;
    }
    else
    {
        qDebug() << "❌ Erreur connexion :" << db.lastError().text();
        QMessageBox::critical(nullptr,
                              "Erreur de connexion",
                              "Impossible de se connecter à Oracle.\n\n"
                              + db.lastError().databaseText()
                              + "\n\nVérifiez que Oracle XE est démarré et que le driver ODBC est installé.");
        return false;
    }
}

QSqlDatabase Connection::getDatabase()
{
    return db;
}
