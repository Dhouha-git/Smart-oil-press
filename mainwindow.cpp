#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->login);

    ui->TABLEAG->setModel(afficherAgriculteurs());


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


void MainWindow::on_btn_modifier_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_modifier_employe_2);
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
    // Tri par Volume Livré
    static bool ascending = true;
    QString order = ascending ? "ASC" : "DESC";
    ui->TABLEAG->setModel(trierAgriculteurs("VOLUME_LIVRAISON " + order));
    ascending = !ascending;

    // Show the table
    ui->stackedWidget->setCurrentWidget(ui->page_analyse_agri);
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

bool MainWindow::ajouterAgriculteur(int cin, QString nom, QString prenom, int telephone, QString email, QString region, QString type_olive, QDate date_inscription, double volume_livraison, QDate date_livraison)
{
    QSqlQuery query;
    query.prepare("INSERT INTO AGRICULTEUR (CIN, NOM, PRENOM, TELEPHONE, EMAIL, REGION, TYPE_OLIVE, DATE_INSCRIPTION, VOLUME_LIVRAISON, DATE_LIVRAISON) "
                  "VALUES (:cin, :nom, :prenom, :telephone, :email, :region, :type_olive, :date_inscription, :volume_livraison, :date_livraison)");
    query.bindValue(":cin", cin);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":telephone", telephone);
    query.bindValue(":email", email);
    query.bindValue(":region", region);
    query.bindValue(":type_olive", type_olive);
    query.bindValue(":date_inscription", date_inscription);
    query.bindValue(":volume_livraison", volume_livraison);
    query.bindValue(":date_livraison", date_livraison);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Erreur d'ajout d'agriculteur:" << query.lastError().text();
        return false;
    }
}

bool MainWindow::modifierAgriculteur(int cin, QString nom, QString prenom, int telephone, QString email, QString region, QString type_olive, QDate date_inscription, double volume_livraison, QDate date_livraison)
{
    QSqlQuery query;
    query.prepare("UPDATE AGRICULTEUR SET NOM = :nom, PRENOM = :prenom, TELEPHONE = :telephone, EMAIL = :email, REGION = :region, TYPE_OLIVE = :type_olive, DATE_INSCRIPTION = :date_inscription, VOLUME_LIVRAISON = :volume_livraison, DATE_LIVRAISON = :date_livraison "
                  "WHERE CIN = :cin");
    query.bindValue(":cin", cin);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":telephone", telephone);
    query.bindValue(":email", email);
    query.bindValue(":region", region);
    query.bindValue(":type_olive", type_olive);
    query.bindValue(":date_inscription", date_inscription);
    query.bindValue(":volume_livraison", volume_livraison);
    query.bindValue(":date_livraison", date_livraison);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Erreur de modification d'agriculteur:" << query.lastError().text();
        return false;
    }
}

bool MainWindow::supprimerAgriculteur(int cin)
{
    QSqlQuery query;
    query.prepare("DELETE FROM AGRICULTEUR WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Erreur de suppression d'agriculteur:" << query.lastError().text();
        return false;
    }
}

QSqlQueryModel* MainWindow::afficherAgriculteurs()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT TO_CHAR(CIN,'99999999999'), NOM, PRENOM, TO_CHAR(TELEPHONE), EMAIL, REGION, TYPE_OLIVE, DATE_INSCRIPTION, VOLUME_LIVRAISON, DATE_LIVRAISON FROM AGRICULTEUR");
    
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Région"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Type d'Olive"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date Inscription"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Volume Livraison"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Date Livraison"));

    return model;
}

