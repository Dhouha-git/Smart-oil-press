#include "client.h"
#include "ui_client.h"
#include "connection.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTableWidgetItem>
#include <QDebug>
#include <QSqlRecord>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <stdexcept>
#include <QPdfWriter>
#include <QPainter>
#include <QPrinter>  // 🔹 obligatoire
#include <QFileDialog>
#include <QPrintPreviewDialog>
#include <qdesktopservices>
#include <QStandardPaths>
#include <QTextEdit>
#include <QScrollBar>
#include <QTextBrowser>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QStandardPaths>
#include <QBuffer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QInputDialog>
#include <QJsonArray>
#include <QPixmap>
#include <QListWidgetItem>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>
#include <QLayoutItem>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QMediaRecorder>
#include <QMediaCaptureSession>
#include <QAudioInput>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMediaCaptureSession>
#include <QAudioDevice>
#include <QMediaDevices>
#include <QVariantMap>


// ================= CONSTRUCTEUR UI =================
client::client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::client)
    , model(nullptr)
{
    ui->setupUi(this);

    if (!connection::getInstance()->createConnect()) {
        qDebug() << "Erreur connexion";
    } else {
        qDebug() << "Connexion OK";
    }

    // Remplir la table au démarrage
    remplirTableau();
    // Dans le constructeur client::client(QWidget *parent)
    connect(ui->ajouter, &QPushButton::clicked, this, &client::on_ajouter_clicked);
    connect(ui->modifier, &QPushButton::clicked, this, &client::on_modifier_clicked);
connect(ui->supprimer, &QPushButton::clicked, this, &client::on_supprimer_clicked);
connect(ui->recherche2_2, &QPushButton::clicked, this, &client::on_recherche2_2_clicked);
connect(ui->executer, &QPushButton::clicked,
        this, &client::afficherCourbeDansTable2);

// Style boutons

ui->recherche2->clear();
ui->particulier_2->setChecked(false);
ui->profisionnel_2->setChecked(false);

on_recherche2_2_clicked();
on_appliquer2_clicked();  // table "clients fidèles"
on_appliquer3_clicked();  // table "meilleurs clients"
QPixmap pix(":/images/logo.png");

if (pix.isNull()) {
    qDebug() << "Image introuvable !";
}

ui->logo_label->setPixmap(pix);

}


// ================= CONSTRUCTEUR AVEC PARAM =================
client::client(int id,
               QString nom,
               QString prenom,
               QString telephone,
               QString email,
               QString adresse,
               QDate date,
               QString type,
               QString huile,
               QString emballage,
               QString total,
               QString point)
{
    this->id = id;
    this->nom = nom;
    this->prenom = prenom;
    this->telephone = telephone;
    this->email = email;
    this->adresse = adresse;
    this->date_inscription = date;
    this->type_client = type;
    this->type_huile_prefere = huile;
    this->emballage_prefere = emballage;
    this->total_achat = total;
    this->points_fidelite = point;
    model = nullptr;
}

// ================= AJOUTER =================

bool client::ajouter()
{
    QSqlQuery query;

    query.prepare("INSERT INTO SMART.CLIENT "
                  "(ID, NOM, PRENOM, TELEPHONE, EMAIL, ADRESSE, DATE_INSCRIPTION, "
                  "TYPE_CLIENT, TYPE_HUILE_PREFERE, EMBALLAGE_PREFERE, TOTAL_ACHAT, POINTS_FIDELITE) "
                  "VALUES (:id, :nom, :prenom, :telephone, :email, :adresse, :date, "
                  ":type_client, :type_huile, :emballage, :total, :points)");

    // 🔴 IMPORTANT : respecter les contraintes

    query.bindValue(":id", id);  // ⚠️ doit être unique

    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":telephone", telephone);

    query.bindValue(":email", email.isEmpty() ? QVariant(QVariant::String) : email);
    query.bindValue(":adresse", adresse.isEmpty() ? QVariant(QVariant::String) : adresse);

    query.bindValue(":date", date_inscription);

    // 🔴 CHECK CONSTRAINT → valeurs exactes
    query.bindValue(":type_client", type_client);          // "particulier" ou "professionnel"
    query.bindValue(":type_huile", type_huile_prefere);    // "vierge" ou "bio"
    query.bindValue(":emballage", emballage_prefere);      // "1L","3L","5L","autre"

    query.bindValue(":total", total_achat.isEmpty() ? 0 : total_achat.toDouble());
    query.bindValue(":points", points_fidelite.isEmpty() ? 0 : points_fidelite.toInt());

    // 🔴 EXECUTION + DEBUG
    if (!query.exec()) {
        qDebug() << "ERREUR AJOUT :" << query.lastError().text();
        return false;
    }

    qDebug() << "AJOUT AVEC SUCCES";
    return true;
}

