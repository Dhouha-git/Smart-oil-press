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

    query.prepare("UPDATE CLIENT SET "
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

void client::on_ajouter_clicked()
{
    try {
        // 🔹 Vérification de la saisie avant tout
        verifierSaisie();

        // Récupération des valeurs après validation
        this->id = ui->id_client->text().toInt();
        this->nom = ui->nom_client->text();
        this->prenom = ui->prenom_client->text();
        this->telephone = ui->tel->text();
        this->email = ui->email->text();
        this->adresse = ui->adresse->text();
        this->date_inscription = ui->date_inscription->date();

        if(ui->particulier->isChecked())
            this->type_client = "particulier";
        else if(ui->profisionnel->isChecked())
            this->type_client = "profisionnel";
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
            QMessageBox::information(this, "Succès", "Client ajouté");
            remplirTableau();
        }

    } catch (const std::exception &e) {
        // Affiche un message d'erreur si la saisie est invalide
        QMessageBox::critical(this, "Erreur saisie", e.what());
    }
}


void client::on_modifier_clicked()
{
    try {
        // 🔹 Vérification complète des champs
        verifierSaisie();

        // 🔹 Récupération des valeurs après validation
        this->id = ui->id_client->text().toInt();
        this->nom = ui->nom_client->text();
        this->prenom = ui->prenom_client->text();
        this->telephone = ui->tel->text();
        this->email = ui->email->text();
        this->adresse = ui->adresse->text();
        this->date_inscription = ui->date_inscription->date();
        this->type_client = ui->particulier->isChecked() ? "particulier" : "profisionnel";
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

    // 🔹 Vérification de l'ID
    bool ok;
    int idTemp = ui->id_client->text().toInt(&ok);
    if (!ok || idTemp <= 0)
        throw std::runtime_error("ID invalide !");

    // 🔹 Vérification du téléphone (au moins 8 chiffres)
    QString tel = ui->tel->text();
    if (tel.length() < 8 || !tel.toULongLong(&ok))
        throw std::runtime_error("Téléphone invalide !");

    // 🔹 Vérification nom et prénom (lettres et espaces)
    QRegularExpression regexNom("^[a-zA-Z\\s]+$");
    if (!regexNom.match(ui->nom_client->text()).hasMatch())
        throw std::runtime_error("Nom invalide !");
    if (!regexNom.match(ui->prenom_client->text()).hasMatch())
        throw std::runtime_error("Prénom invalide !");

    // 🔹 Vérification email (optionnel)
    QString email = ui->email->text();
    if (!email.isEmpty())
    {
        QRegularExpression regexEmail("^[\\w\\.]+@[\\w\\.]+\\.[a-zA-Z]{2,}$");
        if (!regexEmail.match(email).hasMatch())
            throw std::runtime_error("Email invalide !");
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

