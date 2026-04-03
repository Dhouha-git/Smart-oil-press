#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"

#include <QComboBox>
#include <QDateTime>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlDatabase>
#include <QTableWidgetItem>

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
    ui->pushButton_recherche->raise();
    ui->pushButton_effacer_filtre->raise();
    majPlaceholderRecherche();

    afficherOperations();
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

    const QString sql = QStringLiteral(
        "SELECT ID_OPERATION, DATE_PRODUCTION, ID_AGRI, ID_EMP, "
        "QUANTITE_OLIVES, QUANTITE_HUILE, RENDEMENT, TYPE_HUILE, "
        "TEMPERATURE_MOYENNE, DUREE_PRESSAGE, OBSERVATION "
        "FROM PRODUCTION ORDER BY DATE_PRODUCTION DESC");

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
