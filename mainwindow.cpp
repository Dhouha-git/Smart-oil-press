#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QString>
#include <QItemSelectionModel>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->login);
     refreshTable();
    refreshTableClient();
     refreshTableVente();
    refreshTableAgriculteur();
    ui->label_17->setPixmap(
        QPixmap(":/images/images/login-removebg-preview.png"));
    ui->label_20->setPixmap(
        QPixmap(":/images/images/lg-removebg-preview.png"));
    ui->label_7->setPixmap(
        QPixmap(":/images/images/lg-removebg-preview.png"));
    ui->label_62->setPixmap(
        QPixmap(":/images/images/lg-removebg-preview.png"));
    ui->label_54->setPixmap(
        QPixmap(":/images/images/lg-removebg-preview.png"));
    ui->label_36->setPixmap(
        QPixmap(":/images/images/lg-removebg-preview.png"));
    ui->label_65->setPixmap(
        QPixmap(":/images/images/lg-removebg-preview.png"));

    ui->label_164->setPixmap(
        QPixmap(":/images/images/lg-removebg-preview.png"));
    ui->label_23->setPixmap(
        QPixmap(":/images/images/lg-removebg-preview.png"));
    ui->label_66->setPixmap(
        QPixmap(":/images/images/lg-removebg-preview.png"));
    ui->label_30->setPixmap(
        QPixmap(":/images/images/lg-removebg-preview.png"));




}

////////////////////////////////////////////////////
/// REFRESH TABLE
////////////////////////////////////////////////////

void MainWindow::refreshTable()
{
    ui->tableView->setModel(Etmp.afficher());
}

void MainWindow::refreshTableClient()
{
    ui->tableView_client->setModel(Ctmp.afficher());
}

////////////////////////////////////////////////////
/// NAVIGATION
////////////////////////////////////////////////////

void MainWindow::on_btn_valider_3_clicked()
{
    QString cin = ui->lineEdit_cin_4->text();
    QString nom = ui->lineEdit_nom_4->text();
    QString prenom = ui->lineEdit_prenom_4->text();

    QString date_naissance = ui->dateEdit_7->text();
    QString situation = ui->comboBox_18->currentText();
    QString email = ui->lineEdit_cin_6->text();

    QString adresse = ui->lineEdit_adresse_4->text();
    QString code_postal = ui->lineEdit_codepostal_4->text();
    QString telephone = ui->lineEdit_tel_4->text();


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
/// SUPPRIMER employe (par sélection)
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
//// modifier employe


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
    e.setDateNaissance(ui->dateEdit_10->text());
    e.setSituation(ui->comboBox_27->currentText());
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



void MainWindow::on_pushButton_10_clicked()
{
    // ✅ Plus besoin de lire id_vente
    int    id_client = ui->lineEdit_3->text().toInt();
    double prix      = ui->doubleSpinBox->value();
    QDate  date      = ui->dateEdit->date();
    int    qte       = ui->spinBox->value();
    double montant   = ui->doubleSpinBox_2->value();

    if (id_client <= 0) {
        QMessageBox::warning(this, "Erreur", "ID client invalide !");
        return;
    }

    // ✅ Constructeur sans id_vente
    Vente v(0, id_client, prix, date, qte, montant);

    if (v.ajouter()) {
        QMessageBox::information(this, "Succès", "Vente ajoutée !");
        refreshTableVente();
        viderFormulaire();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout.");
    }
}

void MainWindow::on_btn_login_3_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->dashboard);
}


void MainWindow::on_btn_home_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->dashboard);
}


void MainWindow::on_btn_employes_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_employes);
}


void MainWindow::on_btn_clients_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_clients_2);
}


void MainWindow::on_btn_production_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_production);
}


void MainWindow::on_btn_ventes_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_ventes);
}


void MainWindow::on_btn_ajouter_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_ajout_employe);
}

void MainWindow::on_analyse_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_analyse_client);
}


void MainWindow::on_chatbot_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_chat_client);
}


void MainWindow::on_pdf_clicked()
{

}


void MainWindow::on_btn_annuler_3_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_employes);
}


void MainWindow::on_btn_retour_3_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_employes);
}


void MainWindow::on_btn_agriculteurs_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_agriculteurs);
}


void MainWindow::on_btn_agriculteurs_13_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_agriculteurs);
}


void MainWindow::on_btn_clients_13_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_clients_2);
}


void MainWindow::on_btn_employes_13_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_employes);
}


void MainWindow::on_btn_home_13_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->dashboard);
}


void MainWindow::on_btn_production_13_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_production);
}


void MainWindow::on_btn_ventes_13_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_ventes);
}


void MainWindow::on_btn_agriculteurs_5_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_agriculteurs);
}


void MainWindow::on_btn_clients_5_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_clients_2);
}


void MainWindow::on_btn_employes_5_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_employes);
}


void MainWindow::on_btn_home_5_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->dashboard);
}


void MainWindow::on_btn_production_5_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_production);
}


void MainWindow::on_btn_ventes_5_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_ventes);
}


void MainWindow::on_btn_agriculteurs_8_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_agriculteurs);
}


void MainWindow::on_btn_clients_8_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_clients_2);
}


void MainWindow::on_btn_employes_8_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_employes);
}


void MainWindow::on_btn_home_8_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->dashboard);
}


void MainWindow::on_btn_production_8_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_production);
}


void MainWindow::on_btn_ventes_8_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_ventes);
}


void MainWindow::on_btn_agriculteurs_4_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_agriculteurs);
}


void MainWindow::on_btn_clients_4_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_clients_2);
}


void MainWindow::on_btn_employes_4_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_employes);
}


void MainWindow::on_btn_home_4_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->dashboard);
}


void MainWindow::on_btn_production_4_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_production);
}


void MainWindow::on_btn_ventes_4_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_ventes);
}


void MainWindow::on_btn_agriculteurs_10_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_agriculteurs);
}


void MainWindow::on_btn_clients_10_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_clients_2);
}


void MainWindow::on_btn_employes_10_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_employes);
}


