#include "client.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connection* conn = Connection::instance();
    bool test = conn->createConnect();

    client w;

    if (test)
    {
        w.show();
        QMessageBox::information(nullptr,
                                 QObject::tr("Database is open"),
                                 QObject::tr("Connection successful."));
    }
    else
    {
        QMessageBox::critical(nullptr,
                              QObject::tr("Database is not open"),
                              QObject::tr("Connection failed."));
    }
 qDebug() << "Test build OK";
    return a.exec();
}