// ================= MODIFIER =================
bool client::modifier()
{
    QSqlQuery query;

    query.prepare("UPDATE SMART.CLIENT SET "
                  "NOM=:nom, PRENOM=:prenom, TELEPHONE=:telephone, EMAIL=:email, "
                  "ADRESSE=:adresse, DATE_INSCRIPTION=:date_inscription, TYPE_CLIENT=:type_client, "
                  "TYPE_HUILE_PREFERE=:type_huile_prefere, EMBALLAGE_PREFERE=:emballage_prefere, "
                  "TOTAL_ACHAT=:total_achat, POINTS_FIDELITE=:points_fidelite "
                  "WHERE ID=:id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":telephone", telephone);
    query.bindValue(":email", email);
    query.bindValue(":adresse", adresse);
    query.bindValue(":date_inscription", date_inscription);
    query.bindValue(":type_client", type_client);
    query.bindValue(":type_huile_prefere", type_huile_prefere);
    query.bindValue(":emballage_prefere", emballage_prefere);
    query.bindValue(":total_achat", total_achat);
    query.bindValue(":points_fidelite", points_fidelite);

    if (!query.exec()) {
        qDebug() << "Erreur modification:" << query.lastError().text();
        return false;
    }

    remplirTableau();
    return true;
    audioInput = new QAudioInput(this);
    recorder = new QMediaRecorder(this);

    captureSession.setAudioInput(audioInput);
    captureSession.setRecorder(recorder);
}

// ================= SUPPRIMER =================
bool client::supprimer(int id)
{
    QSqlQuery query;

    // 🔴 Ajouter le schéma SMART
    query.prepare("DELETE FROM SMART.CLIENT WHERE ID=:id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur suppression:" << query.lastError().text();
        return false;
    }

    qDebug() << "Suppression réussie pour ID =" << id;
    return true;
}
// ================= REMPLIR TABLE =================
void client::remplirTableau()
{
    try {
        // 🔹 Vérifier que le modèle existe
        if (!model)
            model = new QSqlTableModel(this);

        model->setTable("CLIENT");

        // 🔹 Charger les données depuis la base
        if (!model->select()) {
            throw std::runtime_error("Erreur lors de la récupération des clients : " + model->lastError().text().toStdString());
        }

        // 🔹 Vérifier qu’il y a des lignes
        int rowCount = model->rowCount();
        if (rowCount == 0) {
            ui->table->setRowCount(0);
            ui->table->clearContents();
            return;
        }

        // 🔹 Définir les en-têtes de QTableWidget
        ui->table->setColumnCount(model->columnCount());
        QStringList headers;
        headers << "ID" << "Nom" << "Prénom" << "Téléphone" << "Email"
                << "Adresse" << "Date inscription" << "Type client"
                << "Huile préférée" << "Emballage" << "Total achat" << "Points fidélité";
        ui->table->setHorizontalHeaderLabels(headers);

        // 🔹 Remplir les lignes
        ui->table->setRowCount(rowCount);
        for (int i = 0; i < rowCount; ++i) {
            for (int j = 0; j < model->columnCount(); ++j) {
                QString value = model->record(i).value(j).toString();

                // 🔹 Contrôle basique : si valeur vide pour un champ obligatoire, mettre "N/A"
                if (value.isEmpty() && (j == 0 || j == 1 || j == 2 || j == 3)) {
                    value = "N/A";  // ID, Nom, Prénom, Téléphone
                }

                QTableWidgetItem *item = new QTableWidgetItem(value);
                ui->table->setItem(i, j, item);
            }
        }

        ui->table->resizeColumnsToContents();

    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Erreur", e.what());
    }
}


// ================= DESTRUCTEUR =================
client::~client()
{
    delete ui;
}

// ================= NAVIGATION =================
void client::on_analyse_clicked()
{
    ui->stacked->setCurrentWidget(ui->page_2);
}

void client::on_chatbot_clicked()
{
    ui->stacked->setCurrentWidget(ui->page_3);
}

void client::on_retour_clicked()
{
    ui->stacked->setCurrentWidget(ui->page);
}

void client::on_historique_clicked()
{
    ui->stacked->setCurrentWidget(ui->page_4);
    afficher_historique();
}

void client::on_classement_clicked()
{
    ui->stacked->setCurrentWidget(ui->page_5);
}

void client::on_pushButton_3_clicked()
{
    ui->stacked->setCurrentWidget(ui->page);
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
    ui->stacked->setCurrentWidget(ui->page);
}
int client::getNextId() {
    static int lastId = 0;  // garde le dernier ID utilisé

    // Si c'est la première fois, lire le max ID depuis la base
    if (lastId == 0) {
        QSqlQuery query;
        if (query.exec("SELECT MAX(ID) FROM CLIENT") && query.next()) {
            QVariant maxId = query.value(0);
            if (!maxId.isNull())
                lastId = maxId.toInt();
        } else {
            throw std::runtime_error("Impossible de récupérer le dernier ID");
        }
    }

    lastId++;  // incrémenter pour le prochain client
    return lastId;
}
void client::on_ajouter_clicked()
{
    try {
        // 🔹 Vérification de la saisie avant tout (sans ID)
        verifierSaisie();

        // 🔹 Générer automatiquement l'ID
        this->id = getNextId(); // fonction qui retourne le prochain ID disponible

        // Récupération des autres valeurs
        this->nom = ui->nom_client->text();
        this->prenom = ui->prenom_client->text();
        this->telephone = ui->tel->text();
        this->email = ui->email->text();
        this->adresse = ui->adresse->text();
        this->date_inscription = ui->date_inscription->date();

        if(ui->particulier->isChecked())
            this->type_client = "particulier";
        else if(ui->profisionnel->isChecked())
            this->type_client = "professionnel";
        else {
            QMessageBox::warning(this, "Erreur", "Choisir type client");
            return;
        }

        if(ui->vierge_2->isChecked())
            this->type_huile_prefere = "vierge";
        else if(ui->bio_2->isChecked())
            this->type_huile_prefere = "bio";
        else {
            QMessageBox::warning(this, "Erreur", "Choisir type huile");
            return;
        }

        QString emb = ui->emballage_prefere->currentText();
        if(emb == "1L" || emb == "3L" || emb == "5L" || emb == "autre")
            this->emballage_prefere = emb;
        else {
            QMessageBox::warning(this, "Erreur", "Emballage invalide");
            return;
        }

        this->total_achat = ui->total_achat->text();
        this->points_fidelite = ui->points_fidelite->text();

        // Ajouter le client dans la base
        if(ajouter()) {
            QMessageBox::information(this, "Succès", "Client ajouté avec ID: " + QString::number(this->id));
            remplirTableau();
        }

    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Erreur saisie", e.what());
    }
}


void client::on_modifier_clicked()
{
    try {
        // 🔹 Vérification complète des champs

        // 🔹 Récupération des valeurs après validation
        this->id = ui->id_client->text().toInt();
        this->nom = ui->nom_client->text();
        this->prenom = ui->prenom_client->text();
        this->telephone = ui->tel->text();
        this->email = ui->email->text();
        this->adresse = ui->adresse->text();
        this->date_inscription = ui->date_inscription->date();
        this->type_client = ui->particulier->isChecked() ? "particulier" : "professionnel";
        this->type_huile_prefere = ui->vierge_2->isChecked() ? "vierge" : "bio";
        this->emballage_prefere = ui->emballage_prefere->currentText();
        this->total_achat = ui->total_achat->text();
        this->points_fidelite = ui->points_fidelite->text();

        // 🔹 Appel de la fonction modifier()
        if (modifier()) {
            QMessageBox::information(this, "Succès", "Client modifié avec succès");
            remplirTableau();
        } else {
            QMessageBox::warning(this, "Échec", "Erreur lors de la modification du client");
        }

    } catch (const std::exception &e) {
        // 🔹 Affichage des erreurs de saisie
        QMessageBox::critical(this, "Erreur saisie", e.what());
    }
}
void client::verifierSaisie()
{
    // 🔹 Champs obligatoires
    if (ui->id_client->text().isEmpty() ||
        ui->nom_client->text().isEmpty() ||
        ui->prenom_client->text().isEmpty() ||
        ui->tel->text().isEmpty())
    {
        throw std::runtime_error("Veuillez remplir tous les champs obligatoires !");
    }

    // 🔹 Vérification de l'ID (entier positif)
    bool ok;
    int idTemp = ui->id_client->text().toInt(&ok);
    if (!ok || idTemp <= 0)
        throw std::runtime_error("ID invalide !");

    // 🔹 Vérification unicité ID dans la base
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM CLIENT WHERE ID = :id");
    query.bindValue(":id", idTemp);
    if (!query.exec())
        throw std::runtime_error("Erreur base de données !");
    query.next();
    if (query.value(0).toInt() > 0)
        throw std::runtime_error("ID déjà existant !");

    // 🔹 Vérification téléphone (au moins 8 chiffres) et unicité
    QString tel = ui->tel->text();
    if (tel.length() < 8 || !tel.toULongLong(&ok))
        throw std::runtime_error("Téléphone invalide !");
    query.prepare("SELECT COUNT(*) FROM CLIENT WHERE TELEPHONE = :tel");
    query.bindValue(":tel", tel);
    if (!query.exec())
        throw std::runtime_error("Erreur base de données !");
    query.next();
    if (query.value(0).toInt() > 0)
        throw std::runtime_error("Numéro de téléphone déjà utilisé !");

    // 🔹 Vérification nom et prénom (lettres et espaces)
    QRegularExpression regexNom("^[a-zA-Z\\s]+$");
    if (!regexNom.match(ui->nom_client->text()).hasMatch())
        throw std::runtime_error("Nom invalide !");
    if (!regexNom.match(ui->prenom_client->text()).hasMatch())
        throw std::runtime_error("Prénom invalide !");

    // 🔹 Vérification email (optionnel, unique et @gmail.com)
    QString email = ui->email->text();
    if (!email.isEmpty())
    {
        QRegularExpression regexEmail("^[\\w\\.]+@gmail\\.com$");
        if (!regexEmail.match(email).hasMatch())
            throw std::runtime_error("Email invalide ! Doit se terminer par @gmail.com");
        // Vérification unicité email
        query.prepare("SELECT COUNT(*) FROM CLIENT WHERE EMAIL = :email");
        query.bindValue(":email", email);
        if (!query.exec())
            throw std::runtime_error("Erreur base de données !");
        query.next();
        if (query.value(0).toInt() > 0)
            throw std::runtime_error("Email déjà utilisé !");
    }

    // 🔹 Vérification date
    if (!ui->date_inscription->date().isValid())
        throw std::runtime_error("Date invalide !");

    // 🔹 Vérification type client (radio button)
    if (!ui->particulier->isChecked() && !ui->profisionnel->isChecked())
        throw std::runtime_error("Choisir le type de client");

    // 🔹 Vérification type huile (radio button)
    if (!ui->vierge_2->isChecked() && !ui->bio_2->isChecked())
        throw std::runtime_error("Choisir le type d'huile");

    // 🔹 Vérification emballage (combo box)
    QString emb = ui->emballage_prefere->currentText();
    if (emb != "1L" && emb != "3L" && emb != "5L" && emb != "autre")
        throw std::runtime_error("Emballage invalide");
}
void client::on_supprimer_clicked()
{
    try {
        // 🔹 Vérification que le champ ID n'est pas vide
        QString idText = ui->id_client_2->text().trimmed();
        if (idText.isEmpty())
            throw std::runtime_error("Veuillez entrer un ID !");

        // 🔹 Conversion en entier et contrôle
        bool ok;
        int id = idText.toInt(&ok);
        if (!ok) // on accepte maintenant 0 comme valide
            throw std::runtime_error("ID invalide !");

        // 🔹 Vérifier si l'ID existe dans le tableau
        bool idExiste = false;
        for (int i = 0; i < ui->table->rowCount(); ++i) {
            QTableWidgetItem *idItem = ui->table->item(i, 0);
            if (idItem && idItem->text().toInt() == id) {
                idExiste = true;
                break;
            }
        }
        if (!idExiste) {
            throw std::runtime_error("ID introuvable dans le tableau !");
        }

        // 🔹 Confirmation de l'utilisateur
        if (QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous supprimer ce client ?",
                                  QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
            return;

        // 🔹 Suppression
        if (client::supprimer(id)) {
            QMessageBox::information(this, "Succès", "Client supprimé avec succès.");
            remplirTableau();  // rafraîchir le tableau
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression du client.");
        }

    } catch (const std::exception &e) {
        // 🔹 Affichage des erreurs de saisie
        QMessageBox::warning(this, "Erreur de saisie", e.what());
    }
}
void client::on_recherche1_2_clicked()
{
    // 🔹 Récupérer le texte du champ de recherche
    QString texte = ui->recherche1->text().trimmed();

    // 🔹 Si le champ est vide → afficher tout
    if (texte.isEmpty()) {
        for (int i = 0; i < ui->table->rowCount(); ++i) {
            ui->table->setRowHidden(i, false);
        }
        return;
    }

    bool trouve = false;

    // 🔹 Vérifier si le texte est un nombre (ID)
    bool ok;
    int idRecherche = texte.toInt(&ok);

    // 🔹 Parcourir toutes les lignes du tableau
    for (int i = 0; i < ui->table->rowCount(); ++i) {
        QTableWidgetItem *itemId  = ui->table->item(i, 0); // colonne ID
        QTableWidgetItem *itemNom = ui->table->item(i, 1); // colonne Nom

        if (!itemId || !itemNom) {
            ui->table->setRowHidden(i, true);
            continue;
        }

        bool match = false;

        // 🔹 Correspondance ID si c’est un nombre
        if (ok && itemId->text().toInt() == idRecherche)
            match = true;

        // 🔹 Correspondance Nom (insensible à la casse)
        if (itemNom->text().contains(texte, Qt::CaseInsensitive))
            match = true;

        // 🔹 Afficher ou cacher la ligne
        ui->table->setRowHidden(i, !match);

        if (match)
            trouve = true;
    }

    // 🔹 Message si aucun résultat
    if (!trouve) {
        QMessageBox::information(this, "Résultat", "Aucun client trouvé !");
    }
}

void client::on_tri_2_clicked()
{
    // 🔹 Récupérer le texte du champ de recherche/filtrage
    QString texte = ui->tri->text().trimmed();

    // 🔹 Si le champ est vide → afficher toutes les lignes
    if (texte.isEmpty()) {
        for (int i = 0; i < ui->table->rowCount(); ++i)
            ui->table->setRowHidden(i, false);

        // 🔹 Trier toutes les lignes par Nom (colonne 1)
        ui->table->sortItems(1, Qt::AscendingOrder);
        return;
    }

    bool trouve = false;

    // 🔹 Vérifier si le texte est un nombre (ID)
    bool ok;
    int idRecherche = texte.toInt(&ok);

    // 🔹 Parcourir toutes les lignes du tableau
    for (int i = 0; i < ui->table->rowCount(); ++i) {
        QTableWidgetItem *itemId   = ui->table->item(i, 0); // colonne ID
        QTableWidgetItem *itemNom  = ui->table->item(i, 1); // colonne Nom
        QTableWidgetItem *itemType = ui->table->item(i, 7); // colonne Type_client

        if (!itemId || !itemNom || !itemType) {
            ui->table->setRowHidden(i, true);
            continue;
        }

        bool match = false;

        // 🔹 Correspondance ID si texte numérique
        if (ok && itemId->text().toInt() == idRecherche)
            match = true;

        // 🔹 Correspondance Nom (insensible à la casse)
        if (itemNom->text().contains(texte, Qt::CaseInsensitive))
            match = true;

        // 🔹 Correspondance Type_client (insensible à la casse)
        if (itemType->text().contains(texte, Qt::CaseInsensitive))
            match = true;

        // 🔹 Afficher ou cacher la ligne
        ui->table->setRowHidden(i, !match);

        if (match)
            trouve = true;
    }

    // 🔹 Trier les lignes visibles par ordre alphabétique Nom
    ui->table->sortItems(1, Qt::AscendingOrder);

    // 🔹 Message si aucun résultat
    if (!trouve) {
        QMessageBox::information(this, "Résultat", "Aucun client trouvé !");
    }
}


void client::on_annuler_clicked()
{
    ui->id_client->clear();
    ui->nom_client->clear();
    ui->prenom_client->clear();
    ui->tel->clear();
    ui->email->clear();
    ui->adresse->clear();
    ui->total_achat->clear();
    ui->points_fidelite->clear();

    // 🔹 Réinitialiser la date à aujourd'hui
    ui->date_inscription->setDate(QDate::currentDate());

    // 🔹 Réinitialiser les radio buttons et combo box
    ui->particulier->setChecked(false);
    ui->profisionnel->setChecked(false);
    ui->vierge_2->setChecked(false);
    ui->bio_2->setChecked(false);
    ui->emballage_prefere->setCurrentIndex(0);

    // 🔹 Réinitialiser le line edit de recherche/tri
    ui->tri->clear();
    ui->recherche1->clear();

    // 🔹 Afficher toutes les lignes du tableau
    for (int i = 0; i < ui->table->rowCount(); ++i) {
        ui->table->setRowHidden(i, false);
    }

    // 🔹 Trier le tableau par Nom (colonne 1) après annulation
    ui->table->sortItems(1, Qt::AscendingOrder);
}


void client::on_annuler1_clicked()
{
    // 🔹 Réinitialiser le champ ID pour suppression
    ui->id_client_2->clear();

    // 🔹 Afficher toutes les lignes du tableau
    for (int i = 0; i < ui->table->rowCount(); ++i) {
        ui->table->setRowHidden(i, false);
    }

    // 🔹 Trier le tableau par Nom (colonne 1)
    ui->table->sortItems(1, Qt::AscendingOrder);
}


void client::on_pdf_clicked()
{
    QString nom = ui->recherche->text().trimmed();

    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Enregistrer PDF",
        QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/client.pdf",
        "PDF (*.pdf)"
        );

    if (fileName.isEmpty())
        return;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Erreur", "Base de données non ouverte");
        return;
    }

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QPainter painter(&printer);

    int y = 100;

    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(200, y, "Liste Client");
    y += 100;

    QSqlQuery query;

    // ✔️ requête sécurisée
    if (!query.prepare("SELECT nom, prenom, total_achat, points_fidelite "
                       "FROM CLIENT WHERE nom LIKE :nom")) {
        QMessageBox::critical(this, "Erreur", "Erreur préparation requête");
        return;
    }

    query.bindValue(":nom", "%" + nom + "%");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
        return;
    }

    painter.setFont(QFont("Arial", 10));

    bool found = false;

    while (query.next()) {
        found = true;

        painter.drawText(100, y, "Nom: " + query.value(0).toString());
        y += 50;

        painter.drawText(100, y, "Prénom: " + query.value(1).toString());
        y += 50;

        painter.drawText(100, y, "Total Achat: " + query.value(2).toString());
        y += 50;

        painter.drawText(100, y, "Points: " + query.value(3).toString());
        y += 100;
    }

    if (!found) {
        painter.drawText(100, y, "Aucun client trouvé");
    }

    painter.end();

    QMessageBox::information(this, "Succès", "PDF généré !");
}
void client::on_recherche2_2_clicked()
{
    QString motcle = ui->recherche2->text().trimmed();

    bool isParticulier = ui->particulier_2->isChecked();
    bool isProfessionnel = ui->profisionnel_2->isChecked();

    QSqlQuery query;

    QString sql = "SELECT nom, prenom, total_achat,points_fidelite FROM CLIENT WHERE 1=1";

    // filtre texte
    if (!motcle.isEmpty()) {
        sql += " AND (nom LIKE :mc OR prenom LIKE :mc)";
    }

    // filtre type client
    if (isParticulier) {
        sql += " AND type_client = 'particulier'";
    }
    else if (isProfessionnel) {
        sql += " AND type_client = 'professionnel'";
    }

    query.prepare(sql);

    if (!motcle.isEmpty()) {
        query.bindValue(":mc", "%" + motcle + "%");
    }

    if (!query.exec()) {
        qDebug() << "Erreur SQL :" << query.lastError().text();
        return;
    }

    ui->table1->setRowCount(0);
    ui->table1->setColumnCount(4);
    ui->table1->setHorizontalHeaderLabels(QStringList()
                                          << "Nom" << "Prénom" << "Total Achat");

    int row = 0;

    while (query.next()) {
        ui->table1->insertRow(row);

        ui->table1->setItem(row, 0,
                            new QTableWidgetItem(query.value("nom").toString()));

        ui->table1->setItem(row, 1,
                            new QTableWidgetItem(query.value("prenom").toString()));

        ui->table1->setItem(row, 2,
                            new QTableWidgetItem(query.value("total_achat").toString()));

        ui->table1->setItem(row, 3,
                            new QTableWidgetItem(query.value("points_fidelite").toString()));


        row++;
    }
}