void MainWindow::on_btn_home_10_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->dashboard);
}


void MainWindow::on_btn_production_10_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_production);
}


void MainWindow::on_btn_ventes_10_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_ventes);
}


void MainWindow::on_btn_agriculteurs_7_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_agriculteurs);
}


void MainWindow::on_btn_clients_7_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_clients_2);
}


void MainWindow::on_btn_employes_7_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_employes);
}


void MainWindow::on_btn_home_7_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->dashboard);
}


void MainWindow::on_btn_production_7_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_production);
}


void MainWindow::on_btn_ventes_7_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_ventes);
}


void MainWindow::on_btn_agriculteurs_3_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_agriculteurs);
}


void MainWindow::on_btn_clients_3_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_clients_2);
}


void MainWindow::on_btn_employes_3_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_employes);
}


void MainWindow::on_btn_home_3_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->dashboard);
}


void MainWindow::on_btn_production_3_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_production);
}


void MainWindow::on_btn_ventes_3_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_ventes);
}


void MainWindow::on_btn_agriculteurs_9_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_agriculteurs);
}


void MainWindow::on_btn_clients_9_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_clients_2);
}


void MainWindow::on_btn_employes_9_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_employes);
}


void MainWindow::on_btn_home_9_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->dashboard);
}


void MainWindow::on_btn_production_9_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_production);
}


void MainWindow::on_btn_ventes_9_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_ventes);
}


void MainWindow::on_btn_agriculteurs_6_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_agriculteurs);
}


void MainWindow::on_btn_clients_6_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_clients_2);
}


void MainWindow::on_btn_employes_6_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_employes);
}


void MainWindow::on_btn_home_6_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->dashboard);
}


void MainWindow::on_btn_production_6_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_production);
}


void MainWindow::on_btn_ventes_6_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_ventes);
}


void MainWindow::on_btn_agriculteurs_11_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_agriculteurs);
}


void MainWindow::on_btn_clients_11_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_clients_2);
}


void MainWindow::on_btn_employes_11_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_employes);
}


void MainWindow::on_btn_home_11_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->dashboard);
}


void MainWindow::on_btn_production_11_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_production);
}


void MainWindow::on_btn_ventes_11_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_ventes);
}


void MainWindow::on_btn_agriculteurs_12_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_agriculteurs);
}


void MainWindow::on_btn_clients_12_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_clients_2);
}


void MainWindow::on_btn_employes_12_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_employes);
}


void MainWindow::on_btn_home_12_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->dashboard);
}


void MainWindow::on_btn_production_12_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_production);
}


void MainWindow::on_btn_ventes_12_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_ventes);
}


void MainWindow::on_connecter_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->dashboard);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_historique_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_historique_achat);
}


void MainWindow::on_pushButton_20_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_analyse_agri);
}


void MainWindow::on_pushButton_21_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_classement_agri);
}


void MainWindow::on_btn_retour_5_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_agriculteurs);
}


void MainWindow::on_btn_retour_6_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_clients_2);
}


void MainWindow::on_btn_retour_7_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_clients_2);
}


void MainWindow::on_btn_retour_8_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_agriculteurs);
}


void MainWindow::on_btn_retour_9_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_clients_2);
}


void MainWindow::on_btn_retour_10_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_clients_2);
}


void MainWindow::on_btn_retour_4_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_employes);
}


void MainWindow::on_btn_annuler_4_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_employes);
}

void MainWindow::viderFormulaire()
{
    // ✅ Plus de lineEdit_2 (ID Vente supprimé du formulaire)
    ui->lineEdit_3->clear();           // ID Client
    ui->doubleSpinBox->setValue(0);    // Prix
    ui->dateEdit->setDate(QDate::currentDate());
    ui->spinBox->setValue(0);          // Quantité
    ui->doubleSpinBox_2->setValue(0);  // Montant
    id_vente_selected = 0;             // reset
}

void MainWindow::refreshTableVente()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT ID_VENTE, ID_CLIENT, PRIX_UNITAIRE, "
                  "TO_CHAR(DATE_VENTE,'DD/MM/YYYY') AS DATE_VENTE, "
                  "QUANTITE_VENDUE, MONTANT_PAYE "
                  "FROM SMART.VENTE ORDER BY ID_VENTE");
    query.exec();
    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, "ID Vente");
    model->setHeaderData(1, Qt::Horizontal, "ID Client");
    model->setHeaderData(2, Qt::Horizontal, "Prix Unitaire");
    model->setHeaderData(3, Qt::Horizontal, "Date Vente");
    model->setHeaderData(4, Qt::Horizontal, "Quantité");
    model->setHeaderData(5, Qt::Horizontal, "Montant Payé");

    ui->tableView_vente->setModel(model);
    ui->tableView_vente->resizeColumnsToContents();
    ui->tableView_vente->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_vente->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::on_tableView_vente_clicked(const QModelIndex &index)
{
    int row = index.row();
    QAbstractItemModel *model = ui->tableView_vente->model();

    // ✅ Pas d'ID Vente dans le formulaire — on le stocke en variable
    id_vente_selected = model->data(model->index(row, 0)).toInt();

    ui->lineEdit_3->setText(
        model->data(model->index(row, 1)).toString()); // ID Client
    ui->doubleSpinBox->setValue(
        model->data(model->index(row, 2)).toDouble()); // Prix
    ui->dateEdit->setDate(QDate::fromString(
        model->data(model->index(row, 3)).toString(), "dd/MM/yyyy")); // Date
    ui->spinBox->setValue(
        model->data(model->index(row, 4)).toInt());    // Quantité
    ui->doubleSpinBox_2->setValue(
        model->data(model->index(row, 5)).toDouble()); // Montant
}
// ─── Slots CRUD ───────────────────────────────────────────────────────────────
void MainWindow::on_ajouterVente()
{
    int    id_client = ui->lineEdit_3->text().toInt();
    double prix      = ui->doubleSpinBox->value();
    QDate  date      = ui->dateEdit->date();
    int    qte       = ui->spinBox->value();
    double montant   = ui->doubleSpinBox_2->value();

    if (id_client <= 0) {
        QMessageBox::warning(this, "Erreur", "ID client invalide !");
        return;
    }

    // ✅ Constructeur sans id_vente
    Vente v(0, id_client, prix, date, qte, montant);

    if (v.ajouter()) {
        QMessageBox::information(this, "Succès", "Vente ajoutée !");
        refreshTableVente();
        viderFormulaire();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout.");
    }
}

