#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
class Connection
{
public:
    static Connection* instance(); // Accès à l'instance unique
    bool createConnect(); // Méthode pour créer la connexion
    void closeConnection(); // Fermer la connexion

private:
    Connection(); // Constructeur privé
    ~Connection(); // Destructeur privé
    Connection(const Connection&) = delete; // Supprimer le constructeur de copie
    Connection& operator=(const Connection&) = delete; // Supprimer l'opérateur d'affectation

    static Connection* p_instance;
    QSqlDatabase db;
};

#endif // CONNECTION_H