void client::on_appliquer_clicked()
{
    QDate date = ui->date_2->date();

    bool vierge = ui->vierge->isChecked();
    bool bio = ui->bio->isChecked();

    QSqlQuery query;

    QString sql =
        "SELECT nom, prenom, total_achat, points_fidelite, date_inscription "
        "FROM CLIENT "
        "WHERE date_inscription <= TO_DATE(:date, 'YYYY-MM-DD')";

    // filtres
    if (vierge && !bio) {
        sql += " AND LOWER(type_huile_prefere) LIKE '%vierge%'";
    }
    else if (!vierge && bio) {
        sql += " AND LOWER(type_huile_prefere) LIKE '%bio%'";
    }
    else if (vierge && bio) {
        sql += " AND (LOWER(type_huile_prefere) LIKE '%vierge%' OR LOWER(type_huile_prefere) LIKE '%bio%')";
    }

    query.prepare(sql);
    query.bindValue(":date", date.toString("yyyy-MM-dd"));

    if (!query.exec()) {
        qDebug() << "SQL ERROR:" << query.lastError().text();
        return;
    }

    ui->table1->setRowCount(0);
    ui->table1->setColumnCount(4);

    int row = 0;

    while (query.next()) {
        ui->table1->insertRow(row);

        ui->table1->setItem(row, 0,
                            new QTableWidgetItem(query.value("nom").toString()));

        ui->table1->setItem(row, 1,
                            new QTableWidgetItem(query.value("prenom").toString()));

        ui->table1->setItem(row, 2,
                            new QTableWidgetItem(query.value("total_achat").toString()));

        ui->table1->setItem(row, 3,
                            new QTableWidgetItem(query.value("points_fidelite").toString()));

        row++;
    }
}
void client::afficher_historique()
{
    QSqlQuery query;

    QString sql =
        "SELECT id, date_inscription, type_huile_prefere, emballage_prefere, total_achat "
        "FROM CLIENT "
        "ORDER BY date_inscription DESC";

    if (!query.exec(sql)) {
        qDebug() << "SQL ERROR:" << query.lastError().text();
        return;
    }

    ui->table3->setRowCount(0);
    ui->table3->setColumnCount(5);

    ui->table3->setHorizontalHeaderLabels(
        QStringList() << "id"
                      << "Date inscription"
                      << "Type huile"
                      << "Emballage"
                      << "Total achat"
                      <<"points de fidelite"
        );

    int row = 0;

    while (query.next()) {
        ui->table3->insertRow(row);

        ui->table3->setItem(row, 0,
                            new QTableWidgetItem(query.value(0).toString()));

        ui->table3->setItem(row, 1,
                            new QTableWidgetItem(
                                query.value(1).toDate().toString("dd/MM/yyyy")
                                ));

        ui->table3->setItem(row, 2,
                            new QTableWidgetItem(query.value(2).toString()));

        ui->table3->setItem(row, 3,
                            new QTableWidgetItem(query.value(3).toString()));

        ui->table3->setItem(row, 4,
                            new QTableWidgetItem(query.value(4).toString()));

        row++;
    }
}

