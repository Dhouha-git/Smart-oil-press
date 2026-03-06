#include "client.h"
#include <QMessageBox>
#include <QApplication>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    client w;

    bool test = connection::instance()->createConnect();
    qDebug() << "Drivers disponibles :" << QSqlDatabase::drivers();

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DSN=smart"); // impératif
    db.setUserName("TASNIM");
    db.setPassword("tasnim34");

    if (db.open()) {
           qDebug() << "Connexion réussie";
        QMessageBox::information(nullptr, QObject::tr("Database is open"),
                                 QObject::tr("Connection successful"));
w.show();
    } else {
qDebug() << "Erreur complète :" << db.lastError().text();
        QMessageBox::information(nullptr, QObject::tr("Database is not open"),
                                 QObject::tr("Connection failed.\nClick "),
                                 QMessageBox::Cancel);

    }


    return a.exec();
}