void MainWindow::on_modifierVente_clicked()
{
    if (id_vente_selected <= 0) {
        QMessageBox::warning(this, "Erreur",
                             "Sélectionnez une vente dans le tableau !");
        return;
    }

    int    id_client = ui->lineEdit_3->text().toInt();
    double prix      = ui->doubleSpinBox->value();
    QDate  date      = ui->dateEdit->date();
    int    qte       = ui->spinBox->value();
    double montant   = ui->doubleSpinBox_2->value();

    // ✅ id_vente_selected récupéré depuis le clic sur la ligne
    Vente v(id_vente_selected, id_client, prix, date, qte, montant);

    if (v.modifier()) {
        QMessageBox::information(this, "Succès", "Vente modifiée !");
        id_vente_selected = 0;  // reset
        refreshTableVente();
        viderFormulaire();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification.");
    }
}
void MainWindow::on_annulerVente_clicked()
{
    if (id_vente_selected <= 0) {
        QMessageBox::warning(this, "Erreur",
                             "Sélectionnez une vente dans le tableau !");
        return;
    }

    if (QMessageBox::question(this, "Confirmation",
                              "Supprimer la vente n° "
                                  + QString::number(id_vente_selected) + " ?",
                              QMessageBox::Yes | QMessageBox::No)
        == QMessageBox::No) return;

    Vente v;
    if (v.annuler(id_vente_selected)) {
        QMessageBox::information(this, "Succès", "Vente supprimée !");
        id_vente_selected = 0;  // reset
        refreshTableVente();
        viderFormulaire();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression.");
    }
}
void MainWindow::on_afficherVente()
{
    int idVente = ui->lineEdit_3->text().toInt();

    if (idVente <= 0) {
        QMessageBox::warning(this, "Erreur", "Entrez un ID valide.");
        return;
    }

    Vente v;
    if (v.afficher(idVente)) {
        ui->lineEdit_3->setText(QString::number(v.getIdClient()));
        ui->doubleSpinBox->setValue(v.getPrixUnitaire());
        ui->dateEdit->setDate(v.getDateVente());
        ui->spinBox->setValue(v.getQuantiteVendue());
        ui->doubleSpinBox_2->setValue(v.getMontantPaye());
    } else {
        QMessageBox::warning(this, "Introuvable",
                             "Aucune vente avec l'ID : " + QString::number(idVente));
    }
}
void MainWindow::on_listerVentes()
{
    refreshTableVente();
}
void MainWindow::on_resetFormulaire()
{
    viderFormulaire();
}

///client
void MainWindow::on_ajouter_3_clicked()
{
    QString nom             = ui->nom_client_3->text();
    QString prenom          = ui->prenom_client_3->text();
    QString telephone       = ui->tel_3->text();
    QString email           = ui->email_3->text();
    QString adresse         = ui->adresse_3->text();
    QDate   date            = ui->date_3->date();
    QString type_client     = ui->comboBox_client_3->currentText();
    QString type_huile      = ui->comboBox_client_4->currentText();
    QString emballage       = ui->emballage_3->currentText();
    QString total_achat     = ui->tot_achat->text();
    QString points_fidelite = ui->point_fid->text();

    if (nom.isEmpty() || prenom.isEmpty() || telephone.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Nom, prénom et téléphone obligatoires.");
        return;
    }
    if (telephone.length() < 8) {
        QMessageBox::warning(this, "Erreur", "Téléphone invalide (8 chiffres minimum).");
        return;
    }

    // ✅ Constructeur sans ID — auto-increment Oracle
    Client c(nom, prenom, telephone, email, adresse,
             date, type_client, type_huile,
             emballage, total_achat, points_fidelite);

    if (c.ajouter()) {
        QMessageBox::information(this, "Succès", "Client ajouté !");
        viderFormulaireClient();
        refreshTableClient();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout client.");
    }
}


