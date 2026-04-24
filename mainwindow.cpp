#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"

#include <QComboBox>
#include <QDateTime>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlDatabase>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>

#include <cmath>
#include <limits>

namespace {

// Oracle/Qt ODBC renvoie souvent les NUMBER en double : toString() donne "1.9e+07" au lieu de "19000000".
QString variantAsPlainIntString(const QVariant &v)
{
    if (!v.isValid() || v.isNull())
        return QString();
    bool ok = false;
    const qint64 nLL = v.toLongLong(&ok);
    if (ok)
        return QString::number(nLL);
    const double d = v.toDouble(&ok);
    if (ok && std::isfinite(d)) {
        const qint64 r = static_cast<qint64>(std::llround(d));
        if (std::fabs(d - static_cast<double>(r)) < 1e-3)
            return QString::number(r);
    }
    return v.toString();
}

bool parsePositiveIntId(const QString &text, int *out)
{
    const QString t = text.trimmed();
    bool ok = false;
    qint64 n = t.toLongLong(&ok);
    if (!ok || n <= 0) {
        const double d = t.toDouble(&ok);
        if (ok && std::isfinite(d)) {
            n = static_cast<qint64>(std::llround(d));
            ok = (n > 0);
        }
    }
    if (!ok || n <= 0 || n > static_cast<qint64>(std::numeric_limits<int>::max()))
        return false;
    *out = static_cast<int>(n);
    return true;
}

QDate variantToSqlDate(const QVariant &v)
{
    QDate d = v.toDate();
    if (d.isValid())
        return d;
    const QDateTime dtm = v.toDateTime();
    if (dtm.isValid())
        return dtm.date();
    return QDate();
}

// Filtre côté application : fiable avec Oracle/ODBC (pas de souci de bind SQL).
bool ligneCorrespondAuFiltre(int critere, const QString &needle, const QSqlQuery &q)
{
    if (needle.isEmpty())
        return true;

    switch (critere) {
    case 0: { // Date affichée jj/mm/aaaa (+ formats saisis courants)
        const QVariant dv = q.value(1);
        QDate d = variantToSqlDate(dv);
        if (!d.isValid()) {
            const QString raw = dv.toString();
            return raw.contains(needle, Qt::CaseInsensitive);
        }
        const QString fr = d.toString(QStringLiteral("dd/MM/yyyy"));
        if (fr.contains(needle, Qt::CaseInsensitive))
            return true;
        const QString iso = d.toString(Qt::ISODate);
        if (iso.contains(needle, Qt::CaseInsensitive))
            return true;
        QDate parsed = QDate::fromString(needle, QStringLiteral("dd/MM/yyyy"));
        if (parsed.isValid() && d == parsed)
            return true;
        parsed = QDate::fromString(needle, Qt::ISODate);
        if (parsed.isValid() && d == parsed)
            return true;
        parsed = QDate::fromString(needle, QStringLiteral("d/M/yyyy"));
        if (parsed.isValid() && d == parsed)
            return true;
        bool yOk = false;
        const int yOnly = needle.toInt(&yOk);
        if (yOk && needle.length() == 4 && yOnly >= 1900 && yOnly <= 2100 && d.year() == yOnly)
            return true;
        return false;
    }
    case 1: // Agriculteur (ID)
        return variantAsPlainIntString(q.value(2)).contains(needle, Qt::CaseInsensitive);
    case 2: { // Type d'huile (Oracle/ODBC peut renvoyer texte ou octets)
        const QVariant v = q.value(7);
        QString t = v.toString();
        if (t.isEmpty()) {
            const QByteArray ba = v.toByteArray();
            if (!ba.isEmpty())
                t = QString::fromUtf8(ba);
        }
        return t.trimmed().contains(needle, Qt::CaseInsensitive);
    }
    case 3: // Machine / employé
        return variantAsPlainIntString(q.value(3)).contains(needle, Qt::CaseInsensitive);
    default:
        return true;
    }
}

} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // Initialisation du manager réseau pour Ollama
    managerOllama = new QNetworkAccessManager(this);
    connect(managerOllama, &QNetworkAccessManager::finished, this, &MainWindow::onOllamaResponse);
    
    // Initialisation du manager pour envoyer des mails
    managerEmail = new QNetworkAccessManager(this);

    // Initialisation du manager et bouton pour IA Observation
    managerObservation = new QNetworkAccessManager(this);
    connect(managerObservation, &QNetworkAccessManager::finished, this, &MainWindow::onOllamaObservationResponse);

    ui->Observation->setGeometry(160, 492, 131, 31);
    QPushButton *btnGenererObs = new QPushButton("IA ✨", ui->InformationsPersonnelles_2);
    btnGenererObs->setGeometry(295, 492, 40, 31);
    btnGenererObs->setCursor(Qt::PointingHandCursor);
    btnGenererObs->setStyleSheet("QPushButton { background-color: #c78a10; color: white; font-weight: bold; border-radius: 6px; padding: 0px; } QPushButton:hover { background-color: #e09f1d; }");
    connect(btnGenererObs, &QPushButton::clicked, this, &MainWindow::genererObservationIntelligente);

    // -- Contraintes de longueur maximale (Caractères) pour tous les champs --
    ui->ID_Operation->setMaxLength(8);        // Limite à 8 chiffres max
    ui->ID_Agriculteur->setMaxLength(8);
    ui->ID_machin->setMaxLength(8);
    ui->Quantite_olives->setMaxLength(10);    // Ex: 9999999.99
    ui->Quantite_Huile->setMaxLength(10);
    ui->Rendement->setMaxLength(10);
    ui->Temperature->setMaxLength(6);         // Ex: 999.99
    ui->Duree_Pressage->setMaxLength(6);      // Ex: 999999
    ui->Observation->setMaxLength(255);       // Limite stricte de la base VARCHAR2(255)

    // Configurer le tableWidget avec TOUTES les 11 colonnes (incluant les nouvelles)
    ui->tableWidget->setColumnCount(11);
    QStringList headers;
    headers << QStringLiteral("ID op.") << QStringLiteral("Date") << QStringLiteral("Agri.")
            << QStringLiteral("Machine") << QStringLiteral("Qté olives") << QStringLiteral("Qté huile")
            << QStringLiteral("Rendement") << QStringLiteral("Type huile") << QStringLiteral("Temp.")
            << QStringLiteral("Durée") << QStringLiteral("Observation");
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    auto majPlaceholderRecherche = [this]() {
        switch (ui->comboBox_7->currentIndex()) {
        case 0:
            ui->rechercheEdit->setPlaceholderText(QString::fromUtf8("Ex. 15/04/2026, 2026, 04/2026…"));
            break;
        case 1:
            ui->rechercheEdit->setPlaceholderText(QString::fromUtf8("Partie du n° d'agriculteur…"));
            break;
        case 2:
            ui->rechercheEdit->setPlaceholderText(QString::fromUtf8("Ex. vierge, extra (sans tenir compte des majuscules)…"));
            break;
        case 3:
            ui->rechercheEdit->setPlaceholderText(QString::fromUtf8("Partie du n° machine / employé…"));
            break;
        default:
            break;
        }
    };
    connect(ui->comboBox_7, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            [this, majPlaceholderRecherche](int) {
                majPlaceholderRecherche();
                rafraichirGrilleProduction(ui->rechercheEdit->text().trimmed());
            });
    connect(ui->rechercheEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        rafraichirGrilleProduction(text.trimmed());
    });
    connect(ui->rechercheEdit, &QLineEdit::returnPressed, this, [this]() {
        rafraichirGrilleProduction(ui->rechercheEdit->text().trimmed());
    });
    connect(ui->pushButton_recherche, &QPushButton::clicked, this, [this]() {
        rafraichirGrilleProduction(ui->rechercheEdit->text().trimmed());
    });
    connect(ui->pushButton_effacer_filtre, &QPushButton::clicked, this, &MainWindow::effacerFiltreRecherche);
    // Rendre invisible le bouton de recherche (comme demandé)
    ui->pushButton_recherche->hide();
    ui->pushButton_effacer_filtre->raise();
    majPlaceholderRecherche();
    
    // --- Ajustements géométriques de l'interface (Agrandir la liste) ---
    // Repositionnement dynamique :
    QWidget *parentContainer = ui->tableWidget->parentWidget();
    if (parentContainer) {
        int containerHeight = parentContainer->height();
        int btnHeight = ui->rendement->height();
        int bottomMargin = 15;
        // Définir la nouvelle position Y des boutons tout en bas
        int targetY = containerHeight - btnHeight - bottomMargin;
        
        // Pousser les boutons vers le bas
        ui->rendement->move(ui->rendement->x(), targetY);
        ui->graphique->move(ui->graphique->x(), targetY);
        ui->btn_chatbot->move(ui->btn_chatbot->x(), targetY);
        
        // Agrandir le tableau pour qu'il occupe l'espace libéré (jusqu'aux boutons)
        int tableY = ui->tableWidget->y();
        ui->tableWidget->resize(ui->tableWidget->width(), targetY - tableY - 15);
    }

    // Raccourci pour envoyer un message via 'Entrée'
    connect(ui->chat_input, &QLineEdit::returnPressed, this, &MainWindow::on_btn_envoyer_chat_clicked);

    // Remplir et configurer le comboBox_8 pour le tri
    ui->comboBox_8->clear();
    ui->comboBox_8->addItem(QString::fromUtf8("Trier par : par défaut"));
    ui->comboBox_8->addItem(QString::fromUtf8("Rendement"));
    ui->comboBox_8->addItem(QString::fromUtf8("Quantité produite"));
    ui->comboBox_8->addItem(QString::fromUtf8("Date de production"));

    connect(ui->comboBox_8, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            [this](int) {
                afficherOperations();
            });

    afficherOperations();

    // Masquer le bouton Anomalies comme demandé
    ui->anomalies->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_rendement_clicked()
{
    const QString qoStr = ui->Quantite_olives->text().trimmed();
    const QString qhStr = ui->Quantite_Huile->text().trimmed();

    if (qoStr.isEmpty() || qhStr.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("Calcul du rendement"),
                             QStringLiteral("Indiquez la quantité d'olives et la quantité d'huile pour appliquer la formule :\n"
                                            "rendement (%) = (quantité d'huile ÷ quantité d'olives) × 100"));
        return;
    }

    QString qoNorm = qoStr;
    QString qhNorm = qhStr;
    qoNorm.replace(QLatin1Char(','), QLatin1Char('.'));
    qhNorm.replace(QLatin1Char(','), QLatin1Char('.'));
    bool okO = false;
    bool okH = false;
    const double qO = qoNorm.toDouble(&okO);
    const double qH = qhNorm.toDouble(&okH);
    if (!okO || !okH) {
        QMessageBox::warning(this, QStringLiteral("Calcul du rendement"),
                             QStringLiteral("Les quantités doivent être des nombres valides (ex. 1250 ou 12,5)."));
        return;
    }

    if (qO <= 0.0) {
        QMessageBox::warning(this, QStringLiteral("Calcul du rendement"),
                             QStringLiteral("La quantité d'olives doit être strictement positive."));
        return;
    }
    if (qH < 0.0) {
        QMessageBox::warning(this, QStringLiteral("Calcul du rendement"),
                             QStringLiteral("La quantité d'huile ne peut pas être négative."));
        return;
    }

    const double rendementPct = (qH / qO) * 100.0;
    ui->Rendement->setText(QString::number(rendementPct, 'f', 2));

    if (statusBar())
        statusBar()->showMessage(
            QStringLiteral("Rendement calculé : %1 % (huile ÷ olives × 100)")
                .arg(QString::number(rendementPct, 'f', 2)),
            6000);
}

