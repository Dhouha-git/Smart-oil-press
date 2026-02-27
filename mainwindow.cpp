#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QItemSelectionModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    refreshTable(); // affichage au démarrage
}

MainWindow::~MainWindow()
{
    delete ui;
}

////////////////////////////////////////////////////
/// REFRESH TABLE
////////////////////////////////////////////////////

void MainWindow::refreshTable()
{
    ui->tableView->setModel(Etmp.afficher());
}

////////////////////////////////////////////////////
/// NAVIGATION
////////////////////////////////////////////////////

void MainWindow::on_btn_valider_clicked()
{
    QString cin = ui->lineEdit_cin_4->text();
    QString nom = ui->lineEdit_nom_4->text();
    QString prenom = ui->lineEdit_prenom_4->text();

    QDate date_naissance = ui->dateEdit_7->date();
    QString situation = ui->comboBox_18->currentText();

    QString email = cin + "@smart.com"; // مؤقت باش ما يصيرش UNIQUE error

    QString adresse = ui->lineEdit_adresse_4->text();
    QString code_postal = ui->lineEdit_codepostal_4->text();
    QString telephone = ui->lineEdit_tel_4->text();

    QString departement = ui->comboBox_19->currentText();
    QString type_contrat = ui->comboBox_20->currentText();
    QString poste = ui->comboBox_21->currentText();

    QDate date_embauche = ui->dateEdit_7->date();

    double salaire = ui->lineEdit_salaire_4->text().toDouble();
    double primes = ui->lineEdit_primes_4->text().toDouble();

    QString statut = ui->comboBox_22->currentText();

    if(cin.isEmpty() || nom.isEmpty() || prenom.isEmpty())
    {
        QMessageBox::warning(this,"Attention","Champs obligatoires !");
        return;
    }

    Employe e(cin, nom, prenom,
              date_naissance, situation,
              email,
              adresse, code_postal,
              telephone, departement,
              type_contrat, poste,
              date_embauche,
              salaire, primes,
              statut);

    if(e.ajouter())
    {
        QMessageBox::information(this,"Succès","Employé ajouté");
        refreshTable();
        ui->stackedWidget->setCurrentWidget(ui->page_employes);
    }
    else
    {
        QMessageBox::critical(this,"Erreur","Ajout échoué");
    }
}

////////////////////////////////////////////////////
/// SUPPRIMER (par sélection)
////////////////////////////////////////////////////

void MainWindow::on_btn_supprimer_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();

    if(!index.isValid())
    {
        QMessageBox::warning(this,"Attention","Sélectionnez un employé");
        return;
    }

    int id = ui->tableView->model()->data(
                                       ui->tableView->model()->index(index.row(),0)).toInt();

    if(Etmp.supprimer(id))
    {
        QMessageBox::information(this,"Succès","Suppression réussie");
        refreshTable();
    }
    else
    {
        QMessageBox::critical(this,"Erreur","Suppression échouée");
    }
}

void MainWindow::on_btn_retour_3_clicked()
{
     ui->stackedWidget->setCurrentWidget(ui->page_employes);
}


void MainWindow::on_btn_annuler_3_clicked()
{
      ui->stackedWidget->setCurrentWidget(ui->page_employes);
}


void MainWindow::on_btn_employes_5_clicked()
{
      ui->stackedWidget->setCurrentWidget(ui->page_employes);
}


void MainWindow::on_btn_modifier_clicked()
{
      ui->stackedWidget->setCurrentWidget(ui->page_modifier_employe_2);
}


void MainWindow::on_btn_employes_3_clicked()
{
      ui->stackedWidget->setCurrentWidget(ui->page_employes);
}