// ================= SUPPRIMER CLIENT =================
void MainWindow::on_supprimer_3_clicked()
{
    QString idText = ui->id_client->text().trimmed();

    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID !");
        return;
    }
    bool ok;
    int id = idText.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Erreur", "ID invalide !");
        return;
    }
    if (QMessageBox::question(this, "Confirmation",
                              "Voulez-vous supprimer ce client ?",
                              QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        return;

    if (Client::supprimer(id)) {
        QMessageBox::information(this, "Succès", "Client supprimé avec succès.");
        refreshTableClient();
        ui->id_client->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression.");
    }
}

// ================= MODIFIER CLIENT =================
void MainWindow::on_modifier_3_clicked()
{
    bool ok;
    int id = ui->id_client->text().toInt(&ok);
    if (!ok || id <= 0) {
        QMessageBox::warning(this, "Erreur", "ID invalide !");
        return;
    }

    QString nom             = ui->nom_client_3->text();
    QString prenom          = ui->prenom_client_3->text();
    QString telephone       = ui->tel_3->text();
    QString email           = ui->email_3->text();
    QString adresse         = ui->adresse_3->text();
    QDate   date            = ui->date_3->date();
    QString type_client     = ui->comboBox_client_3->currentText();
    QString type_huile      = ui->comboBox_client_4->currentText();
    QString emballage       = ui->emballage_3->currentText();
    QString total_achat     = ui->tot_achat->text();
    QString points_fidelite = ui->point_fid->text();

    if (nom.isEmpty() || prenom.isEmpty() || telephone.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Nom, prénom et téléphone obligatoires.");
        return;
    }
    if (telephone.length() < 8) {
        QMessageBox::warning(this, "Erreur", "Téléphone invalide (8 chiffres minimum).");
        return;
    }
    if (!email.isEmpty()) {
        QRegularExpression regexEmail("^[\\w\\.]+@[\\w\\.]+\\.[a-zA-Z]{2,}$");
        if (!regexEmail.match(email).hasMatch()) {
            QMessageBox::warning(this, "Erreur", "Email invalide !");
            return;
        }
    }

    // ✅ Constructeur avec ID pour la modification
    Client c(id, nom, prenom, telephone, email, adresse,
             date, type_client, type_huile,
             emballage, total_achat, points_fidelite);

    if (c.modifier()) {
        QMessageBox::information(this, "Succès", "Client modifié avec succès !");
        viderFormulaireClient();
        refreshTableClient();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification.");
    }
}

// ================= RECHERCHE CLIENT =================
void MainWindow::on_recherche1_2_clicked()
{
    QString texte = ui->recherche1->text().trimmed();

    // Si vide → tout afficher
    if (texte.isEmpty()) {
        refreshTableClient();
        return;
    }

    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT ID, NOM, PRENOM, TELEPHONE, EMAIL, ADRESSE, "
                  "DATE_INSCRIPTION, TYPE_CLIENT, TYPE_HUILE_PREFERE, "
                  "EMBALLAGE_PREFERE, TOTAL_ACHAT, POINTS_FIDELITE "
                  "FROM SMART.CLIENT "
                  "WHERE UPPER(NOM) LIKE UPPER(:val) "
                  "OR UPPER(PRENOM) LIKE UPPER(:val) "
                  "OR TO_CHAR(ID) LIKE :val");
    query.bindValue(":val", "%" + texte + "%");
    query.exec();
    model->setQuery(std::move(query));

    model->setHeaderData(0,  Qt::Horizontal, "ID");
    model->setHeaderData(1,  Qt::Horizontal, "Nom");
    model->setHeaderData(2,  Qt::Horizontal, "Prénom");
    model->setHeaderData(3,  Qt::Horizontal, "Téléphone");
    model->setHeaderData(4,  Qt::Horizontal, "Email");
    model->setHeaderData(5,  Qt::Horizontal, "Adresse");
    model->setHeaderData(6,  Qt::Horizontal, "Date inscription");
    model->setHeaderData(7,  Qt::Horizontal, "Type client");
    model->setHeaderData(8,  Qt::Horizontal, "Huile préférée");
    model->setHeaderData(9,  Qt::Horizontal, "Emballage");
    model->setHeaderData(10, Qt::Horizontal, "Total achat");
    model->setHeaderData(11, Qt::Horizontal, "Points fidélité");

    ui->tableView_client->setModel(model);

    if (model->rowCount() == 0)
        QMessageBox::information(this, "Résultat", "Aucun client trouvé !");
}

// ================= TRI CLIENT =================
void MainWindow::on_tri_2_clicked()
{
    QString texte = ui->tri->text().trimmed();

    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    if (texte.isEmpty()) {
        // Tri par nom par défaut
        query.prepare("SELECT ID, NOM, PRENOM, TELEPHONE, EMAIL, ADRESSE, "
                      "DATE_INSCRIPTION, TYPE_CLIENT, TYPE_HUILE_PREFERE, "
                      "EMBALLAGE_PREFERE, TOTAL_ACHAT, POINTS_FIDELITE "
                      "FROM SMART.CLIENT ORDER BY NOM ASC");
    } else {
        // Filtre par type client ou nom + tri
        query.prepare("SELECT ID, NOM, PRENOM, TELEPHONE, EMAIL, ADRESSE, "
                      "DATE_INSCRIPTION, TYPE_CLIENT, TYPE_HUILE_PREFERE, "
                      "EMBALLAGE_PREFERE, TOTAL_ACHAT, POINTS_FIDELITE "
                      "FROM SMART.CLIENT "
                      "WHERE UPPER(NOM) LIKE UPPER(:val) "
                      "OR UPPER(TYPE_CLIENT) LIKE UPPER(:val) "
                      "ORDER BY NOM ASC");
        query.bindValue(":val", "%" + texte + "%");
    }

    query.exec();
    model->setQuery(std::move(query));

    model->setHeaderData(0,  Qt::Horizontal, "ID");
    model->setHeaderData(1,  Qt::Horizontal, "Nom");
    model->setHeaderData(2,  Qt::Horizontal, "Prénom");
    model->setHeaderData(3,  Qt::Horizontal, "Téléphone");
    model->setHeaderData(4,  Qt::Horizontal, "Email");
    model->setHeaderData(5,  Qt::Horizontal, "Adresse");
    model->setHeaderData(6,  Qt::Horizontal, "Date inscription");
    model->setHeaderData(7,  Qt::Horizontal, "Type client");
    model->setHeaderData(8,  Qt::Horizontal, "Huile préférée");
    model->setHeaderData(9,  Qt::Horizontal, "Emballage");
    model->setHeaderData(10, Qt::Horizontal, "Total achat");
    model->setHeaderData(11, Qt::Horizontal, "Points fidélité");

    ui->tableView_client->setModel(model);

    if (model->rowCount() == 0)
        QMessageBox::information(this, "Résultat", "Aucun client trouvé !");
}