QSqlQueryModel* MainWindow::rechercherAgriculteur(QString queryStr)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT CIN, NOM, PRENOM, TELEPHONE, EMAIL, REGION, TYPE_OLIVE, DATE_INSCRIPTION, VOLUME_LIVRAISON, DATE_LIVRAISON FROM AGRICULTEUR WHERE NOM LIKE :q OR PRENOM LIKE :q OR REGION LIKE :q OR CIN LIKE :q");
    query.bindValue(":q", "%" + queryStr + "%");
    
    if (query.exec()) {
        model->setQuery(std::move(query));
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Téléphone"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Email"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Région"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("Type d'Olive"));
        model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date Inscription"));
        model->setHeaderData(8, Qt::Horizontal, QObject::tr("Volume Livraison"));
        model->setHeaderData(9, Qt::Horizontal, QObject::tr("Date Livraison"));
    } else {
        qDebug() << "Erreur Recherche Agriculteur:" << query.lastError().text();
    }
    return model;
}

QSqlQueryModel* MainWindow::trierAgriculteurs(QString critere)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT CIN, NOM, PRENOM, TELEPHONE, EMAIL, REGION, TYPE_OLIVE, DATE_INSCRIPTION, VOLUME_LIVRAISON, DATE_LIVRAISON FROM AGRICULTEUR ORDER BY " + critere);
    
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Région"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Type d'Olive"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date Inscription"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Volume Livraison"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Date Livraison"));

    return model;
}

void MainWindow::on_btn_ajouter_2_clicked()
{
    QString cinStr = ui->cin->text();
    QString nom_val = ui->nom->text();
    QString prenom_val = ui->lineEdit_4->text();
    QString telStr = ui->lineEdit_9->text();
    QString email_val = ui->lineEdit_11->text();
    QString region_val = ui->lineEdit_12->text();
    QString type_olive_val = ui->comboBox_olive->currentText();
    QString volStr = ui->lineEdit_10->text();

    if(cinStr.isEmpty() || nom_val.isEmpty() || prenom_val.isEmpty() || telStr.isEmpty() || email_val.isEmpty() || region_val.isEmpty() || volStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires.");
        return;
    }
    
    bool okCin;
    int cin_val = cinStr.toInt(&okCin);
    if(!okCin || cinStr.length() != 8) {
        QMessageBox::warning(this, "Erreur", "Le CIN doit comporter exactement 8 chiffres.");
        return;
    }

    bool okTel;
    int tel_val = telStr.toInt(&okTel);
    if(!okTel || telStr.length() != 8) {
        QMessageBox::warning(this, "Erreur", "Le Téléphone doit comporter exactement 8 chiffres.");
        return;
    }

    if(!email_val.contains("@") || !email_val.contains(".")) {
        QMessageBox::warning(this, "Erreur", "Format d'E-mail invalide. Il doit contenir '@' et '.'.");
        return;
    }
    
    bool okVol;
    double volume_val = volStr.toDouble(&okVol);
    if(!okVol || volume_val < 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un Volume de livraison valide.");
        return;
    }

    QDate date_inscr_val = ui->dateEdit_4->date();
    QDate date_livr_val = ui->dateEdit_2->date();

    bool success = ajouterAgriculteur(cin_val, nom_val, prenom_val, tel_val, email_val, region_val, type_olive_val, date_inscr_val, volume_val, date_livr_val);
    
    if (success) {
        qDebug() << "Agriculteur added successfully from UI.";
        // Clear inputs after successful addition
        ui->cin->clear();
        ui->nom->clear();
        ui->lineEdit_4->clear();
        ui->lineEdit_9->clear();
        ui->lineEdit_11->clear();
        ui->lineEdit_12->clear();
        ui->lineEdit_10->clear();

        // Refresh table view
        ui->TABLEAG->setModel(afficherAgriculteurs());
    } else {
        qDebug() << "Failed to add Agriculteur from UI.";
    }
}