void MainWindow::on_anomalies_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void MainWindow::on_graphique_clicked()
{
    // Construire dynamiquement l'interface des statistiques si elle n'existe pas encore
    if (!ui->page_2->layout()) {
        QVBoxLayout *layout = new QVBoxLayout(ui->page_2);
        
        QPushButton *btnRetour = new QPushButton("< Retour à la liste", ui->page_2);
        btnRetour->setCursor(Qt::PointingHandCursor);
        btnRetour->setStyleSheet("QPushButton { background-color: #c78a10; color: white; font-weight: bold; border-radius: 5px; padding: 10px; font-size: 14px; } QPushButton:hover { background-color: #a66f08; }");
        
        connect(btnRetour, &QPushButton::clicked, this, [this]() {
            ui->stackedWidget->setCurrentWidget(ui->page);
        });
        layout->addWidget(btnRetour);
        
        QTextBrowser *statsBrowser = new QTextBrowser(ui->page_2);
        statsBrowser->setObjectName("statsBrowser");
        // Utilisation des mêmes couleurs : blanc cassé pour le fond, bordure dorée
        statsBrowser->setStyleSheet("QTextBrowser { background-color: #fcfcfc; border: 2px solid #b8860b; border-radius: 10px; padding: 25px; font-size: 14pt; }");
        layout->addWidget(statsBrowser);
    }
    
    QTextBrowser *browser = ui->page_2->findChild<QTextBrowser *>("statsBrowser");
    if (browser) {
        Connection conn;
        // Vert classique pour le titre, ligne dorée
        QString html = "<h1 style='color: #2e7d32; text-align: center; font-family: Segoe UI, sans-serif; font-size: 26pt;'>Tableau de Bord - Statistiques</h1><hr style='background-color:#b8860b; height: 2px;'><br>";
        
        if (conn.createConnection()) {
            QSqlQuery q;
            
            q.exec("SELECT COUNT(*) FROM PRODUCTION");
            int totalOp = q.next() ? q.value(0).toInt() : 0;
            
            q.exec("SELECT SUM(QUANTITE_HUILE) FROM PRODUCTION");
            double totalHuile = q.next() ? q.value(0).toDouble() : 0.0;
            
            q.exec("SELECT AVG(RENDEMENT) FROM PRODUCTION");
            double avgRend = q.next() ? q.value(0).toDouble() : 0.0;
            
            html += QString("<p style='margin-bottom: 15px;'><b>&#128202; Total des opérations :</b> <span style='color:#c78a10; font-weight:bold;'>%1 opérations</span></p>").arg(totalOp);
            html += QString("<p style='margin-bottom: 15px;'><b>&#128167; Quantité d'huile totale :</b> <span style='color:#c78a10; font-weight:bold;'>%1 Litres</span></p>").arg(QString::number(totalHuile, 'f', 2));
            html += QString("<p style='margin-bottom: 15px;'><b>&#128200; Rendement Moyen Global :</b> <span style='color:#c78a10; font-weight:bold;'>%1 %</span></p><br>").arg(QString::number(avgRend, 'f', 2));
            
            q.exec("SELECT ID_EMP, AVG(RENDEMENT) as R FROM PRODUCTION GROUP BY ID_EMP ORDER BY R DESC");
            if (q.next()) {
                html += QString("<p style='margin-bottom: 15px;'><b>&#127942; Machine la plus performante :</b> Machine N° <span style='color:#c78a10; font-weight:bold;'>%1</span> (Rendement moyen : %2%)</p>")
                        .arg(q.value(0).toString())
                        .arg(QString::number(q.value(1).toDouble(), 'f', 2));
            }
            
            q.exec("SELECT TYPE_HUILE FROM PRODUCTION GROUP BY TYPE_HUILE ORDER BY COUNT(*) DESC");
            if (q.next()) {
                html += QString("<p style='margin-bottom: 15px;'><b>&#11088; Type d'huile le plus produit :</b> <span style='color:#c78a10; font-weight:bold;'>%1</span></p>")
                        .arg(q.value(0).toString());
            }
            
        } else {
            html += "<p style='color:red; text-align:center;'>Connexion à la base de données impossible pour charger les statistiques.</p>";
        }
        
        browser->setHtml(html);
    }

    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void MainWindow::on_ajouter_clicked()
{
    // Récupérer les valeurs des champs
    QString idOperation = ui->ID_Operation->text();
    QDate date = ui->Date->date();
    QString idAgriculteur = ui->ID_Agriculteur->text();
    QString idMachin = ui->ID_machin->text();
    QString quantiteOlives = ui->Quantite_olives->text();
    QString quantiteHuile = ui->Quantite_Huile->text();
    QString rendement = ui->Rendement->text();
    QString typeHuile = ui->Type_Huile->currentText();
    QString temperature = ui->Temperature->text();
    QString dureePressage = ui->Duree_Pressage->text();
    QString observation = ui->Observation->text();

    // --- VALIDATION INTELLIGENTE ---
    if (idOperation.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "L'identifiant de la production (ID) est obligatoire !");
        return;
    }
    int idOp = 0;
    if (!parsePositiveIntId(idOperation, &idOp)) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID de la production doit être un nombre entier positif valide.");
        return;
    }

    if (idAgriculteur.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "L'identifiant de l'agriculteur est obligatoire !");
        return;
    }
    int idAgri = 0;
    if (!parsePositiveIntId(idAgriculteur, &idAgri)) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID de l'agriculteur doit être un nombre entier positif.");
        return;
    }

    // ID MACHINE MANTENANT OBLIGATOIRE
    if (idMachin.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID Machine/Employé est obligatoire !");
        return;
    }
    int idEmp = 0;
    if (!parsePositiveIntId(idMachin, &idEmp)) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID Machine doit être un nombre entier positif.");
        return;
    }

    bool ok;
    if (quantiteOlives.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "La quantité d'olives est obligatoire !");
        return;
    }
    double qteOlives = quantiteOlives.toDouble(&ok);
    if (!ok || qteOlives <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "La quantité d'olives doit être un nombre décimal positif.");
        return;
    }

    if (quantiteHuile.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "La quantité d'huile est obligatoire !");
        return;
    }
    double qteHuile = quantiteHuile.toDouble(&ok);
    if (!ok || qteHuile < 0) {
        QMessageBox::warning(this, "Erreur de saisie", "La quantité d'huile doit être un nombre positif ou nul.");
        return;
    }

    if (rendement.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le rendement est obligatoire !");
        return;
    }
    double rendem = rendement.toDouble(&ok);
    if (!ok || rendem < 0) {
        QMessageBox::warning(this, "Erreur de saisie", "Le rendement doit être un nombre décimal positif ou nul.");
        return;
    }

    if (typeHuile.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le type d'huile est obligatoire !");
        return;
    }
    QString typeH = typeHuile.trimmed();

    if (temperature.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "La température moyenne est obligatoire !");
        return;
    }
    double tempMoy = temperature.toDouble(&ok);
    if (!ok || tempMoy <= 0 || tempMoy > 999.99) {
        QMessageBox::warning(this, "Erreur de saisie", "La température moyenne doit être comprise entre 0 et 999.99 (ex: 25.5).");
        return;
    }

    if (dureePressage.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "La durée de pressage est obligatoire !");
        return;
    }
    int duree = 0;
    if (!parsePositiveIntId(dureePressage, &duree)) {
        QMessageBox::warning(this, "Erreur de saisie", "La durée de pressage doit être un nombre entier positif (minutes).");
        return;
    }

    if (observation.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "L'observation est obligatoire !");
        return;
    }
    if (observation.length() > 255) {
        QMessageBox::warning(this, "Erreur de saisie", "L'observation est trop longue (maximum 255 caractères).");
        return;
    }

    // Connexion
    Connection conn;
    if (!conn.createConnection()) {
        QMessageBox::critical(this, "Erreur système", "Impossible de se connecter à la base de données Oracle !");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO PRODUCTION (ID_OPERATION, DATE_PRODUCTION, ID_AGRI, ID_EMP, "
                  "QUANTITE_OLIVES, QUANTITE_HUILE, RENDEMENT, TYPE_HUILE, "
                  "TEMPERATURE_MOYENNE, DUREE_PRESSAGE, OBSERVATION) "
                  "VALUES (:id_op, :date_op, :id_agri, :id_mach, :qte_olive, "
                  ":qte_huile, :rendement, :type_huile, :temp, :duree, :obs)");

    query.bindValue(":id_op", idOp);
    query.bindValue(":date_op", date);
    query.bindValue(":id_agri", idAgri);
    query.bindValue(":id_mach", idEmp); // Obligatoire
    query.bindValue(":qte_olive", qteOlives);
    query.bindValue(":qte_huile", qteHuile);

    if (rendement.isEmpty()) query.bindValue(":rendement", QVariant(QMetaType::fromType<double>()));
    else query.bindValue(":rendement", rendem);

    if (typeH.isEmpty()) query.bindValue(":type_huile", QVariant(QMetaType::fromType<QString>()));
    else query.bindValue(":type_huile", typeH);

    if (temperature.isEmpty()) query.bindValue(":temp", QVariant(QMetaType::fromType<double>()));
    else query.bindValue(":temp", tempMoy);

    if (dureePressage.isEmpty()) query.bindValue(":duree", QVariant(QMetaType::fromType<int>()));
    else query.bindValue(":duree", duree);

    if (observation.isEmpty()) query.bindValue(":obs", QVariant(QMetaType::fromType<QString>()));
    else query.bindValue(":obs", observation);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Production ajoutée avec succès !");
        
        // --- Vérification du seuil d'alerte Email ---
        if (!rendement.isEmpty() && rendem < 10.0) {
            envoyerAlerteEmail(rendem, QString::number(idOp));
        }

        afficherOperations();
        viderChamps();
    } else {
        QString errorMsg = query.lastError().text();
        if (errorMsg.contains("ORA-00001")) {
            QMessageBox::warning(this, "Erreur de doublon", "Cet identifiant ID_Operation existe déjà dans la base. Veuillez taper un nouveau numéro !");
        } else if (errorMsg.contains("ORA-01438")) {
            QMessageBox::warning(this, "Valeur trop grande", "Chiffre trop grand ! La base de données refuse ce nombre (ex: Température > 999.99). Réduisez la taille de vos nombres.");
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout :\n" + errorMsg);
        }
    }
}