// ================= VIDER FORMULAIRE CLIENT =================
void MainWindow::viderFormulaireClient()
{
    ui->id_client->clear();
    ui->nom_client_3->clear();
    ui->prenom_client_3->clear();
    ui->tel_3->clear();
    ui->email_3->clear();
    ui->adresse_3->clear();
    ui->tot_achat->clear();
    ui->point_fid->clear();
    ui->date_3->setDate(QDate::currentDate());
    ui->comboBox_client_3->setCurrentIndex(0);
    ui->comboBox_client_4->setCurrentIndex(0);
    ui->emballage_3->setCurrentIndex(0);
}
void MainWindow::on_tableView_client_clicked(const QModelIndex &index)
{
    int row = index.row();
    QAbstractItemModel *model = ui->tableView_client->model();

    // Remplir l'ID (champ caché ou visible)
    ui->id_client->setText(model->data(model->index(row, 0)).toString());

    // Remplir les autres champs du formulaire
    ui->nom_client_3->setText(model->data(model->index(row, 1)).toString());
    ui->prenom_client_3->setText(model->data(model->index(row, 2)).toString());
    ui->tel_3->setText(model->data(model->index(row, 3)).toString());
    ui->email_3->setText(model->data(model->index(row, 4)).toString());
    ui->adresse_3->setText(model->data(model->index(row, 5)).toString());
    ui->date_3->setDate(model->data(model->index(row, 6)).toDate());
    ui->comboBox_client_3->setCurrentText(model->data(model->index(row, 7)).toString());
    ui->comboBox_client_4->setCurrentText(model->data(model->index(row, 8)).toString());
    ui->emballage_3->setCurrentText(model->data(model->index(row, 9)).toString());
    ui->tot_achat->setText(model->data(model->index(row, 10)).toString());
    ui->point_fid->setText(model->data(model->index(row, 11)).toString());
}




void MainWindow::on_btn_ajouter_2_clicked()
{
    QString cinStr     = ui->cin->text();
    QString nom_val    = ui->nom->text();
    QString prenom_val = ui->lineEdit_4->text();
    QString telStr     = ui->lineEdit_9->text();
    QString email_val  = ui->lineEdit_11->text();
    QString region_val = ui->lineEdit_12->text();
    QString type_olive_val = ui->comboBox_olive->currentText();
    QString volStr     = ui->lineEdit_10->text();

    if(cinStr.isEmpty() || nom_val.isEmpty() || prenom_val.isEmpty() ||
        telStr.isEmpty() || email_val.isEmpty() || region_val.isEmpty() || volStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs.");
        return;
    }
    bool okCin;
    int cin_val = cinStr.toInt(&okCin);
    if(!okCin || cinStr.length() != 8) {
        QMessageBox::warning(this, "Erreur", "CIN doit comporter exactement 8 chiffres.");
        return;
    }
    bool okTel;
    int tel_val = telStr.toInt(&okTel);
    if(!okTel || telStr.length() != 8) {
        QMessageBox::warning(this, "Erreur", "Téléphone doit comporter exactement 8 chiffres.");
        return;
    }
    if(!email_val.contains("@") || !email_val.contains(".")) {
        QMessageBox::warning(this, "Erreur", "Email invalide.");
        return;
    }
    bool okVol;
    double volume_val = volStr.toDouble(&okVol);
    if(!okVol || volume_val < 0) {
        QMessageBox::warning(this, "Erreur", "Volume invalide.");
        return;
    }

    QDate date_inscr_val = ui->dateEdit_4->date();
    QDate date_livr_val  = ui->dateEdit_2->date();

    // ✅ Utilise la classe Agriculteur
    Agriculteur a(cin_val, nom_val, prenom_val, tel_val, email_val,
                  region_val, type_olive_val, date_inscr_val,
                  volume_val, date_livr_val);

    if(a.ajouter()) {
        QMessageBox::information(this, "Succès", "Agriculteur ajouté !");
        ui->cin->clear(); ui->nom->clear(); ui->lineEdit_4->clear();
        ui->lineEdit_9->clear(); ui->lineEdit_11->clear();
        ui->lineEdit_12->clear(); ui->lineEdit_10->clear();
        refreshTableAgriculteur();
    } else {
        QMessageBox::critical(this, "Erreur", "Ajout échoué.");
    }
}

void MainWindow::on_btn_modifier_agri_clicked()
{
    QString cinStr     = ui->cin->text();
    QString nom_val    = ui->nom->text();
    QString prenom_val = ui->lineEdit_4->text();
    QString telStr     = ui->lineEdit_9->text();
    QString email_val  = ui->lineEdit_11->text();
    QString region_val = ui->lineEdit_12->text();
    QString type_olive_val = ui->comboBox_olive->currentText();
    QString volStr     = ui->lineEdit_10->text();

    bool okCin;
    int cin_val = cinStr.toInt(&okCin);
    if(!okCin || cinStr.length() != 8) {
        QMessageBox::warning(this, "Erreur", "CIN invalide.");
        return;
    }
    bool okTel;
    int tel_val = telStr.toInt(&okTel);
    bool okVol;
    double volume_val = volStr.toDouble(&okVol);

    QDate date_inscr_val = ui->dateEdit_4->date();
    QDate date_livr_val  = ui->dateEdit_2->date();

    Agriculteur a(cin_val, nom_val, prenom_val, tel_val, email_val,
                  region_val, type_olive_val, date_inscr_val,
                  volume_val, date_livr_val);

    if(a.modifier()) {
        QMessageBox::information(this, "Succès", "Agriculteur modifié !");
        ui->cin->clear(); ui->nom->clear(); ui->lineEdit_4->clear();
        ui->lineEdit_9->clear(); ui->lineEdit_11->clear();
        ui->lineEdit_12->clear(); ui->lineEdit_10->clear();
        refreshTableAgriculteur();
    } else {
        QMessageBox::critical(this, "Erreur", "Modification échouée.");
    }
}