void client::on_exporter1_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Exporter PDF",
        "",
        "PDF Files (*.pdf)"
        );

    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(".pdf"))
        fileName += ".pdf";

    QPdfWriter pdf(fileName);
    pdf.setPageSize(QPageSize::A4);
    pdf.setResolution(300);

    QPainter painter(&pdf);
    if (!painter.isActive()) return;

    int x = 80;
    int y = 150;

    int colWidth = 400;   // 🔽 tableau plus petit
    int rowHeight = 220;  // 🔽 compact

    QColor olive(107, 142, 35);
    QColor gold(212, 175, 55);
    QColor black(0, 0, 0);

    // ===== TITRE =====
    painter.setPen(olive);
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(x, y, "Historique Clients");
    y += 400;

    // ===== HEADER =====
    painter.setFont(QFont("Arial", 9, QFont::Bold));

    for (int col = 0; col < ui->table3->columnCount(); col++) {

        QRect rect(x + col * colWidth, y, colWidth, rowHeight);

        painter.setPen(gold);
        painter.setBrush(olive);
        painter.drawRect(rect);

        painter.setPen(Qt::black);
        painter.drawText(rect.adjusted(5, 0, 0, 0),
                         ui->table3->horizontalHeaderItem(col)->text());
    }

    y += rowHeight;

    // ===== DATA =====
    painter.setFont(QFont("Arial", 8));

    for (int row = 0; row < ui->table3->rowCount(); row++) {

        for (int col = 0; col < ui->table3->columnCount(); col++) {

            QString text = ui->table3->item(row, col)
            ? ui->table3->item(row, col)->text()
            : "";

            QRect rect(x + col * colWidth, y, colWidth, rowHeight);

            // alternance couleur fond
            if (row % 2 == 0)
                painter.setBrush(QColor(245, 245, 245)); // gris clair
            else
                painter.setBrush(Qt::white);

            painter.setPen(olive);
            painter.drawRect(rect);

            painter.setPen(Qt::black);
            painter.drawText(rect.adjusted(5, 0, 0, 0), text);
        }

        y += rowHeight;

        // page break
        if (y > 26000) {
            pdf.newPage();
            y = 150;
        }
    }

    painter.end();
}