void MainWindow::on_afficher_clicked()
{
    afficherOperations();
}

void MainWindow::on_suprimer_clicked()
{
    QString idOperation = ui->ID_Operation->text();

    if (idOperation.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une production à supprimer !");
        return;
    }

    int idOpDel = 0;
    if (!parsePositiveIntId(idOperation, &idOpDel)) {
        QMessageBox::warning(this, "Erreur", "ID d'opération invalide. Sélectionnez une ligne dans le tableau ou saisissez un nombre entier.");
        return;
    }

    Connection conn;
    if (!conn.createConnection()) {
        QMessageBox::critical(this, "Erreur", "Impossible de se connecter à la base de données !");
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM PRODUCTION WHERE ID_OPERATION = :id_op");
    query.bindValue(":id_op", idOpDel);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Production supprimée avec succès !");
        afficherOperations();
        viderChamps();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression :\n" + query.lastError().text());
    }
}

void MainWindow::on_modifier_clicked()
{
    QString idOperation = ui->ID_Operation->text();
    QDate date = ui->Date->date();
    QString idAgriculteur = ui->ID_Agriculteur->text();
    QString idMachin = ui->ID_machin->text();
    QString quantiteOlives = ui->Quantite_olives->text();
    QString quantiteHuile = ui->Quantite_Huile->text();
    QString rendement = ui->Rendement->text();
    QString typeHuile = ui->Type_Huile->currentText();
    QString temperature = ui->Temperature->text();
    QString dureePressage = ui->Duree_Pressage->text();
    QString observation = ui->Observation->text();

    if (idOperation.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Veuillez sélectionner une production à modifier (ID requis) !");
        return;
    }
    int idOp = 0;
    if (!parsePositiveIntId(idOperation, &idOp)) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID de la production doit être un entier positif.");
        return;
    }

    if (idAgriculteur.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "L'identifiant de l'agriculteur est obligatoire !");
        return;
    }
    int idAgri = 0;
    if (!parsePositiveIntId(idAgriculteur, &idAgri)) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID de l'agriculteur doit être un entier positif.");
        return;
    }

    if (idMachin.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID Machine/Employé est obligatoire !");
        return;
    }
    int idEmp = 0;
    if (!parsePositiveIntId(idMachin, &idEmp)) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID Machine doit être un entier positif.");
        return;
    }

    bool ok;
    if (quantiteOlives.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "La quantité d'olives est obligatoire !");
        return;
    }
    double qteOlives = quantiteOlives.toDouble(&ok);
    if (!ok || qteOlives <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "La quantité d'olives n'est pas valide.");
        return;
    }

    if (quantiteHuile.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "La quantité d'huile est obligatoire !");
        return;
    }
    double qteHuile = quantiteHuile.toDouble(&ok);
    if (!ok || qteHuile < 0) {
        QMessageBox::warning(this, "Erreur de saisie", "La quantité d'huile n'est pas valide.");
        return;
    }

    if (rendement.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le rendement est obligatoire !");
        return;
    }
    double rendem = rendement.toDouble(&ok);
    if (!ok || rendem < 0) {
        QMessageBox::warning(this, "Erreur de saisie", "Le rendement doit être un nombre décimal positif ou nul.");
        return;
    }

    if (typeHuile.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le type d'huile est obligatoire !");
        return;
    }
    QString typeH = typeHuile.trimmed();

    if (temperature.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "La température moyenne est obligatoire !");
        return;
    }
    double tempMoy = temperature.toDouble(&ok);
    if (!ok || tempMoy <= 0 || tempMoy > 999.99) {
        QMessageBox::warning(this, "Erreur de saisie", "La température moyenne doit être comprise entre 0 et 999.99.");
        return;
    }

    if (dureePressage.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "La durée de pressage est obligatoire !");
        return;
    }
    int duree = 0;
    if (!parsePositiveIntId(dureePressage, &duree)) {
        QMessageBox::warning(this, "Erreur de saisie", "La durée de pressage doit être un entier > 0.");
        return;
    }

    if (observation.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "L'observation est obligatoire !");
        return;
    }
    if (observation.length() > 255) {
        QMessageBox::warning(this, "Erreur de saisie", "L'observation est trop longue (maximum 255 caractères).");
        return;
    }

    Connection conn;
    if (!conn.createConnection()) {
        QMessageBox::critical(this, "Erreur", "Impossible de se connecter à la base de données !");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE PRODUCTION SET DATE_PRODUCTION = :date_op, ID_AGRI = :id_agri, ID_EMP = :id_mach, "
                  "QUANTITE_OLIVES = :qte_olive, QUANTITE_HUILE = :qte_huile, RENDEMENT = :rendement, "
                  "TYPE_HUILE = :type_huile, TEMPERATURE_MOYENNE = :temp, DUREE_PRESSAGE = :duree, OBSERVATION = :obs "
                  "WHERE ID_OPERATION = :id_op");

    query.bindValue(":id_op", idOp);
    query.bindValue(":date_op", date);
    query.bindValue(":id_agri", idAgri);
    query.bindValue(":id_mach", idEmp);
    query.bindValue(":qte_olive", qteOlives);
    query.bindValue(":qte_huile", qteHuile);

    if (rendement.isEmpty()) query.bindValue(":rendement", QVariant(QMetaType::fromType<double>()));
    else query.bindValue(":rendement", rendem);

    if (typeH.isEmpty()) query.bindValue(":type_huile", QVariant(QMetaType::fromType<QString>()));
    else query.bindValue(":type_huile", typeH);

    if (temperature.isEmpty()) query.bindValue(":temp", QVariant(QMetaType::fromType<double>()));
    else query.bindValue(":temp", tempMoy);

    if (dureePressage.isEmpty()) query.bindValue(":duree", QVariant(QMetaType::fromType<int>()));
    else query.bindValue(":duree", duree);

    if (observation.isEmpty()) query.bindValue(":obs", QVariant(QMetaType::fromType<QString>()));
    else query.bindValue(":obs", observation);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Production modifiée avec succès !");
        
        // --- Vérification du seuil d'alerte Email ---
        if (!rendement.isEmpty() && rendem < 10.0) {
            envoyerAlerteEmail(rendem, QString::number(idOp));
        }
        
        afficherOperations();
        viderChamps();
    } else {
        QString errorMsg = query.lastError().text();
        if (errorMsg.contains("ORA-01438")) {
            QMessageBox::warning(this, "Valeur trop grande", "Chiffre trop grand ! La base de données refuse ce nombre. Réduisez la taille de vos nombres.");
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la modification :\n" + errorMsg);
        }
    }
}

