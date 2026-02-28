#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QString>  // pour QString
#include <QDate>
#include <QSqlTableModel>
class connection
{
private:
    static connection* p_instance;
    QSqlDatabase db;

    connection();      // constructeur privé (singleton)
    ~connection();     // destructeur

public:
    static connection* instance();  // accès à l'instance unique

    bool createConnect();           // ouvrir connexion
    void closeConnection();         // fermer connexion
};

#endif // CONNECTION_H
