#include "connection.h"
#include <QSqlError>
#include <QDebug>

Connection* Connection::instance = nullptr;

// ════════════════════════════════════════════════════
// Constructeur collègue — DSN "Source_Projet2A"
// ════════════════════════════════════════════════════
Connection::Connection()
{
    // Eviter d'ajouter la DB deux fois
    if (!QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::addDatabase("QODBC", "qt_sql_default_connection");
        db.setDatabaseName("Source_Projet2A");
        db.setUserName("SMART");
        db.setPassword("smart123");
    } else {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
}

// ════════════════════════════════════════════════════
// SINGLETON
// ════════════════════════════════════════════════════
Connection* Connection::getInstance()
{
    if (instance == nullptr)
        instance = new Connection();
    return instance;
}

QSqlDatabase Connection::getDatabase()
{
    return db;
}

// ════════════════════════════════════════════════════
// createconnect() — méthode collègue (DSN)
// ════════════════════════════════════════════════════
bool Connection::createconnect()
{
    if (db.isOpen())
        return true;

    if (db.open()) {
        qDebug() << "✅ Connexion réussie (DSN)";
        return true;
    }

    qDebug() << "❌ Erreur connexion (DSN):" << db.lastError().text();
    return false;
}

// ════════════════════════════════════════════════════
// createConnection() — méthode toi (connection string directe)
// fallback si DSN échoue
// ════════════════════════════════════════════════════
bool Connection::createConnection()
{
    // Si déjà ouverte via createconnect() → OK
    if (db.isOpen())
        return true;

    // Essai 1 — DSN
    if (createconnect())
        return true;

    // Essai 2 — Connection string directe (fallback)
    qDebug() << "⚠️ DSN échoué, tentative connexion directe...";

    if (!QSqlDatabase::contains("qt_sql_direct")) {
        QSqlDatabase::addDatabase("QODBC", "qt_sql_direct");
    }

    QSqlDatabase dbDirect = QSqlDatabase::database("qt_sql_direct");
    dbDirect.setDatabaseName(
        "Driver={Oracle in XE};DBQ=XE;Uid=SMART;Pwd=smart123;"
        );

    if (dbDirect.open()) {
        qDebug() << "✅ Connexion directe réussie !";
        db = dbDirect; // utiliser cette connexion
        return true;
    }

    qDebug() << "❌ Connexion directe échouée:" << dbDirect.lastError().text();
    return false;
}

// ════════════════════════════════════════════════════
// testConnexion()
// ════════════════════════════════════════════════════
void Connection::testConnexion()
{
    qDebug() << "=== Test connexion BD ===";

    // Test DSN
    QSqlDatabase t1 = QSqlDatabase::addDatabase("QODBC", "test_dsn");
    t1.setDatabaseName("Source_Projet2A");
    t1.setUserName("SMART");
    t1.setPassword("smart123");

    if (t1.open()) {
        qDebug() << "✅ Test DSN OK";
        t1.close();
    } else {
        qDebug() << "❌ Test DSN échoué:" << t1.lastError().text();
    }
    QSqlDatabase::removeDatabase("test_dsn");

    // Test connexion directe
    QSqlDatabase t2 = QSqlDatabase::addDatabase("QODBC", "test_direct");
    t2.setDatabaseName("Driver={Oracle in XE};DBQ=XE;Uid=SMART;Pwd=smart123;");

    if (t2.open()) {
        qDebug() << "✅ Test connexion directe OK";
        t2.close();
    } else {
        qDebug() << "❌ Test direct échoué:" << t2.lastError().text();
    }
    QSqlDatabase::removeDatabase("test_direct");
}