void MainWindow::on_tableWidget_cellClicked(int row, int /*column*/)
{
    int idOpCell = 0;
    const QString idOpStr = ui->tableWidget->item(row, 0)->text();
    if (parsePositiveIntId(idOpStr, &idOpCell))
        ui->ID_Operation->setText(QString::number(idOpCell));
    else
        ui->ID_Operation->setText(idOpStr);
    ui->Date->setDate(QDate::fromString(ui->tableWidget->item(row, 1)->text(), "dd/MM/yyyy"));
    int idA = 0, idM = 0;
    const QString idAStr = ui->tableWidget->item(row, 2)->text();
    const QString idMStr = ui->tableWidget->item(row, 3)->text();
    ui->ID_Agriculteur->setText(parsePositiveIntId(idAStr, &idA) ? QString::number(idA) : idAStr);
    ui->ID_machin->setText(parsePositiveIntId(idMStr, &idM) ? QString::number(idM) : idMStr);
    ui->Quantite_olives->setText(ui->tableWidget->item(row, 4)->text());
    ui->Quantite_Huile->setText(ui->tableWidget->item(row, 5)->text());
    ui->Rendement->setText(ui->tableWidget->item(row, 6)->text());
    
    // Le type d'huile
    QString th = ui->tableWidget->item(row, 7)->text();
    if(th.isEmpty()) ui->Type_Huile->setCurrentIndex(0); // remettre par défaut si vide
    else ui->Type_Huile->setCurrentText(th);
    
    // Nouveaux champs inclus  
    ui->Temperature->setText(ui->tableWidget->item(row, 8)->text());
    ui->Duree_Pressage->setText(ui->tableWidget->item(row, 9)->text());
    ui->Observation->setText(ui->tableWidget->item(row, 10)->text());
}

