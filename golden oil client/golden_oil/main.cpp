#include <QApplication>
#include <QMessageBox>
#include "connection.h"
#include "client.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>
#include <QString>  // pour QString
#include <QDate>
#include <QSqlTableModel>
#include <QSqlError>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(connection::getInstance()->createConnect())
    {
        QMessageBox::information(nullptr,
                                 "Database",
                                 "Connection successful");

        client w;
        w.show();
        return a.exec();
    }
    else
    {
        QMessageBox::critical(nullptr,
                              "Database",
                              "Connection failed");

        return -1;
    }
}