void MainWindow::on_btn_modifier_agri_clicked()
{
    QString cinStr = ui->cin->text();
    QString nom_val = ui->nom->text();
    QString prenom_val = ui->lineEdit_4->text();
    QString telStr = ui->lineEdit_9->text();
    QString email_val = ui->lineEdit_11->text();
    QString region_val = ui->lineEdit_12->text();
    QString type_olive_val = ui->comboBox_olive->currentText();
    QString volStr = ui->lineEdit_10->text();

    if(cinStr.isEmpty() || nom_val.isEmpty() || prenom_val.isEmpty() || telStr.isEmpty() || email_val.isEmpty() || region_val.isEmpty() || volStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires.");
        return;
    }
    
    bool okCin;
    int cin_val = cinStr.toInt(&okCin);
    if(!okCin || cinStr.length() != 8) {
        QMessageBox::warning(this, "Erreur", "Le CIN doit comporter exactement 8 chiffres.");
        return;
    }

    bool okTel;
    int tel_val = telStr.toInt(&okTel);
    if(!okTel || telStr.length() != 8) {
        QMessageBox::warning(this, "Erreur", "Le Téléphone doit comporter exactement 8 chiffres.");
        return;
    }

    if(!email_val.contains("@") || !email_val.contains(".")) {
        QMessageBox::warning(this, "Erreur", "Format d'E-mail invalide. Il doit contenir '@' et '.'.");
        return;
    }
    
    bool okVol;
    double volume_val = volStr.toDouble(&okVol);
    if(!okVol || volume_val < 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un Volume de livraison valide.");
        return;
    }

    QDate date_inscr_val = ui->dateEdit_4->date();
    QDate date_livr_val = ui->dateEdit_2->date();

    bool success = modifierAgriculteur(cin_val, nom_val, prenom_val, tel_val, email_val, region_val, type_olive_val, date_inscr_val, volume_val, date_livr_val);
    
    if (success) {
        qDebug() << "Agriculteur modifié avec succès.";
        ui->TABLEAG->setModel(afficherAgriculteurs());
        
        ui->cin->clear();
        ui->nom->clear();
        ui->lineEdit_4->clear();
        ui->lineEdit_9->clear();
        ui->lineEdit_11->clear();
        ui->lineEdit_12->clear();
        ui->lineEdit_10->clear();
    }
}

void MainWindow::on_btn_supprimer_agri_clicked()
{
    int cin_val = ui->cin->text().toInt();
    bool success = supprimerAgriculteur(cin_val);
    
    if (success) {
        qDebug() << "Agriculteur supprimé avec succès.";
        ui->TABLEAG->setModel(afficherAgriculteurs());
        
        ui->cin->clear();
        ui->nom->clear();
        ui->lineEdit_4->clear();
        ui->lineEdit_9->clear();
        ui->lineEdit_11->clear();
        ui->lineEdit_12->clear();
        ui->lineEdit_10->clear();
    }
}

void MainWindow::on_btn_recherche_agri_clicked()
{
    QString requete = ui->lineEdit_recherche_agri->text();
    ui->TABLEAG->setModel(rechercherAgriculteur(requete));
}

void MainWindow::on_btn_tri_agri_clicked()
{
    static bool ascending = true;
    QString order = ascending ? "ASC" : "DESC";
    ui->TABLEAG->setModel(trierAgriculteurs("VOLUME_LIVRAISON " + order));
    ascending = !ascending;
}

void MainWindow::on_TABLEAG_clicked(const QModelIndex &index)
{
    int row = index.row();
    qDebug() << "TableView single-clicked on row:" << row;
    QAbstractItemModel *model = ui->TABLEAG->model();
    
    QString cinStr = model->data(model->index(row, 0)).toString();
    QString nomStr = model->data(model->index(row, 1)).toString();
    QString prenomStr = model->data(model->index(row, 2)).toString();
    QString telStr = model->data(model->index(row, 3)).toString();
    QString emailStr = model->data(model->index(row, 4)).toString();
    QString regionStr = model->data(model->index(row, 5)).toString();
    QString typeOliveStr = model->data(model->index(row, 6)).toString();
    QDate dateInsc = model->data(model->index(row, 7)).toDate();
    QString volStr = model->data(model->index(row, 8)).toString();
    QDate dateLivr = model->data(model->index(row, 9)).toDate();
    
    qDebug() << "Extracted -> CIN:" << cinStr << "NOM:" << nomStr << "PRENOM:" << prenomStr;
    
    ui->cin->setText(cinStr);
    ui->nom->setText(nomStr);
    ui->lineEdit_4->setText(prenomStr); 
    ui->lineEdit_9->setText(telStr); 
    ui->lineEdit_11->setText(emailStr); 
    ui->lineEdit_12->setText(regionStr); 
    ui->comboBox_olive->setCurrentText(typeOliveStr);
    ui->dateEdit_4->setDate(dateInsc);
    ui->lineEdit_10->setText(volStr); 
    ui->dateEdit_2->setDate(dateLivr);
}

