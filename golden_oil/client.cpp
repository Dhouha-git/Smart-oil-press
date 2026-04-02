#include "client.h"
#include "ui_client.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>
#include <QString>  // pour QString
#include <QDate>
#include <QSqlTableModel>
#include <QSqlError>
client::client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::client)
{
    ui->setupUi(this);
}

client::~client()
{
    delete ui;
}

void client::on_analyse_clicked()
{
     ui->stacked->setCurrentWidget(ui->page_2);
}


void client::on_chatbot_clicked()
{
ui->stacked->setCurrentWidget(ui->page_3);
}


void client::on_pdf_clicked()
{

}


void client::on_pushButton_3_clicked()
{
  ui->stacked->setCurrentWidget(ui->page);
}


void client::on_historique_clicked()
{
ui->stacked->setCurrentWidget(ui->page_4);
}


void client::on_classement_clicked()
{
ui->stacked->setCurrentWidget(ui->page_5);
}


void client::on_pushButton_5_clicked()
{
ui->stacked->setCurrentWidget(ui->page_2);
}


void client::on_pushButton_6_clicked()
{
    ui->stacked->setCurrentWidget(ui->page_2);

}


void client::on_pushButton_4_clicked()
{
         ui->stacked->setCurrentWidget(ui->page_2);
}

