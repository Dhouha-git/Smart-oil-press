#include "connection.h"
#include <QDebug>
#include <QSqlError>

void testConnexion() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    
    // Test 1 : Avec DSN
    db.setDatabaseName("SmartOilPress");
    db.setUserName("SMART");
    db.setPassword("smart123");
    
    if (db.open()) {
        qDebug() << "✅ Test 1 réussi : DSN OK";
        db.close();
    } else {
        qDebug() << "❌ Test 1 échoué :" << db.lastError().text();
    }
    
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    
    // Test 2 : Connexion directe
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Driver={Oracle in XE};Server=localhost;Port=1521;Database=XE;Uid=SMART;Pwd=smart123;");
    
    if (db.open()) {
        qDebug() << "✅ Test 2 réussi : Connexion directe OK";
    } else {
        qDebug() << "❌ Test 2 échoué :" << db.lastError().text();
    }
}

Connection::Connection()
{

}

bool Connection::createConnection()
{
    // Vérifier si la connexion existe déjà
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
        if (db.isOpen()) {
            return true;
        }
    } else {
        QSqlDatabase::addDatabase("QODBC", "qt_sql_default_connection");
    }

    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
    // On passe directement par le pilote complet (contourne le bug des caractères/Unicode)
    QString connectionString = "Driver={Oracle in XE};DBQ=XE;Uid=SMART;Pwd=smart123;";
    db.setDatabaseName(connectionString);

    if (!db.open()) {
        qDebug() << "❌ Erreur de connexion détaillée :";
        qDebug() << "Driver:" << db.driverName();
        qDebug() << "DSN:" << db.databaseName();
        qDebug() << "Erreur:" << db.lastError().text();
        return false;
    }

    qDebug() << "✅ Connexion réussie à Oracle !";
    return true;
}