void MainWindow::on_btn_supprimer_agri_clicked()
{
    int cin_val = ui->cin->text().toInt();
    if(cin_val <= 0) {
        QMessageBox::warning(this, "Erreur", "CIN invalide !");
        return;
    }
    if(QMessageBox::question(this, "Confirmation", "Supprimer cet agriculteur ?",
                              QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        return;

    if(Agriculteur::supprimer(cin_val)) {
        QMessageBox::information(this, "Succès", "Agriculteur supprimé !");
        ui->cin->clear(); ui->nom->clear(); ui->lineEdit_4->clear();
        ui->lineEdit_9->clear(); ui->lineEdit_11->clear();
        ui->lineEdit_12->clear(); ui->lineEdit_10->clear();
        refreshTableAgriculteur();
    } else {
        QMessageBox::critical(this, "Erreur", "Suppression échouée.");
    }
}

void MainWindow::on_btn_recherche_agri_clicked()
{
    QString val = ui->lineEdit_6->text();
    ui->TABLEAG->setModel(Atmp.rechercher(val));
}

void MainWindow::on_btn_tri_agri_clicked()
{
    static bool ascending = true;
    QString order = ascending ? "ASC" : "DESC";
    ui->TABLEAG->setModel(Atmp.trier("VOLUME_LIVRAISON " + order));
    ascending = !ascending;
}

void MainWindow::refreshTableAgriculteur()
{
    ui->TABLEAG->setModel(Atmp.afficher());
}

void MainWindow::on_TABLEAG_clicked(const QModelIndex &index)
{
    int row = index.row();
    QAbstractItemModel *model = ui->TABLEAG->model();

    ui->cin->setText(model->data(model->index(row, 0)).toString());
    ui->nom->setText(model->data(model->index(row, 1)).toString());
    ui->lineEdit_4->setText(model->data(model->index(row, 2)).toString());
    ui->lineEdit_9->setText(model->data(model->index(row, 3)).toString());
    ui->lineEdit_11->setText(model->data(model->index(row, 4)).toString());
    ui->lineEdit_12->setText(model->data(model->index(row, 5)).toString());
    ui->comboBox_olive->setCurrentText(model->data(model->index(row, 6)).toString());
    ui->dateEdit_4->setDate(model->data(model->index(row, 7)).toDate());
    ui->lineEdit_10->setText(model->data(model->index(row, 8)).toString());
    ui->dateEdit_2->setDate(model->data(model->index(row, 9)).toDate());
}

void MainWindow::on_TABLEAG_doubleClicked(const QModelIndex &index)
{
    on_TABLEAG_clicked(index);
    ui->stackedWidget->setCurrentWidget(ui->page_agriculteurs);
}

void MainWindow::on_metier_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_classement_agri);

    QSqlQuery query;
    query.prepare("SELECT CIN, NOM, PRENOM, TELEPHONE, COUNT(*), SUM(VOLUME_LIVRAISON) "
                  "FROM AGRICULTEUR "
                  "GROUP BY CIN, NOM, PRENOM, TELEPHONE "
                  "HAVING COUNT(*) > 1 "
                  "ORDER BY COUNT(*) DESC, SUM(VOLUME_LIVRAISON) DESC");

    if (query.exec()) {
        ui->tableWidget_3->setRowCount(0);
        ui->tableWidget_3->setColumnCount(6);
        ui->tableWidget_3->setHorizontalHeaderLabels(
            {"CIN", "Nom", "Prénom", "Téléphone", "Nb Livraisons", "Volume Total"});
        int row = 0;
        while (query.next()) {
            ui->tableWidget_3->insertRow(row);
            for(int col = 0; col < 5; col++)
                ui->tableWidget_3->setItem(row, col,
                                           new QTableWidgetItem(query.value(col).toString()));
            ui->tableWidget_3->setItem(row, 5, new QTableWidgetItem(
                                                   QString::number(query.value(5).toDouble(), 'f', 2) + " L"));
            row++;
        }
        ui->tableWidget_3->resizeColumnsToContents();
    } else {
        qDebug() << "Erreur métier:" << query.lastError().text();
    }
}


// ----------------------------------------------------------------
// FONCTIONS UTILITAIRES  (garder en haut du bloc production)
// ----------------------------------------------------------------

static QString variantAsPlainIntString(const QVariant &v)
{
    if (!v.isValid() || v.isNull()) return QString();
    bool ok = false;
    const qint64 nLL = v.toLongLong(&ok);
    if (ok) return QString::number(nLL);
    const double d = v.toDouble(&ok);
    if (ok) return QString::number(static_cast<qint64>(std::llround(d)));
    return v.toString();
}

static bool parsePositiveIntId(const QString &text, int *out)
{
    bool ok = false;
    qint64 n = text.trimmed().toLongLong(&ok);
    if (!ok || n <= 0) return false;
    *out = static_cast<int>(n);
    return true;
}

// ================================================================
// ÉTAPE 1 — rafraichirGrilleProduction()
// ✅ CORRIGÉ :
//   - SELECT : ID_AGRI → CIN, suppression TEMPERATURE_MOYENNE
//   - setColumnCount : 11 → 10
//   - En-têtes : suppression "Temp.", renumérotation
//   - Boucle remplissage : 11 colonnes → 10 colonnes
// ================================================================
void MainWindow::rafraichirGrilleProduction(const QString &needle)
{
    QSqlQuery query;
    // ✅ CORRIGÉ : CIN à la place de ID_AGRI, pas de TEMPERATURE_MOYENNE
    query.exec(
        "SELECT ID_OPERATION, DATE_PRODUCTION, CIN, ID_EMP, "
        "QUANTITE_OLIVES, QUANTITE_HUILE, RENDEMENT, TYPE_HUILE, "
        "DUREE_PRESSAGE, OBSERVATION "
        "FROM PRODUCTION ORDER BY DATE_PRODUCTION DESC"
        );

    ui->tableWidget_prod->setRowCount(0);
    ui->tableWidget_prod->setColumnCount(10); // ✅ 10 colonnes (pas 11)
    ui->tableWidget_prod->setHorizontalHeaderLabels({
        "ID op.", "Date", "CIN Agri.", "ID Emp.",
        "Qté olives", "Qté huile", "Rendement",
        "Type huile", "Durée", "Observation"
        // ✅ "Temp." supprimée
    });

    int row = 0;
    while (query.next()) {
        // Filtre recherche
        if (!needle.isEmpty()) {
            bool found = false;
            for (int c = 0; c < 10; c++) { // ✅ 10 colonnes
                if (query.value(c).toString().contains(needle, Qt::CaseInsensitive)) {
                    found = true; break;
                }
            }
            if (!found) continue;
        }

        ui->tableWidget_prod->insertRow(row);

        // col 0 — ID_OPERATION
        ui->tableWidget_prod->setItem(row, 0,
                                      new QTableWidgetItem(variantAsPlainIntString(query.value(0))));

        // col 1 — DATE_PRODUCTION
        QDate d = query.value(1).toDate();
        ui->tableWidget_prod->setItem(row, 1,
                                      new QTableWidgetItem(d.isValid() ? d.toString("dd/MM/yyyy")
                                                                       : query.value(1).toString()));

        // col 2 — CIN (agriculteur)
        ui->tableWidget_prod->setItem(row, 2,
                                      new QTableWidgetItem(variantAsPlainIntString(query.value(2))));

        // col 3 — ID_EMP
        ui->tableWidget_prod->setItem(row, 3,
                                      new QTableWidgetItem(variantAsPlainIntString(query.value(3))));

        // col 4 — QUANTITE_OLIVES
        ui->tableWidget_prod->setItem(row, 4,
                                      new QTableWidgetItem(query.value(4).toString()));

        // col 5 — QUANTITE_HUILE
        ui->tableWidget_prod->setItem(row, 5,
                                      new QTableWidgetItem(query.value(5).toString()));

        // col 6 — RENDEMENT
        ui->tableWidget_prod->setItem(row, 6,
                                      new QTableWidgetItem(query.value(6).toString()));

        // col 7 — TYPE_HUILE
        ui->tableWidget_prod->setItem(row, 7,
                                      new QTableWidgetItem(query.value(7).toString()));

        // col 8 — DUREE_PRESSAGE  (✅ était index 9 avant, maintenant 8)
        ui->tableWidget_prod->setItem(row, 8,
                                      new QTableWidgetItem(variantAsPlainIntString(query.value(8))));

        // col 9 — OBSERVATION     (✅ était index 10 avant, maintenant 9)
        ui->tableWidget_prod->setItem(row, 9,
                                      new QTableWidgetItem(query.value(9).toString()));

        row++;
    }
    ui->tableWidget_prod->resizeColumnsToContents();
}