void MainWindow::on_TABLEAG_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    qDebug() << "TableView double-clicked on row:" << row;
    QAbstractItemModel *model = ui->TABLEAG->model();
    
    QString cinStr = model->data(model->index(row, 0)).toString();
    QString nomStr = model->data(model->index(row, 1)).toString();
    QString prenomStr = model->data(model->index(row, 2)).toString();
    QString telStr = model->data(model->index(row, 3)).toString();
    QString emailStr = model->data(model->index(row, 4)).toString();
    QString regionStr = model->data(model->index(row, 5)).toString();
    QString typeOliveStr = model->data(model->index(row, 6)).toString();
    QDate dateInsc = model->data(model->index(row, 7)).toDate();
    QString volStr = model->data(model->index(row, 8)).toString();
    QDate dateLivr = model->data(model->index(row, 9)).toDate();
    
    qDebug() << "DB Extracted Double-Click -> CIN:" << cinStr << "NOM:" << nomStr;
    
    ui->cin->setText(cinStr);
    ui->nom->setText(nomStr);
    ui->lineEdit_4->setText(prenomStr); 
    ui->lineEdit_9->setText(telStr); 
    ui->lineEdit_11->setText(emailStr); 
    ui->lineEdit_12->setText(regionStr); 
    ui->comboBox_olive->setCurrentText(typeOliveStr);
    ui->dateEdit_4->setDate(dateInsc);
    ui->lineEdit_10->setText(volStr); 
    ui->dateEdit_2->setDate(dateLivr);
    
    // Switch the view to the form page automatically so the user can see and modify the data
    ui->stackedWidget->setCurrentWidget(ui->page_agriculteurs);
}

void MainWindow::on_metier_clicked()
{
    // Naviguer vers la page
    ui->stackedWidget->setCurrentWidget(ui->page_classement_agri);

    // Métier avancé: Trouver les agriculteurs avec des doublons (plusieurs livraisons)
    QSqlQuery query;
    query.prepare("SELECT CIN, NOM, PRENOM, TELEPHONE, COUNT(*), SUM(VOLUME_LIVRAISON) "
                  "FROM AGRICULTEUR "
                  "GROUP BY CIN, NOM, PRENOM, TELEPHONE "
                  "HAVING COUNT(*) > 1 "
                  "ORDER BY COUNT(*) DESC, SUM(VOLUME_LIVRAISON) DESC");

    if (query.exec()) {
        ui->tableWidget_3->setRowCount(0); // clear existing rows
        ui->tableWidget_3->setColumnCount(6);
        ui->tableWidget_3->setHorizontalHeaderLabels({"CIN", "Nom", "Prénom", "Téléphone", "Nb Livraisons", "Volume Total"});

        int row = 0;
        while (query.next()) {
            ui->tableWidget_3->insertRow(row);
            ui->tableWidget_3->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
            ui->tableWidget_3->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
            ui->tableWidget_3->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
            ui->tableWidget_3->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
            ui->tableWidget_3->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
            
            // Format volume nicely
            QString volStr = QString::number(query.value(5).toDouble(), 'f', 2) + " L";
            ui->tableWidget_3->setItem(row, 5, new QTableWidgetItem(volStr));
            
            row++;
        }
        
        // Resize columns to fit contents
        ui->tableWidget_3->resizeColumnsToContents();
    } else {
        qDebug() << "Erreur métier avancé :" << query.lastError().text();
    }
}

