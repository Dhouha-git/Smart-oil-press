#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

class Connection
{
private:
    static Connection* instance;
    QSqlDatabase db;

public:
    Connection();

    // Singleton (collègue)
    static Connection* getInstance();
    QSqlDatabase getDatabase();
    bool createconnect();

    // Direct (toi)
    bool createConnection();
    void testConnexion();
};

#endif // CONNECTION_H