// ================================================================
// ÉTAPE 2 — on_afficher_prod_clicked()
// ✅ OK — aucun changement
// ================================================================
void MainWindow::on_afficher_prod_clicked()
{
    rafraichirGrilleProduction("");
}

// ================================================================
// ÉTAPE 3 — on_ajouter_prod_clicked()
// ✅ CORRIGÉ :
//   - INSERT : ID_AGRI → CIN, bindValue corrigé
//   - Validation du rendement ajoutée
// ================================================================
void MainWindow::on_ajouter_prod_clicked()
{
    QString idOpStr   = ui->ID_Operation->text().trimmed();
    QString idAgriStr = ui->ID_Agriculteur->text().trimmed();


    QString idEmpStr = ui->ID_Emp->text().trimmed(); // ← ajouter

    int idOp = 0, idAgri = 0, idEmp = 0;
    if (!parsePositiveIntId(idOpStr, &idOp)) {
        QMessageBox::warning(this, "Erreur", "ID opération invalide !"); return; }
    if (!parsePositiveIntId(idAgriStr, &idAgri)) {
        QMessageBox::warning(this, "Erreur", "ID agriculteur (CIN) invalide !"); return; }
    if (!parsePositiveIntId(idEmpStr, &idEmp)) {
        QMessageBox::warning(this, "Erreur", "ID employé invalide !"); return; } // ← ajouter
    bool ok;
    double qteOlives = ui->Quantite_olives->text().toDouble(&ok);
    if (!ok || qteOlives <= 0) {
        QMessageBox::warning(this, "Erreur", "Quantité olives invalide !"); return; }

    double qteHuile = ui->Quantite_Huile->text().toDouble(&ok);
    if (!ok || qteHuile < 0) {
        QMessageBox::warning(this, "Erreur", "Quantité huile invalide !"); return; }

    // ✅ Validation du rendement ajoutée
    double rendem = ui->Rendement->text().toDouble(&ok);
    if (!ok) rendem = 0.0;

    QString typeH = ui->Type_Huile->currentText().trimmed();
    QDate   date  = ui->Date->date();
    QString obs   = ui->Observation->text().trimmed();
    int duree = 0;
    parsePositiveIntId(ui->Duree_Pressage->text().trimmed(), &duree);

    QSqlQuery query;
    // ✅ CORRIGÉ : CIN à la place de ID_AGRI
    query.prepare(
        "INSERT INTO PRODUCTION "
        "(ID_OPERATION, DATE_PRODUCTION, CIN, ID_EMP, "
        "QUANTITE_OLIVES, QUANTITE_HUILE, RENDEMENT, TYPE_HUILE, "
        "DUREE_PRESSAGE, OBSERVATION) "
        "VALUES (:id_op, :date, :cin, :id_emp, :qto, :qth, "
        ":rend, :type, :duree, :obs)"
        );
    query.bindValue(":id_op",  idOp);
    query.bindValue(":date",   date);
    query.bindValue(":cin",    idAgri);  // ✅ CIN (pas :id_agri)
    query.bindValue(":id_emp", idEmp);
    query.bindValue(":qto",    qteOlives);
    query.bindValue(":qth",    qteHuile);
    query.bindValue(":rend",   rendem);
    query.bindValue(":type",   typeH);
    query.bindValue(":duree",  duree);
    query.bindValue(":obs",    obs);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Production ajoutée !");
        viderChampsProduction();
        rafraichirGrilleProduction("");
    } else {
        QString err = query.lastError().text();
        if (err.contains("ORA-00001"))
            QMessageBox::warning(this, "Erreur", "Cet ID opération existe déjà !");
        else
            QMessageBox::critical(this, "Erreur", "Erreur SQL : " + err);
    }
}