void MainWindow::effacerFiltreRecherche()
{
    ui->rechercheEdit->clear();
}

void MainWindow::afficherOperations()
{
    rafraichirGrilleProduction(ui->rechercheEdit->text().trimmed());
}

void MainWindow::rafraichirGrilleProduction(const QString &needle)
{
    Connection conn;
    if (!conn.createConnection()) {
        return;
    }

    QSqlDatabase db = QSqlDatabase::database(QStringLiteral("qt_sql_default_connection"));
    if (!db.isValid() || !db.isOpen()) {
        QMessageBox::warning(this, QStringLiteral("Données"),
                             QStringLiteral("La connexion à la base n'est pas disponible."));
        return;
    }

    int critere = ui->comboBox_7->currentIndex();
    if (critere < 0 || critere > 3)
        critere = 0;

    int triCritere = ui->comboBox_8->currentIndex();
    QString orderBy = QStringLiteral("ORDER BY DATE_PRODUCTION DESC"); // Default
    if (triCritere == 1) {
        orderBy = QStringLiteral("ORDER BY RENDEMENT DESC");
    } else if (triCritere == 2) {
        orderBy = QStringLiteral("ORDER BY QUANTITE_HUILE DESC");
    } else if (triCritere == 3) {
        orderBy = QStringLiteral("ORDER BY DATE_PRODUCTION DESC");
    }

    const QString sql = QStringLiteral(
        "SELECT ID_OPERATION, DATE_PRODUCTION, ID_AGRI, ID_EMP, "
        "QUANTITE_OLIVES, QUANTITE_HUILE, RENDEMENT, TYPE_HUILE, "
        "TEMPERATURE_MOYENNE, DUREE_PRESSAGE, OBSERVATION "
        "FROM PRODUCTION ") + orderBy;

    QSqlQuery query(db);
    if (!query.exec(sql)) {
        QMessageBox::warning(this, QStringLiteral("Données"),
                             QStringLiteral("Impossible de charger les productions :\n%1")
                                 .arg(query.lastError().text()));
        return;
    }

    ui->tableWidget->setRowCount(0);

    int row = 0;
    while (query.next()) {
        if (!ligneCorrespondAuFiltre(critere, needle, query))
            continue;

        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(variantAsPlainIntString(query.value(0))));
        {
            const QDate dCell = variantToSqlDate(query.value(1));
            const QString dateAff = dCell.isValid()
                ? dCell.toString(QStringLiteral("dd/MM/yyyy"))
                : query.value(1).toString();
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(dateAff));
        }
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(variantAsPlainIntString(query.value(2))));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(variantAsPlainIntString(query.value(3))));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(query.value(6).toString()));
        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(query.value(7).toString()));
        ui->tableWidget->setItem(row, 8, new QTableWidgetItem(query.value(8).toString()));
        ui->tableWidget->setItem(row, 9, new QTableWidgetItem(variantAsPlainIntString(query.value(9))));
        ui->tableWidget->setItem(row, 10, new QTableWidgetItem(query.value(10).toString()));

        row++;
    }
}

