#include <QApplication>
#include <QMessageBox>
#include "connection.h"
#include "client.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(Connection::getInstance()->createconnect())
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