void MainWindow::on_btn_ajouter_clicked()
{
      ui->stackedWidget->setCurrentWidget(ui->page_ajout_employe);
}
/*void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    int row = index.row();

    // 1. Récupération des données

    QString cin = ui->tableView->model()->data(ui->tableView->model()->index(row, 0)).toString();
    QString nom = ui->tableView->model()->data(ui->tableView->model()->index(row, 1)).toString();
    QString prenom = ui->tableView->model()->data(ui->tableView->model()->index(row, 2)).toString();
    QString date_naissance = ui->tableView->model()->data(ui->tableView->model()->index(row, 3)).toString();
    QString situation = ui->tableView->model()->data(ui->tableView->model()->index(row, 4)).toString();
    QString email = ui->tableView->model()->data(ui->tableView->model()->index(row, 5)).toString();
    QString adresse = ui->tableView->model()->data(ui->tableView->model()->index(row, 6)).toString();
    QString code_postal = ui->tableView->model()->data(ui->tableView->model()->index(row, 7)).toString();
    QString telephone = ui->tableView->model()->data(ui->tableView->model()->index(row, 8)).toString();
    QString departement = ui->tableView->model()->data(ui->tableView->model()->index(row, 9)).toString();
    QString type_contrat = ui->tableView->model()->data(ui->tableView->model()->index(row, 10)).toString();
    QString poste = ui->tableView->model()->data(ui->tableView->model()->index(row, 11)).toString();
    QString date_embauche = ui->tableView->model()->data(ui->tableView->model()->index(row, 12)).toString();
    QString salaire = ui->tableView->model()->data(ui->tableView->model()->index(row, 13)).toString();
    QString primes = ui->tableView->model()->data(ui->tableView->model()->index(row, 14)).toString();
    QString statut = ui->tableView->model()->data(ui->tableView->model()->index(row, 15)).toString();
    // 2. Remplissage des champs de l'interface "Modifier"

    ui->lineEdit_cin_5->setText(cin);
    ui->lineEdit_nom_5->setText(nom);
    ui->lineEdit_prenom_5->setText(prenom);
    ui->dateEdit_7->setText(date_naissance);
    ui->comboBox_27->setText(situation);
    ui->lineEdit_email_5->setText(email);
    ui->lineEdit_adresse_5->setText(adresse);
    ui->lineEdit_codepostal_5->setText(code_postal);
    ui->lineEdit_tel_5->setText(telephone);
    ui->comboBox_23->setText(departement);
    ui->comboBox_24->setText(type_contrat);
    ui->comboBox_25->setText(poste);
    ui->dateEdit_9->setText(date_embauche);
    ui->lineEdit_salaire_5->setText(salaire);
    ui->lineEdit_primes_5->setText(primes);
    ui->comboBox_26->setText(statut);

    // 3. Aller vers la page Modifier
    ui->stackedWidget->setCurrentWidget(ui->page_modifier_employe_2);
}
void MainWindow::on_btn_modifier_valider_clicked()
{
    int id = ui->btn_modifier->text().toInt(); // Ne5thou el ID elli 7atitah f ReadOnly

    // Nsna3ou objet jdid b'data elli dakhaltha tawwa
    Employe e(ui->lineEdit_cin_5->text(),
              ui->lineEdit_nom_5->text(),
              ui->lineEdit_prenom_5->text(),
              ui->dateEdit_7->date(), // QDateEdit
              ui->comboBox_27->currentText(), // ComboBox
              ui->lineEdit_adresse_5->text(),
              ui->lineEdit_codepostal_5->text(),
              ui->lineEdit_tel_5->text(),
              ui->lineEdit_email_5->text(),
              ui->comboBox_23->currentText(),
              ui->comboBox_24->currentText(),
              ui->comboBox_25->currentText(),
              ui->dateEdit_9->date(),
              ui->lineEdit_salaire_5->text().toDouble(),
              ui->lineEdit_primes_5->text().toDouble(),
              ui->comboBox_26->currentText());

    if(e.modifier(id)) {
        QMessageBox::information(this, "Succès", "L'employé a été mis à jour !");
        refreshTable(); // Refresh el TableView
        ui->stackedWidget->setCurrentWidget(ui->page_employes); // Arja3 lel liste
    } else {
        QMessageBox::critical(this, "Erreur", "La mise à jour a échoué.");
    }
}*/

void MainWindow::on_btn_annuler_4_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_employes);
}


void MainWindow::on_btn_retour_4_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_employes);
}


void MainWindow::on_btn_employes_6_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_employes);
}