// ================================================================
// ÉTAPE 4 — on_modifier_prod_clicked()
// ✅ CORRIGÉ :
//   - UPDATE : ID_AGRI → CIN
//   - bindValue ":id_agri" → ":cin"
// ================================================================
void MainWindow::on_modifier_prod_clicked()
{
    int idOp = 0;
    if (!parsePositiveIntId(ui->ID_Operation->text(), &idOp)) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez une production dans le tableau !"); return; }
    int idAgri = 0, idEmp = 0;
    parsePositiveIntId(ui->ID_Agriculteur->text(), &idAgri);
    parsePositiveIntId(ui->ID_Emp->text(), &idEmp); // ← ajouter

    bool ok;
    double qteOlives = ui->Quantite_olives->text().toDouble(&ok);
    double qteHuile  = ui->Quantite_Huile->text().toDouble();
    double rendem    = ui->Rendement->text().toDouble();
    QString typeH    = ui->Type_Huile->currentText().trimmed();
    QDate   date     = ui->Date->date();
    QString obs      = ui->Observation->text().trimmed();
    int duree = 0;
    parsePositiveIntId(ui->Duree_Pressage->text(), &duree);

    QSqlQuery query;
    // ✅ CORRIGÉ : CIN à la place de ID_AGRI
    query.prepare(
        "UPDATE PRODUCTION SET "
        "DATE_PRODUCTION=:date, CIN=:cin, ID_EMP=:id_emp, "
        "QUANTITE_OLIVES=:qto, QUANTITE_HUILE=:qth, "
        "RENDEMENT=:rend, TYPE_HUILE=:type, "
        "DUREE_PRESSAGE=:duree, OBSERVATION=:obs "
        "WHERE ID_OPERATION=:id_op"
        );
    query.bindValue(":id_op",  idOp);
    query.bindValue(":date",   date);
    query.bindValue(":cin",    idAgri);   // ✅ :cin (pas :id_agri)
    query.bindValue(":id_emp", idEmp);
    query.bindValue(":qto",    qteOlives);
    query.bindValue(":qth",    qteHuile);
    query.bindValue(":rend",   rendem);
    query.bindValue(":type",   typeH);
    query.bindValue(":duree",  duree);
    query.bindValue(":obs",    obs);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Production modifiée !");
        viderChampsProduction();
        rafraichirGrilleProduction("");
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur SQL : " + query.lastError().text());
    }
}

// ================================================================
// ÉTAPE 5 — on_suprimer_prod_clicked()
// ✅ OK — aucun changement nécessaire
// ================================================================
void MainWindow::on_suprimer_prod_clicked()
{
    int idOp = 0;
    if (!parsePositiveIntId(ui->ID_Operation->text(), &idOp)) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez une production dans le tableau !"); return; }

    if (QMessageBox::question(this, "Confirmation",
                              "Supprimer la production n° " + QString::number(idOp) + " ?",
                              QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        return;

    QSqlQuery query;
    query.prepare("DELETE FROM PRODUCTION WHERE ID_OPERATION=:id_op");
    query.bindValue(":id_op", idOp);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Production supprimée !");
        viderChampsProduction();
        rafraichirGrilleProduction("");
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur SQL : " + query.lastError().text());
    }
}

// ================================================================
// ÉTAPE 6 — on_tableWidget_prod_cellClicked()
// ✅ CORRIGÉ :
//   - Index 8 = DUREE_PRESSAGE  (avant c'était 9 à cause de TEMPERATURE)
//   - Index 9 = OBSERVATION     (avant c'était 10)
// ================================================================
void MainWindow::on_tableWidget_prod_cellClicked(int row, int)
{
    ui->ID_Operation->setText(
        ui->tableWidget_prod->item(row, 0)->text());
    ui->ID_Emp->setText(
        ui->tableWidget_prod->item(row, 3)->text()); // ← ajouter
    ui->Date->setDate(QDate::fromString(
        ui->tableWidget_prod->item(row, 1)->text(), "dd/MM/yyyy"));

    ui->ID_Agriculteur->setText(
        ui->tableWidget_prod->item(row, 2)->text());  // CIN

    ui->Quantite_olives->setText(
        ui->tableWidget_prod->item(row, 4)->text());

    ui->Quantite_Huile->setText(
        ui->tableWidget_prod->item(row, 5)->text());

    ui->Rendement->setText(
        ui->tableWidget_prod->item(row, 6)->text());

    ui->Type_Huile->setCurrentText(
        ui->tableWidget_prod->item(row, 7)->text());

    // ✅ index 8 (pas 9) car TEMPERATURE_MOYENNE supprimée
    ui->Duree_Pressage->setText(
        ui->tableWidget_prod->item(row, 8)->text());

    // ✅ index 9 (pas 10)
    ui->Observation->setText(
        ui->tableWidget_prod->item(row, 9)->text());
}

// ================================================================
// ÉTAPE 7 — on_rendement_clicked()
// ✅ OK — aucun changement nécessaire
// ================================================================
void MainWindow::on_rendement_clicked()
{
    bool okO, okH;
    double qO = ui->Quantite_olives->text().toDouble(&okO);
    double qH = ui->Quantite_Huile->text().toDouble(&okH);

    if (!okO || !okH || qO <= 0) {
        QMessageBox::warning(this, "Erreur",
                             "Entrez d'abord la quantité d'olives et d'huile.");
        return;
    }
    double rend = (qH / qO) * 100.0;
    ui->Rendement->setText(QString::number(rend, 'f', 2));
}

// ================================================================
// ÉTAPE 8 — viderChampsProduction()
// ✅ OK — aucun changement nécessaire
// ================================================================
void MainWindow::viderChampsProduction()
{
    ui->ID_Operation->clear();
    ui->Date->setDate(QDate::currentDate());
    ui->ID_Agriculteur->clear();
    ui->ID_Emp->clear();
    ui->Quantite_olives->clear();
    ui->Quantite_Huile->clear();
    ui->Rendement->clear();
    ui->Type_Huile->setCurrentIndex(0);
    ui->Duree_Pressage->clear();
    ui->Observation->clear();
}

// ================================================================
// ÉTAPE 9 — refreshTableProduction()
// ✅ Fonction utilitaire appelée depuis d'autres modules si besoin
// ================================================================
void MainWindow::refreshTableProduction()
{
    rafraichirGrilleProduction("");
}