void MainWindow::viderChamps()
{
    ui->ID_Operation->clear();
    ui->Date->setDate(QDate::currentDate());
    ui->ID_Agriculteur->clear();
    ui->ID_machin->clear();
    ui->Quantite_olives->clear();
    ui->Quantite_Huile->clear();
    ui->Rendement->clear();
    ui->Type_Huile->setCurrentIndex(0);
    ui->Temperature->clear();
    ui->Duree_Pressage->clear();
    ui->Observation->clear();
}

void MainWindow::on_pushButton_5_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", QString(), "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty())
        return;

    if (QFileInfo(fileName).suffix().isEmpty())
        fileName.append(".pdf");

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QString html = "<h1 style='text-align: center; color: #4a5c25;'>Liste des Productions</h1>";
    html += "<table border='1' cellspacing='0' cellpadding='4' width='100%'>";
    html += "<tr style='background-color: #d6b25e;'>";
    for (int i = 0; i < ui->tableWidget->columnCount(); ++i) {
        html += "<th>" + ui->tableWidget->horizontalHeaderItem(i)->text() + "</th>";
    }
    html += "</tr>";

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        html += "<tr>";
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            QString text = item ? item->text() : "";
            html += "<td>" + text + "</td>";
        }
        html += "</tr>";
    }
    html += "</table>";

    QTextDocument document;
    document.setHtml(html);
    document.print(&printer);

    QMessageBox::information(this, "Succès", "Données exportées en PDF avec succès !");
}