void client::on_appliquer2_clicked()
{
    QSqlQuery query;

    QString type;

    if (ui->particulier_3->isChecked())
        type = "particulier";
    else if (ui->profisionnel_4->isChecked())
        type = "professionnel";

    QString sql = "SELECT nom, prenom, type_client, points_fidelite FROM CLIENT";

    if (!type.isEmpty()) {
        sql += " WHERE TRIM(LOWER(type_client)) = '" + type + "'";
    }

    sql += " ORDER BY points_fidelite DESC";

    if (!query.exec(sql)) {
        qDebug() << "SQL ERROR:" << query.lastError().text();
        return;
    }

    ui->table4->setRowCount(0);
    ui->table4->setColumnCount(4);

    int row = 0;

    while (query.next()) {
        ui->table4->insertRow(row);

        ui->table4->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->table4->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->table4->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->table4->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));

        row++;
    }
}

void client::on_appliquer3_clicked()
{
    QString sql = "SELECT id, nom, prenom, total_achat, emballage_prefere, type_client "
                  "FROM CLIENT WHERE 1=1";

    bool particulier = ui->particulier_4->isChecked();
    bool professionnel = ui->profisionnel_5->isChecked();

    if (particulier || professionnel)
    {
        sql += " AND (";

        if (particulier && professionnel)
        {
            sql += "type_client = 'particulier' OR type_client = 'professionnel'";
        }
        else if (particulier)
        {
            sql += "type_client = 'particulier'";
        }
        else if (professionnel)
        {
            sql += "type_client = 'professionnel'";
        }

        sql += ")";
    }

    // 🔥 TRI AJOUTÉ ICI
    sql += " ORDER BY total_achat ASC, emballage_prefere ASC";

    QSqlQuery query;
    if (!query.exec(sql))
    {
        qDebug() << "SQL Error:" << query.lastError().text();
        return;
    }

    ui->table5->clearContents();
    ui->table5->setRowCount(0);

    int row = 0;

    while (query.next())
    {
        ui->table5->insertRow(row);

        ui->table5->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        ui->table5->setItem(row, 1, new QTableWidgetItem(query.value("nom").toString()));
        ui->table5->setItem(row, 2, new QTableWidgetItem(query.value("prenom").toString()));
        ui->table5->setItem(row, 3, new QTableWidgetItem(query.value("total_achat").toString()));
        ui->table5->setItem(row, 4, new QTableWidgetItem(query.value("emballage_prefere").toString()));
    }

    ui->table5->setColumnCount(5);
    ui->table5->setHorizontalHeaderLabels(
        {"ID", "Nom", "Prenom", "Total Achat", "Emballage"}
        );
}
void client::on_exporter2_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(
        this, "Exporter PDF", "", "*.pdf"
        );

    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(".pdf"))
        fileName += ".pdf";

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QPainter painter;
    if (!painter.begin(&printer))
        return;

    int y = 100;

    QFont titleFont("Arial", 14, QFont::Bold);
    QFont sectionFont("Arial", 11, QFont::Bold);
    QFont textFont("Arial", 9);

    QColor olive(107, 142, 35);
    QColor gold(212, 175, 55);

    painter.setFont(titleFont);
    painter.setPen(olive);
    painter.drawText(200, 50, "RAPPORT CLIENTS");

    QStringList types = {"particulier", "professionnel"};

    // =========================================================
    // 1. CLIENTS FIDELES PAR TYPE (ASC)
    // =========================================================
    for (QString type : types)
    {
        painter.setFont(sectionFont);
        painter.setPen(gold);
        painter.drawText(50, y, "CLIENTS FIDELES - " + type.toUpper());
        y += 25;

        painter.setFont(textFont);
        painter.setPen(Qt::black);

        QSqlQuery q;

        q.prepare(
            "SELECT id, nom, prenom, total_achat, emballage_prefere, type_client, points_fidelite "
            "FROM CLIENT "
            "WHERE type_client = :type "
            "AND NVL(points_fidelite,0) BETWEEN 0 AND 100 "
            "ORDER BY points_fidelite DESC"
            );

        q.bindValue(":type", type);

        if (q.exec())
        {
            while (q.next())
            {
                painter.drawText(50, y, q.value(0).toString());
                painter.drawText(90, y, q.value(1).toString());
                painter.drawText(180, y, q.value(2).toString());
                painter.drawText(280, y, q.value(3).toString());
                painter.drawText(360, y, q.value(4).toString());
                painter.drawText(460, y, q.value(5).toString());
                painter.drawText(560, y, q.value(6).toString());

                y += 18;

                if (y > 1000)
                {
                    printer.newPage();
                    y = 100;
                }
            }
        }

        y += 30;
    }

    // =========================================================
    // 2. MEILLEURS CLIENTS PAR TYPE (ASC)
    // =========================================================
    for (QString type : types)
    {
        painter.setFont(sectionFont);
        painter.setPen(olive);
        painter.drawText(50, y, "MEILLEURS CLIENTS - " + type.toUpper());
        y += 25;

        painter.setFont(textFont);
        painter.setPen(Qt::black);

        QSqlQuery q2;

        q2.prepare(
            "SELECT id, nom, prenom, total_achat, emballage_prefere, type_client, points_fidelite "
            "FROM CLIENT "
            "WHERE type_client = :type "
            "ORDER BY total_achat DESC"
            );

        q2.bindValue(":type", type);

        if (q2.exec())
        {
            while (q2.next())
            {
                painter.drawText(50, y, q2.value(0).toString());
                painter.drawText(90, y, q2.value(1).toString());
                painter.drawText(180, y, q2.value(2).toString());
                painter.drawText(280, y, q2.value(3).toString());
                painter.drawText(360, y, q2.value(4).toString());
                painter.drawText(460, y, q2.value(5).toString());
                painter.drawText(560, y, q2.value(6).toString());

                y += 18;

                if (y > 1000)
                {
                    printer.newPage();
                    y = 100;
                }
            }
        }

        y += 30;
    }

    painter.end();
}
void client::on_envoyer_clicked()
{
    QString msg = ui->message_2->toPlainText().trimmed();
    if (msg.isEmpty())
        return;

    QString lower = msg.toLower();
    QStringList reponses;

    // ===== INTENTIONS TEXTE =====
    if (lower.contains("bonjour") || lower.contains("salut"))
        reponses << "Bonjour chez Golden Oil Press.";

    if (lower.contains("entreprise"))
        reponses << "Golden Oil Press est spécialisée dans l’huile d’olive vierge et bio.";

    if (lower.contains("service"))
        reponses << "Nous assurons suivi client, assistance et satisfaction.";

    if (lower.contains("huile"))
        reponses << "Nous proposons huile vierge et huile bio de haute qualité.";

    if (lower.contains("commande"))
        reponses << "Vous pouvez commander facilement avec livraison assurée.";

    if (lower.contains("emballage"))
        reponses << "Formats disponibles : 1L, 3L et 5L.";

    if (lower.contains("fidèle") || lower.contains("points"))
        reponses << "Programme fidélité avec réductions exclusives.";

    // ===== FICHIER / IMAGE =====
    if (lower.contains("fichier") || lower.contains("envoyer"))
    {
        QString filePath = QFileDialog::getOpenFileName(this, "Choisir un fichier");

        if (!filePath.isEmpty())
        {
            QFileInfo info(filePath);
            QString suffix = info.suffix().toLower();

            ui->chat->appendPlainText("📎 Fichier reçu : " + info.fileName());

            // ===== IMAGE -> IA =====
            if (suffix == "jpg" || suffix == "jpeg" || suffix == "png" ||
                suffix == "bmp" || suffix == "gif" || suffix == "webp")
            {
                ui->chat->appendPlainText("🧠 Analyse IA de l’image en cours...");


                ajouterDiscussionRecente("Image", info.fileName());
                return;
            }

            // ===== TXT =====
            if (suffix == "txt")
            {
                QFile file(filePath);
                if (file.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    QString contenu = QTextStream(&file).readAll();
                    reponses << "📝 Résumé : " + contenu.left(200) + "...";
                }
            }

            // ===== AUTRES =====
            else
            {
                reponses << "📄 Fichier reçu mais type non analysé.";
            }

            ajouterDiscussionRecente("Fichier", info.fileName());
        }
    }

    // ===== VOCAL =====
    if (lower.contains("vocal") || lower.contains("audio"))
    {
        reponses << "🎤 Enregistrement vocal en cours...";

        QString fileName = "audio_" +
                           QString::number(QDateTime::currentSecsSinceEpoch()) + ".wav";

        reponses << "✅ Message vocal enregistré : " + fileName;
    }

    // ===== PAR DEFAUT =====
    if (reponses.isEmpty())
        reponses << "Posez une question sur l’entreprise, produits ou fichiers.";

    // ===== AFFICHAGE =====
    ui->chat->appendPlainText("Vous: " + msg);

    for (const QString &rep : reponses)
        ui->chat->appendPlainText("Bot: " + rep);

    ui->chat->appendPlainText("");

    // ===== RESET =====
    ui->message_2->clear();
    ui->message_2->setFocus();

    ajouterDiscussionRecente("Client", msg);
}


