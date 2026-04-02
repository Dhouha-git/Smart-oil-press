#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QString>
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

    QString date_naissance = ui->dateEdit_7->text();
    QString situation = ui->comboBox_18->currentText();


    QString adresse = ui->lineEdit_adresse_4->text();
    QString code_postal = ui->lineEdit_codepostal_4->text();
    QString telephone = ui->lineEdit_tel_4->text();
    QString email = ui->lineEdit_cin_6->text();

    QString departement = ui->comboBox_19->currentText();
    QString type_contrat = ui->comboBox_20->currentText();
    QString poste = ui->comboBox_21->currentText();

    QString date_embauche = ui->dateEdit_7->text();

    double salaire = ui->lineEdit_salaire_4->text().toDouble();
    double primes = ui->lineEdit_primes_4->text().toDouble();

    QString statut = ui->comboBox_22->currentText();

    /// ================= CIN =================
    if(cin.length() != 8 || !cin.toInt())
    {
        QMessageBox::warning(this,"Erreur","CIN doit contenir 8 chiffres");
        return;
    }

    /// ================= NOM =================
    if(nom.isEmpty())
    {
        QMessageBox::warning(this,"Erreur","Nom obligatoire");
        return;
    }

    /// ================= PRENOM =================
    if(prenom.isEmpty())
    {
        QMessageBox::warning(this,"Erreur","Prénom obligatoire");
        return;
    }

    /// ================= TELEPHONE =================
    if(telephone.length() != 8 || !telephone.toInt())
    {
        QMessageBox::warning(this,"Erreur","Téléphone invalide (8 chiffres)");
        return;
    }

    /// ================= EMAIL =================
    QRegularExpression regexEmail("^[\\w\\.]+@[\\w\\.]+\\.[a-zA-Z]{2,}$");
    if(!regexEmail.match(email).hasMatch())
    {
        QMessageBox::warning(this,"Erreur","Email invalide");
        return;
    }

    /// ================= ADRESSE =================
    if(adresse.isEmpty())
    {
        QMessageBox::warning(this,"Erreur","Adresse obligatoire");
        return;
    }

    /// ================= CODE POSTAL =================
    if(code_postal.length() != 4 || !code_postal.toInt())
    {
        QMessageBox::warning(this,"Erreur","Code postal invalide (4 chiffres)");
        return;
    }

    /// ================= DEPARTEMENT =================
    if(departement.isEmpty())
    {
        QMessageBox::warning(this,"Erreur","Choisir département");
        return;
    }

    /// ================= TYPE CONTRAT =================
    if(type_contrat.isEmpty())
    {
        QMessageBox::warning(this,"Erreur","Choisir type contrat");
        return;
    }

    /// ================= POSTE =================
    if(poste.isEmpty())
    {
        QMessageBox::warning(this,"Erreur","Choisir poste");
        return;
    }



    /// ================= STATUT =================
    if(statut.isEmpty())
    {
        QMessageBox::warning(this,"Erreur","Choisir statut");
        return;
    }

    Employe e(cin, nom, prenom,
              date_naissance, situation,email,
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


    int row = index.row();

    int id = ui->tableView->model()->data(
                                       ui->tableView->model()->index(row,0)).toInt();

    QString nom = ui->tableView->model()->data(
                                            ui->tableView->model()->index(row,2)).toString();

    QString prenom = ui->tableView->model()->data(
                                               ui->tableView->model()->index(row,3)).toString();

    /// 🔥 CONFIRMATION avec nom
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  "Confirmation",
                                  "⚠️ Vous êtes sûr de supprimer l'employé : " + nom + " " + prenom + " ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::No)
        return;

    /// 🔴 SUPPRESSION
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
    QModelIndex index = ui->tableView->currentIndex();

    if(!index.isValid())
    {
        QMessageBox::warning(this,"Erreur","Choisir un employé");
        return;
    }

    int row = index.row();


    id_selected = ui->tableView->model()->index(row,0).data().toInt();



    ui->lineEdit_cin_5->setText(
        ui->tableView->model()->index(row,1).data().toString());

    ui->lineEdit_nom_5->setText(
        ui->tableView->model()->index(row,2).data().toString());

    ui->lineEdit_prenom_5->setText(
        ui->tableView->model()->index(row,3).data().toString());

    ui->lineEdit_email_5->setText(
        ui->tableView->model()->index(row,6).data().toString());

    ui->lineEdit_adresse_5->setText(
        ui->tableView->model()->index(row,7).data().toString());

    ui->lineEdit_codepostal_5->setText(
        ui->tableView->model()->index(row,8).data().toString());

    ui->lineEdit_tel_5->setText(
        ui->tableView->model()->index(row,9).data().toString());

    ui->comboBox_23->setCurrentText(
        ui->tableView->model()->index(row,10).data().toString());

    ui->comboBox_24->setCurrentText(
        ui->tableView->model()->index(row,11).data().toString());

    ui->comboBox_25->setCurrentText(
        ui->tableView->model()->index(row,12).data().toString());

    ui->lineEdit_salaire_5->setText(
        ui->tableView->model()->index(row,14).data().toString());

    ui->lineEdit_primes_5->setText(
        ui->tableView->model()->index(row,15).data().toString());

    ui->comboBox_26->setCurrentText(
        ui->tableView->model()->index(row,16).data().toString());



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


void MainWindow::on_btn_valider_4_clicked()
{

        if(id_selected == 0)
        {
            QMessageBox::warning(this,"Erreur","Aucun employé sélectionné");
            return;
        }

        Employe e;

        e.setCin(ui->lineEdit_cin_5->text());
        e.setNom(ui->lineEdit_nom_5->text());
        e.setPrenom(ui->lineEdit_prenom_5->text());
        e.setEmail(ui->lineEdit_email_5->text());
        e.setTelephone(ui->lineEdit_tel_5->text());
        e.setAdresse(ui->lineEdit_adresse_5->text());
        e.setCodePostal(ui->lineEdit_codepostal_5->text());

        e.setDepartement(ui->comboBox_23->currentText());
        e.setTypeContrat(ui->comboBox_24->currentText());
        e.setPoste(ui->comboBox_25->currentText());

        e.setSalaire(ui->lineEdit_salaire_5->text().toDouble());
        e.setPrimes(ui->lineEdit_primes_5->text().toDouble());
        e.setStatut(ui->comboBox_26->currentText());
        e.setDateNaissance(ui->dateEdit_10->text());
        e.setSituation(ui->comboBox_27->currentText());
        e.setDateEmbauche(ui->dateEdit_9->text());
        /// 🎯 UPDATE
        if(e.modifier(id_selected))
        {
            QMessageBox::information(this,"Succès","Modification réussie");
            refreshTable();
            ui->stackedWidget->setCurrentWidget(ui->page_employes);
        }
        else
        {
            QMessageBox::critical(this,"Erreur","Modification échouée");
        }

}
void MainWindow::updateRecherche()
{
    QString val = ui->lineEdit_recherche->text();
    QString filtre = ui->comboBox_filtre->currentText();
    QString tri = ui->comboBox_tri->currentText();

    QSqlQueryModel* model = Etmp.rechercherAvance(val, filtre, tri);

    ui->tableView->setModel(model);


    if(model->rowCount() == 0 && !val.isEmpty())
    {
        QMessageBox::information(this, "Info", "Aucun résultat trouvé");
    }
}
void MainWindow::on_lineEdit_recherche_textChanged(const QString &)
{
    updateRecherche();
}
void MainWindow::on_comboBox_filtre_currentTextChanged(const QString &)
{
    updateRecherche();
}
void MainWindow::on_comboBox_tri_currentTextChanged(const QString &)
{
    updateRecherche();
}
