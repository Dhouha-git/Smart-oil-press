#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlDatabase>

class connection
{
private:
    static connection* instance;
    QSqlDatabase db;

    connection();  // constructeur privé

public:
    static connection* getInstance();       // Accès singleton
    bool createConnect();                   // Ouvre la connexion
    QSqlDatabase getDatabase();             // Accès à la base
};

#endif // CONNECTION_H