void client::on_fichier_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Choisir un fichier");

    if (filePath.isEmpty())
        return;

    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();
    QString suffix = fileInfo.suffix().toLower();

    ui->chat->appendPlainText("📎 Fichier reçu : " + fileName);

    QString analyse;

    QFile file(filePath);

    // ================= IMAGES =================
    if (suffix == "jpg" || suffix == "jpeg" || suffix == "png" ||
        suffix == "bmp" || suffix == "gif" || suffix == "webp")
    {
        QPixmap pix(filePath);

        if (!pix.isNull())
        {
            analyse =
                "🖼️ Image détectée :\n"
                "• Nom : " + fileName + "\n"
                             "• Format : " + suffix.toUpper() + "\n"
                                     "• Taille : " + QString::number(fileInfo.size() / 1024) + " KB\n"
                                                            "• Statut : chargée avec succès.";

            ui->chat->appendPlainText(analyse);
            ui->chat->appendPlainText("🖼️ Image prête à être affichée dans l’interface.");
        }
        else
        {
            analyse = "❌ Erreur : impossible de charger l’image.";
            ui->chat->appendPlainText(analyse);
        }

        ajouterDiscussionRecente("Fichier Image", fileName);
        return;
    }

    // ================= TXT =================
    else if (suffix == "txt")
    {
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            QString contenu = in.readAll();

            analyse =
                "📝 Fichier texte :\n"
                "• Nom : " + fileName + "\n"
                             "• Aperçu :\n" + contenu.left(300) + "...";
        }
    }

    // ================= CSV =================
    else if (suffix == "csv")
    {
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            QStringList lignes = in.readAll().split("\n");

            int nbLignes = lignes.size() - 1;
            int total = 0;

            if (lignes.size() > 1)
            {
                QStringList headers = lignes[0].split(",");

                for (int i = 1; i < lignes.size(); i++)
                {
                    QStringList champs = lignes[i].split(",");
                    if (champs.size() > 3)
                        total += champs[3].toInt();
                }

                analyse =
                    "📊 CSV analysé :\n"
                    "• Lignes : " + QString::number(nbLignes) + "\n"
                                                  "• Total : " + QString::number(total) + "\n"
                                               "• Colonnes : " + headers.join(", ");
            }
        }
    }

    // ================= JSON =================
    else if (suffix == "json")
    {
        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray data = file.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);

            if (doc.isObject())
            {
                QStringList keys = doc.object().keys();

                analyse =
                    "🧩 JSON détecté :\n"
                    "• Clés : " + QString::number(keys.size()) + "\n"
                                                     "• Liste : " + keys.join(", ");
            }
            else
            {
                analyse = "⚠️ JSON invalide ou vide.";
            }
        }
    }

    // ================= PDF =================
    else if (suffix == "pdf")
    {
        analyse =
            "📄 PDF détecté :\n"
            "• Nom : " + fileName + "\n"
                         "• Analyse avancée nécessite module PDF ou OCR.";
    }

    // ================= AUTRES =================
    else
    {
        analyse =
            "⚠️ Type non supporté : " + suffix + "\n"
                                                "• Nom : " + fileName;
    }

    // ================= AFFICHAGE =================
    ui->chat->appendPlainText(analyse);

    // ================= HISTORIQUE =================
    ajouterDiscussionRecente("Fichier", fileName);
}
void client::ajouterDiscussionRecente(const QString &nom, const QString &message)
{
    QString time = QTime::currentTime().toString("hh:mm");

    // 🔹 Aperçu intelligent (nettoyage + limite)
    QString propre = message.simplified();
    QString apercu = propre.left(40);
    if (propre.length() > 40)
        apercu += "...";

    QString titre = QString("🫒 %1  •  %2  (%3)").arg(nom, apercu, time);

    // 🔹 Chercher si discussion existe
    for (int i = 0; i < ui->list->count(); i++)
    {
        QListWidgetItem *item = ui->list->item(i);

        QString nomExistant = item->data(Qt::UserRole + 1).toString();

        if (nomExistant == nom)
        {
            // 🔸 Mise à jour du titre
            item->setText(titre);

            // 🔸 Historique complet structuré
            QVariantList historique = item->data(Qt::UserRole).toList();

            QVariantMap msgData;
            msgData["texte"] = message;
            msgData["heure"] = time;

            historique.append(msgData);
            item->setData(Qt::UserRole, historique);

            // 🔸 Remonter en haut
            ui->list->insertItem(0, ui->list->takeItem(i));
            ui->list->setCurrentRow(0);

            return;
        }
    }

    // 🔹 Nouvelle discussion
    QListWidgetItem *item = new QListWidgetItem(titre);

    QVariantList historique;

    QVariantMap msgData;
    msgData["texte"] = message;
    msgData["heure"] = time;

    historique.append(msgData);

    item->setData(Qt::UserRole, historique);     // historique complet
    item->setData(Qt::UserRole + 1, nom);        // nom client
    item->setToolTip("Conversation avec " + nom);

    ui->list->insertItem(0, item);
    ui->list->setCurrentRow(0);

    // 🔹 Limite mémoire
    while (ui->list->count() > MAX_DISCUSSIONS)
        delete ui->list->takeItem(ui->list->count() - 1);
}


