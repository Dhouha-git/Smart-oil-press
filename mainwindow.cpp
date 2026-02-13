#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include <QChartView>
#include <QLineSeries>
#include <QChart>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStandardItemModel *model = new QStandardItemModel(2,3,this);
    model->setHorizontalHeaderLabels({"CIN","Nom","Poste"});

    model->setItem(0,0,new QStandardItem("12345678"));
    model->setItem(0,1,new QStandardItem("Ali"));
    model->setItem(0,2,new QStandardItem("Manager"));

    model->setItem(1,0,new QStandardItem("87654321"));
    model->setItem(1,1,new QStandardItem("Sarra"));
    model->setItem(1,2,new QStandardItem("Technicien"));

    ui->tableView->setModel(model);

    // ⬅️ تنادي createChart بعد ما تحط model
    createChart();
}

void MainWindow::on_btn_login_clicked()
{
    ui->stackedWidget_main->setCurrentWidget(ui->page_dashboard);
}


void MainWindow::on_btn_home_clicked()
{
    ui->stackedWidget_modules->setCurrentWidget(ui->page_home);

}


void MainWindow::on_btn_employes_clicked()
{
     ui->stackedWidget_modules->setCurrentWidget(ui->page_employes);
}


void MainWindow::on_btn_agriculteurs_clicked()
{
    ui->stackedWidget_modules->setCurrentWidget(ui->page_agriculteurs);

}


void MainWindow::on_btn_ajouter_clicked()
{
    ui->stackedWidget_modules->setCurrentWidget(ui->page_ajout_employe);

}


void MainWindow::on_btn_retour_clicked()
{
    ui->stackedWidget_modules->setCurrentWidget(ui->page_employes);

}


void MainWindow::on_btn_valider_clicked()
{

    QString cin = ui->lineEdit_cin->text();
    QString nom = ui->lineEdit_nom->text();

    if(cin.isEmpty() || nom.isEmpty())
    {
        QMessageBox::warning(this,"Attention","Champs obligatoires !");
        return;
    }

    QMessageBox::information(this,"Succès","Employé ajouté (Simulation)");

    ui->lineEdit_cin->clear();
    ui->lineEdit_nom->clear();

    ui->stackedWidget_modules->setCurrentWidget(ui->page_employes);
}
void MainWindow::createChart()
{
    QStandardItemModel *model =
        qobject_cast<QStandardItemModel*>(ui->tableView->model());

    if(!model)
        return;

    int total = model->rowCount();

    QLineSeries *series = new QLineSeries();

    for(int i = 0; i < total; i++)
    {
        series->append(i+1, i+1);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Evolution du nombre d'employés");
    chart->createDefaultAxes();

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumHeight(250);

    ui->verticalLayout_chart->addWidget(chartView);
}

MainWindow::~MainWindow()
{
    delete ui;
}