void MainWindow::on_btn_chatbot_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_chatbot);
}

void MainWindow::on_btn_retour_chat_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
}

void MainWindow::on_btn_envoyer_chat_clicked()
{
    QString q = ui->chat_input->text().trimmed();
    if(q.isEmpty()) return;
    
    // Afficher la question en gras
    ui->chat_history->append("<b>Vous:</b> " + q);
    ui->chat_input->clear();
    
    // Traiter la question et générer la réponse
    traiterQuestionChatbot(q);
}

void MainWindow::traiterQuestionChatbot(const QString &question)
{
    // Afficher l'état de "réflexion"
    ui->chat_history->append("<span style='color:gray;'><i>Bot réfléchit...</i></span><br>");

    // 1. Récupération du Contexte depuis la BD (Extraction des données récentes & statistiques)
    QString contextData = "Voici les statistiques actuelles de l'application SmartOilPress :\n";
    Connection conn;
    if (conn.createConnection()) {
        QSqlQuery query;
        // Obtenir le rendement global
        if (query.exec("SELECT AVG(RENDEMENT) FROM PRODUCTION") && query.next()) {
            double avgGlobal = query.value(0).toDouble();
            contextData += QString("- Rendement moyen global: %1%\n").arg(avgGlobal, 0, 'f', 2);
        }
        
        // Obtenir la meilleure huile
        if (query.exec("SELECT TYPE_HUILE, AVG(RENDEMENT) as R FROM PRODUCTION GROUP BY TYPE_HUILE ORDER BY R DESC") && query.next()) {
            contextData += QString("- Type d'huile le plus rentable: %1 (Rendement moyen: %2%)\n")
                               .arg(query.value(0).toString())
                               .arg(query.value(1).toDouble(), 0, 'f', 2);
        }
        
        // Obtenir les dernières observations fréquentes
        if (query.exec("SELECT OBSERVATION FROM PRODUCTION WHERE OBSERVATION IS NOT NULL ORDER BY DATE_PRODUCTION DESC")) {
            contextData += "- Observations et problèmes récents :\n";
            int count = 0;
            while (query.next() && count < 3) {
                QString obs = query.value(0).toString().trimmed();
                if(!obs.isEmpty()) {
                    contextData += "  * " + obs + "\n";
                    count++;
                }
            }
        }
    } else {
        contextData += "(Attention: Base de données actuellement inaccessible, pas de contexte récent.)\n";
    }

    // 2. Préparation du Prompt "RAG" combiné pour Ollama
    QString systemPrompt = "Tu es un assistant IA professionnel et utile intégré au logiciel SmartOilPress (une application de gestion de production d'huile d'olive). "
                           "Réponds de manière claire, concise et toujours en français. "
                           "Utilise exclusivement le CONTEXTE fourni ci-dessous pour répondre aux questions sur la production. "
                           "Si la réponse n'est pas dans le contexte, dis simplement que tu n'as pas l'information.\n\n"
                           "----- CONTEXTE -----\n" + contextData + "\n--------------------\n";

    QString fullPrompt = systemPrompt + "\nQUESTION DE L'AGRILCULTEUR: " + question;

    // 3. Préparation de la requête HTTP JSON pour Ollama (model: llama3)
    // IMPORTANT: L'utilisateur peut devoir changer 'llama3' en 'mistral' ci-dessous s'il installe un autre modèle
    QJsonObject jsonBody;
    jsonBody["model"] = "llama3";
    jsonBody["prompt"] = fullPrompt;
    jsonBody["stream"] = false; // Désactiver le stream pour récupérer la réponse en un seul bloc (plus simple)

    QJsonDocument jsonDoc(jsonBody);
    QByteArray data = jsonDoc.toJson();

    QUrl url("http://localhost:11434/api/generate");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Envoi de la requête POST
    managerOllama->post(request, data);
}