void client::on_nouvelle_discussion_clicked()
{
    // 🔹 vider le chat
    ui->chat->clear();

    // 🔹 message d’accueil
    ui->chat->appendPlainText("Bot: Nouvelle discussion démarrée.");
    ui->chat->appendPlainText("Bot: Bonjour ! Comment puis-je vous aider ?");
    ui->chat->appendPlainText("");

    // 🔹 vider la zone de saisie
    ui->message_2->clear();

    // 🔹 focus pour écrire directement
    ui->message_2->setFocus();
}


void client::on_exporter2_2_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Exporter vers Excel",
        "",
        "Fichiers Excel (*.csv)"
        );

    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(".csv"))
        fileName += ".csv";

    QSqlQuery query;
    if (!query.exec("SELECT id, nom, prenom, total_achat FROM CLIENT")) {
        qDebug() << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Erreur SQL");
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier");
        return;
    }

    QTextStream out(&file);

    // 🔹 Header Excel
    out << "ID,Nom,Prenom,Total Achat\n";

    // 🔹 Données
    while (query.next())
    {
        QString id = query.value(0).toString();
        QString nom = query.value(1).toString();
        QString prenom = query.value(2).toString();
        QString total = query.value(3).toString();

        out << id << "," << nom << "," << prenom << "," << total << "\n";
    }

    file.close();

    QMessageBox::information(this, "Succès", "Export Excel terminé !");
}

