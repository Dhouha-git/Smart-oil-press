#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btn_home, &QPushButton::clicked, this, [=](){
        ui->stackedWidget_modules->setCurrentWidget(ui->page_home);
    });

    connect(ui->btn_employes, &QPushButton::clicked, this, [=](){
        ui->stackedWidget_modules->setCurrentWidget(ui->page_employes);
    });

    connect(ui->btn_agriculteurs, &QPushButton::clicked, this, [=](){
        ui->stackedWidget_modules->setCurrentWidget(ui->page_agriculteurs);
    });

    connect(ui->btn_clients, &QPushButton::clicked, this, [=](){
        ui->stackedWidget_modules->setCurrentWidget(ui->page_clients);
    });

    connect(ui->btn_production, &QPushButton::clicked, this, [=](){
        ui->stackedWidget_modules->setCurrentWidget(ui->page_production);
    });

    connect(ui->btn_ventes, &QPushButton::clicked, this, [=](){
        ui->stackedWidget_modules->setCurrentWidget(ui->page_ventes);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