void MainWindow::onOllamaResponse(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        // Lecture du JSON
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();
        
        QString reponseIA = jsonObj.value("response").toString();
        
        // Formatter et afficher la réponse dans le chat history
        // Remplacer les retours à la ligne par des balises HTML <br>
        reponseIA.replace("\n", "<br>");
        
        ui->chat_history->append("<span style='color:#2e7d32;'><b>Bot IA (Ollama) :</b><br> " + reponseIA + "</span><br>");
    } else {
        QString errorMsg = reply->errorString();
        ui->chat_history->append("<span style='color:red;'><b>Bot:</b> L'IA n'arrive pas à répondre. Assurez-vous que l'application Ollama est ouverte en arrière plan et que le modèle 'llama3' est bien installé. (Erreur réseau: " + errorMsg + ")</span><br>");
    }
    
    reply->deleteLater();
}

void MainWindow::envoyerAlerteEmail(double rendement, const QString &idOperation)
{
    // L'URL secrète que l'utilisateur va générer (Il doit remplacer MON_CODE par son code formspree)
    QUrl url("https://formspree.io/f/xeevaprb");
    
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["email"] = "ahmedaminemokni0@gmail.com";
    json["subject"] = "🚨 ALERTE URGENCE : Rendement Critique (" + QString::number(rendement, 'f', 2) + "%)";
    json["message"] = QString("Bonjour Ahmed Amine,\n\nUne alerte de production vient de se déclencher.\n\n"
                              "Détails :\n"
                              "- ID Opération : %1\n"
                              "- Rendement calculé : %2%\n\n"
                              "Veuillez vérifier la machine et la qualité des olives immédiatement.\n\n"
                              "Ceci est un message automatique de SmartOilPress.")
                              .arg(idOperation)
                              .arg(rendement);

    QJsonDocument doc(json);
    
    // Envoi de la requête POST (Formspree transmettra l'e-mail instantanément)
    QNetworkReply *reply = managerEmail->post(request, doc.toJson());
    
    // Vérification en cas d'erreur de réseau (Souvent SSL manquant sur Windows)
    connect(reply, &QNetworkReply::errorOccurred, this, [this, reply](QNetworkReply::NetworkError) {
        QMessageBox::critical(this, "Erreur d'envoi d'alerte", "Impossible d'envoyer l'alerte email.\nErreur: " + reply->errorString() + "\nVeuillez vérifier votre connexion internet ou l'installation de OpenSSL.");
    });
    
    // Vérification de réussite
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(this, "Alerte envoyée", "Un email d'alerte a bien été envoyé !");
        }
        reply->deleteLater();
    });
}

void MainWindow::genererObservationIntelligente()
{
    QString typeHuile = ui->Type_Huile->currentText();
    QString rendement = ui->Rendement->text();
    QString temperature = ui->Temperature->text();
    QString duree = ui->Duree_Pressage->text();

    if(rendement.isEmpty() || temperature.isEmpty() || duree.isEmpty()) {
        QMessageBox::information(this, "Données manquantes", "Veuillez remplir au moins le rendement, la température et la durée avant de demander l'avis de l'IA.");
        return;
    }

    ui->Observation->setText("Analyse IA en cours...");

    QString prompt = QString("Tu es un expert agronome. En te basant sur un rendement de %1%, une température de %2°C, et une durée de %3 min pour de l'huile %4, donne-moi UNIQUEMENT une très courte phrase d'observation (maximum 15 mots) professionnelle. Ne fais aucune introduction, ne mets pas de guillemets.")
                        .arg(rendement)
                        .arg(temperature)
                        .arg(duree)
                        .arg(typeHuile);

    QJsonObject jsonBody;
    jsonBody["model"] = "llama3";
    jsonBody["prompt"] = prompt;
    jsonBody["stream"] = false;

    QJsonDocument jsonDoc(jsonBody);
    QByteArray data = jsonDoc.toJson();

    QUrl url("http://localhost:11434/api/generate");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    managerObservation->post(request, data);
}

void MainWindow::onOllamaObservationResponse(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();
        
        QString reponseIA = jsonObj.value("response").toString().trimmed();
        
        if (reponseIA.startsWith('"')) {
            reponseIA = reponseIA.mid(1);
        }
        if (reponseIA.endsWith('"')) {
            reponseIA.chop(1);
        }
        
        ui->Observation->setText(reponseIA.trimmed());
    } else {
        ui->Observation->setText("Erreur réseau");
        QMessageBox::warning(this, "Erreur IA", "Impossible de joindre Ollama: " + reply->errorString());
    }
    
    reply->deleteLater();
}