void client::afficherCourbe()
{
    QLineSeries *series = new QLineSeries();
    series->append(0, 0);
    series->append(1, 10);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Test Courbe");

    QChartView *view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = new QVBoxLayout(ui->chart);
    ui->chart->setLayout(layout);

    layout->addWidget(view);
}
void client::afficherCourbeDansTable2()
{
    QSqlQuery query;

    if (!query.exec("SELECT date_inscription, total_achat FROM CLIENT ORDER BY date_inscription")) {
        qDebug() << query.lastError().text();
        return;
    }

    QLineSeries *seriesAchat = new QLineSeries();
    QLineSeries *seriesCumul = new QLineSeries();

    double cumul = 0;

    while (query.next())
    {
        QDate date = query.value(0).toDate();
        double achat = query.value(1).toDouble();

        cumul += achat;

        qint64 x = date.startOfDay().toMSecsSinceEpoch();

        seriesAchat->append(x, achat);
        seriesCumul->append(x, cumul);
    }

    QChart *chart = new QChart();
    chart->addSeries(seriesAchat);
    chart->addSeries(seriesCumul);
    chart->setTitle("Évolution des achats clients");

    QDateTimeAxis *axisX = new QDateTimeAxis();
    axisX->setFormat("dd/MM");

    QValueAxis *axisY = new QValueAxis();

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    seriesAchat->attachAxis(axisX);
    seriesCumul->attachAxis(axisX);
    seriesAchat->attachAxis(axisY);
    seriesCumul->attachAxis(axisY);

    QChartView *view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);

    QLayout *layout = ui->chart->layout();
    if (!layout)
    {
        layout = new QVBoxLayout(ui->chart);
        ui->chart->setLayout(layout);
    }

    // nettoyage ancien contenu
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }

    layout->addWidget(view);
}

void client::on_executer_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Exporter vers CSV",
        "",
        "Fichiers CSV (*.csv)"
        );

    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(".csv"))
        fileName += ".csv";

    QSqlQuery query;

    if (!query.exec("SELECT id, nom, prenom, total_achat FROM CLIENT")) {
        QMessageBox::critical(this, "Erreur", "Erreur SQL");
        return;
    }

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier");
        return;
    }

    QTextStream out(&file);

    out << "ID,Nom,Prenom,Total Achat\n";

    while (query.next())
    {
        out << query.value(0).toString() << ","
            << query.value(1).toString() << ","
            << query.value(2).toString() << ","
            << query.value(3).toString() << "\n";
    }

    file.close();

    QMessageBox::information(this, "Succès", "Export terminé avec succès !");

    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}




void client::on_mode_clicked()
{
    static int mode = 0;

    if (mode == 0)
    {
        applyTheme("dark");
        mode = 1;
    }
    else if (mode == 1)
    {
        applyTheme("light");
        mode = 2;
    }
    else
    {
        applyTheme("sleep");
        mode = 0;
    }
}

void client::applyTheme(QString mode)
{
    if (mode == "dark")
    {
        ui->stacked->setStyleSheet(
            "background-color:#1e1e1e;"
            "color:white;"
            );
    }
    else if (mode == "light")
    {
        ui->stacked->setStyleSheet(
            "background-color:white;"
            "color:black;"
            );
    }
    else if (mode == "sleep")
    {
        ui->stacked->setStyleSheet(
            "background-color:#0f172a;"
            "color:#94a3b8;"
            );
    }
}

void client::ajouterMessage(const QString &id, const QString &message)
{
    QString time = QTime::currentTime().toString("hh:mm");

    QVariantMap msg;
    msg["texte"] = message;
    msg["heure"] = time;

    conversations[id].append(msg);

    updateListeConversations(id, message);
}
void client::updateListeConversations(const QString &id, const QString &lastMsg)
{
    QString time = QTime::currentTime().toString("hh:mm");

    QString apercu = lastMsg.left(30);
    if (lastMsg.length() > 30)
        apercu += "...";

    QString titre = id + " • " + apercu + " (" + time + ")";

    // 🔹 chercher si existe
    for (int i = 0; i < ui->list->count(); i++)
    {
        QListWidgetItem *it = ui->list->item(i);

        if (it->data(Qt::UserRole).toString() == id)
        {
            it->setText(titre);
            ui->list->insertItem(0, ui->list->takeItem(i));
            return;
        }
    }

    // 🔹 nouvelle conversation
    QListWidgetItem *item = new QListWidgetItem(titre);
    item->setData(Qt::UserRole, id);

    ui->list->insertItem(0, item);
}

void client::on_recherche_2_clicked()
{
        QString texte = ui->recherche->text().trimmed();

        if (texte.isEmpty())
        {
            // 🔹 Afficher tout si vide
            for (int i = 0; i < ui->list->count(); i++)
            {
                ui->list->item(i)->setHidden(false);
            }
            return;
        }

        // 🔹 Parcours des discussions
        for (int i = 0; i < ui->list->count(); i++)
        {
            QListWidgetItem *item = ui->list->item(i);

            // 🔹 Texte de l'item (nom + message)
            QString contenu = item->text();

            // 🔹 Vérifier si ça contient le texte recherché
            if (contenu.contains(texte, Qt::CaseInsensitive))
            {
                item->setHidden(false); // afficher
            }
            else
            {
                item->setHidden(true); // cacher
            }
        }
    }


