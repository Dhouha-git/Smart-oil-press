#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QString>
#include <QItemSelectionModel>
#include <QDebug>

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRandomGenerator>
#include <QProcess>
#include <QSqlQuery>

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QAbstractItemModel>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QScrollBar>
#include <QStandardPaths>
#include <QProcess>
#include <QDir>
#include <QInputDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QDesktopServices>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>
#include <QDate>
#include <numeric>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->login);
     refreshTable();
    refreshTableClient();
     ui->recherche2->clear();
     ui->particulier_6->setChecked(false);
     ui->profisionnel_6->setChecked(false);
     on_recherche2_2_clicked();
     on_appliquer3_2_clicked();
     ui->particulier_5->setChecked(false);
     ui->profisionnel_4->setChecked(false);     // table "clients fidèles"
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

    connect(ui->executer, &QPushButton::clicked,
            this, &MainWindow::afficherCourbeDansTable2);

    // ── Ventes : Recherche, Tri, Exports, Analyses ────────────────────────────
    connect(ui->vente_rechercheClientBtn,  &QPushButton::clicked,
            this, &MainWindow::on_vente_rechercheClientBtn_clicked);
    connect(ui->vente_rechercheDateBtn,    &QPushButton::clicked,
            this, &MainWindow::on_vente_rechercheDateBtn_clicked);
    connect(ui->vente_triBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_vente_triBox_currentIndexChanged);
    connect(ui->vente_exportCSVBtn,        &QPushButton::clicked,
            this, &MainWindow::on_vente_exportCSVBtn_clicked);
    connect(ui->vente_exportPDFBtn,        &QPushButton::clicked,
            this, &MainWindow::on_vente_exportPDFBtn_clicked);
    connect(ui->pushButton_12,             &QPushButton::clicked,
            this, &MainWindow::on_listerVentes);
    connect(ui->vente_analyserCABtn,       &QPushButton::clicked,
            this, &MainWindow::on_vente_analyserCABtn_clicked);
    connect(ui->vente_detecterAnomaliesBtn,&QPushButton::clicked,
            this, &MainWindow::on_vente_detecterAnomaliesBtn_clicked);
    connect(ui->vente_previsionBtn,        &QPushButton::clicked,
            this, &MainWindow::on_vente_previsionBtn_clicked);
    connect(ui->vente_graphiqueCABtn,      &QPushButton::clicked,
            this, &MainWindow::on_vente_graphiqueCABtn_clicked);

    // ── Arduino ──────────────────────────────────────────────────
    m_arduino = new Arduino(this);

    connect(m_arduino, &Arduino::carteDetectee,
            this,      &MainWindow::onCarteDetectee);
    connect(m_arduino, &Arduino::employeIdentifie,
            this,      &MainWindow::onEmployeIdentifie);
    connect(m_arduino, &Arduino::employeInconnu,
            this,      &MainWindow::onEmployeInconnu);
    connect(m_arduino, &Arduino::messageRecu,
            this,      &MainWindow::onMessageArduino);

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

void MainWindow::on_btn_pdf_employes_clicked()
{
    // 1. Choisir où sauvegarder
    QString chemin = QFileDialog::getSaveFileName(
        this,
        "Exporter les employés en PDF",
        QDir::homePath() + "/employes_" +
            QDateTime::currentDateTime().toString("yyyyMMdd_HHmm") + ".pdf",
        "PDF (*.pdf)"
        );
    if (chemin.isEmpty()) return;

    // 2. Configurer le PDF
    // ✅ CORRIGÉ : ScreenResolution au lieu de HighResolution
    //    HighResolution = 1200 DPI → tout devient minuscule
    //    ScreenResolution = 96 DPI  → tailles normales et lisibles
    QPrinter printer(QPrinter::ScreenResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(chemin);
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setPageSize(QPageSize::A4);
    printer.setPageMargins(QMarginsF(10, 10, 10, 10), QPageLayout::Millimeter);

    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le PDF !");
        return;
    }

    // 3. Dimensions réelles de la page
    QRect page = painter.viewport();
    int W = page.width();
    int H = page.height();

    // ✅ Marges latérales fixes en pixels
    int margeG = 20;
    int margeD = 20;
    int totalW = W - margeG - margeD;

    // 4. Couleurs
    QColor vertFonce(85, 107, 47);
    QColor orCouleur(212, 175, 55);
    QColor blanc(255, 255, 255);
    QColor grisClair(240, 244, 235);
    QColor texteSombre(40, 40, 40);
    QColor bordureGrise(180, 180, 180);

    // ══════════════════════════════════════════
    // 5. EN-TÊTE
    // ══════════════════════════════════════════
    int hauteurHeader = 80;
    painter.fillRect(0, 0, W, hauteurHeader, vertFonce);

    // Titre
    painter.setPen(blanc);
    painter.setFont(QFont("Arial", 20, QFont::Bold));
    painter.drawText(QRect(0, 8, W, 35), Qt::AlignCenter, "Smart Oil Press");

    // Sous-titre
    painter.setFont(QFont("Arial", 11));
    painter.drawText(QRect(0, 40, W, 25), Qt::AlignCenter, "Liste des Employes");

    // Date
    painter.setFont(QFont("Arial", 8));
    painter.drawText(QRect(margeG, 60, totalW, 18),
                     Qt::AlignRight,
                     "Genere le : " + QDateTime::currentDateTime().toString("dd/MM/yyyy a HH:mm"));

    // Ligne dorée sous l'en-tête
    painter.setPen(QPen(orCouleur, 3));
    painter.drawLine(0, hauteurHeader, W, hauteurHeader);

    int y = hauteurHeader + 10;

    // ══════════════════════════════════════════
    // 6. COLONNES
    // ══════════════════════════════════════════
    QStringList headers = {
        "ID", "CIN", "Nom", "Prenom",
        "Departement", "Poste", "Contrat",
        "Salaire", "Primes", "Statut"
    };

    // ✅ Largeurs proportionnelles bien réparties
    QList<double> ratios = { 0.05, 0.09, 0.10, 0.10, 0.13, 0.14, 0.09, 0.10, 0.09, 0.11 };
    QList<int> colW;
    for (double r : ratios)
        colW << int(totalW * r);

    // ✅ Hauteur de ligne généreuse
    int rowH = 28;

    // ══════════════════════════════════════════
    // 7. FONCTION LAMBDA : dessiner en-tête tableau
    // ══════════════════════════════════════════
    auto dessinerEnteteTableau = [&]() {
        painter.fillRect(margeG, y, totalW, rowH, vertFonce);
        painter.setFont(QFont("Arial", 9, QFont::Bold));
        int xCol = margeG;
        for (int i = 0; i < headers.size(); i++) {
            // Séparateur vertical blanc
            if (i > 0) {
                painter.setPen(QPen(QColor(255,255,255,80), 1));
                painter.drawLine(xCol, y, xCol, y + rowH);
            }
            painter.setPen(blanc);
            painter.drawText(
                QRect(xCol + 4, y, colW[i] - 8, rowH),
                Qt::AlignVCenter | Qt::AlignHCenter,
                headers[i]);
            xCol += colW[i];
        }
    };

    dessinerEnteteTableau();
    y += rowH;

    // ══════════════════════════════════════════
    // 8. DONNÉES DEPUIS LA BD
    // ══════════════════════════════════════════
    QSqlDatabase db = Connection::getInstance()->getDatabase();
    QSqlQuery query(db);
    query.exec(
        "SELECT ID_EMP, CIN, NOM, PRENOM, DEPARTEMENT, "
        "POSTE, TYPE_CONTRAT, SALAIRE, PRIMES, STATUT "
        "FROM EMPLOYE ORDER BY ID_EMP"
        );

    int nbLigne = 0;
    // ✅ Lignes par page calculées dynamiquement
    int lignesParPage = (H - hauteurHeader - 10 - 40) / rowH - 1;

    while (query.next()) {

        // ── Nouvelle page si besoin ──────────────
        if (nbLigne > 0 && nbLigne % lignesParPage == 0) {
            printer.newPage();

            // En-tête simplifié sur les pages suivantes
            painter.fillRect(0, 0, W, 35, vertFonce);
            painter.setPen(blanc);
            painter.setFont(QFont("Arial", 11, QFont::Bold));
            painter.drawText(QRect(0, 0, W, 35), Qt::AlignCenter, "Smart Oil Press — Liste des Employes (suite)");
            painter.setPen(QPen(orCouleur, 2));
            painter.drawLine(0, 35, W, 35);

            y = 45;
            dessinerEnteteTableau();
            y += rowH;
            painter.setFont(QFont("Arial", 8));
        }

        // ── Fond alterné ─────────────────────────
        QColor bg = (nbLigne % 2 == 0) ? blanc : grisClair;
        painter.fillRect(margeG, y, totalW, rowH, bg);

        // ── Valeurs ──────────────────────────────
        QString statut = query.value(9).toString();
        QStringList vals;
        vals << query.value(0).toString()
             << query.value(1).toString()
             << query.value(2).toString()
             << query.value(3).toString()
             << query.value(4).toString()
             << query.value(5).toString()
             << query.value(6).toString()
             << QString::number(query.value(7).toDouble(), 'f', 0) + " DT"
             << QString::number(query.value(8).toDouble(), 'f', 0) + " DT"
             << statut;

        int xCol = margeG;
        for (int i = 0; i < vals.size(); i++) {

            // Séparateur vertical
            painter.setPen(QPen(bordureGrise, 1));
            if (i > 0)
                painter.drawLine(xCol, y, xCol, y + rowH);

            // Couleur texte selon colonne
            if (i == 9) {
                // Statut coloré
                QString s = statut.toLower();
                QColor couleurStatut;
                if      (s == "actif")        couleurStatut = QColor(34, 120, 34);
                else if (s.contains("cong"))  couleurStatut = QColor(200, 100, 0);
                else                          couleurStatut = QColor(200, 20, 20);
                painter.setPen(couleurStatut);
                painter.setFont(QFont("Arial", 8, QFont::Bold));
            } else if (i == 0) {
                // ID en gras
                painter.setPen(vertFonce);
                painter.setFont(QFont("Arial", 8, QFont::Bold));
            } else {
                painter.setPen(texteSombre);
                painter.setFont(QFont("Arial", 8));
            }

            painter.drawText(
                QRect(xCol + 4, y, colW[i] - 8, rowH),
                Qt::AlignVCenter | Qt::AlignHCenter,
                vals[i]);

            xCol += colW[i];
        }

        // Ligne horizontale du bas de chaque ligne
        painter.setPen(QPen(bordureGrise, 1));
        painter.drawLine(margeG, y + rowH, margeG + totalW, y + rowH);

        y += rowH;
        nbLigne++;
    }

    // Bordure extérieure du tableau

    painter.setPen(QPen(vertFonce, 2));
    painter.drawRect(margeG, hauteurHeader + 10,
                     totalW, y - hauteurHeader - 10);

    // ══════════════════════════════════════════
    // 9. PIED DE PAGE
    // ══════════════════════════════════════════
    painter.fillRect(0, H - 35, W, 35, grisClair);
    painter.setPen(QPen(orCouleur, 2));
    painter.drawLine(0, H - 35, W, H - 35);

    painter.setPen(texteSombre);
    painter.setFont(QFont("Arial", 8));
    painter.drawText(
        QRect(margeG, H - 30, totalW, 25),
        Qt::AlignCenter,
        QString("Smart Oil Press  |  Total : %1 employe(s)  |  "
                + QDateTime::currentDateTime().toString("dd/MM/yyyy")).arg(nbLigne)
        );

    painter.end();

    QMessageBox::information(
        this, "Succes",
        QString("PDF genere avec succes !\n%1 employe(s) exporte(s).\n\nFichier :\n%2")
            .arg(nbLigne).arg(chemin));
}

void MainWindow::exporterTableViewExcel(QAbstractItemModel* model, const QString& titre)
{
    if (!model) {
        QMessageBox::warning(this, "Export Excel", "Aucune donnée à exporter.");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Exporter vers Excel",
        QDir::homePath() + "/" + titre + ".xls",
        "Fichier Excel (*.xls)"
        );

    if (filePath.isEmpty()) return;

    // Génération du fichier XLSX via SpreadsheetML (XML)
    // Structure minimale d'un .xlsx : c'est un ZIP contenant des XML
    // On utilise QZipWriter si disponible, sinon on génère un CSV compatible Excel

    // Option simple : générer un fichier CSV avec extension .csv
    // (renommer en .xlsx pour compatibilité ou utiliser la méthode XML ci-dessous)

    // ---- Méthode XML SpreadsheetML (s'ouvre directement dans Excel) ----
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le fichier.");
        return;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    // En-tête XML Excel
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << "<?mso-application progid=\"Excel.Sheet\"?>\n";
    out << "<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"\n";
    out << " xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\">\n";
    out << "<Worksheet ss:Name=\"" << titre << "\">\n";
    out << "<Table>\n";

    // Style en-tête (gras)
    out << "<Row>\n";
    for (int col = 0; col < model->columnCount(); ++col) {
        QString header = model->headerData(col, Qt::Horizontal).toString()
        .replace("&","&amp;").replace("<","&lt;").replace(">","&gt;");
        out << "<Cell><Data ss:Type=\"String\"><b>" << header << "</b></Data></Cell>\n";
    }
    out << "</Row>\n";

    // Données
    for (int row = 0; row < model->rowCount(); ++row) {
        out << "<Row>\n";
        for (int col = 0; col < model->columnCount(); ++col) {
            QString val = model->data(model->index(row, col)).toString()
            .replace("&","&amp;").replace("<","&lt;").replace(">","&gt;");

            // Détecter si c'est un nombre
            bool isNum = false;
            double num = val.toDouble(&isNum);

            if (isNum && !val.isEmpty()) {
                out << "<Cell><Data ss:Type=\"Number\">" << num << "</Data></Cell>\n";
            } else {
                out << "<Cell><Data ss:Type=\"String\">" << val << "</Data></Cell>\n";
            }
        }
        out << "</Row>\n";
    }

    out << "</Table>\n</Worksheet>\n</Workbook>\n";
    file.close();

    QMessageBox::information(this, "Export réussi",
                             "Fichier exporté avec succès :\n" + filePath);
}
void MainWindow::on_exporter_excel_employes_clicked()
{
    Employe e;
    exporterTableViewExcel(e.afficher(), "Employes");
}
void MainWindow::envoyerEmail(QString dest, QString code)
{
    // ── Tes infos Gmail ───────────────────────────────────
    const QString smtpHost = "smtp.gmail.com";
    const int     smtpPort = 465;                        // SSL direct
    const QString smtpUser = "farhatsaidisaidi@gmail.com";       // ← ton Gmail
    const QString smtpPass = "pjyl ycfr sryt tzvx";      // ← App Password

    // ── Corps du mail ─────────────────────────────────────
    QString corps =
        "Bonjour,\r\n\r\n"
        "Vous avez demande une reinitialisation de mot de passe.\r\n\r\n"
        "Votre code de reinitialisation est :\r\n\r\n"
        "        " + code + "\r\n\r\n"
                 "Ce code expire dans 10 minutes.\r\n\r\n"
                  "Cordialement \r\n\r\n"
                 "-- Smart Oil Press";

    // ── Connexion SSL directe sur port 465 ────────────────
    QSslSocket socket;
    socket.setPeerVerifyMode(QSslSocket::VerifyNone); // pas de verif certificat
    socket.connectToHostEncrypted(smtpHost, smtpPort);

    if (!socket.waitForEncrypted(8000)) {
        QMessageBox::critical(this, "Erreur connexion",
                              "Impossible de se connecter au serveur Gmail.\n"
                              "Vérifiez votre connexion internet.\n\n"
                              "Détail : " + socket.errorString());
        return;
    }

    // ── Helpers lire / écrire ─────────────────────────────
    auto lire = [&]() -> QString {
        socket.waitForReadyRead(5000);
        QString rep = socket.readAll();
        qDebug() << "<<" << rep.trimmed();
        return rep;
    };

    auto envoyer = [&](QString cmd) {
        qDebug() << ">>" << cmd;
        socket.write((cmd + "\r\n").toUtf8());
        socket.waitForBytesWritten(3000);
    };

    // ── Dialogue SMTP ─────────────────────────────────────
    lire();                                    // 220 smtp.gmail.com

    envoyer("EHLO localhost");
    lire();                                    // 250 capabilities

    envoyer("AUTH LOGIN");
    lire();                                    // 334 username

    envoyer(smtpUser.toUtf8().toBase64());
    lire();                                    // 334 password

    envoyer(smtpPass.toUtf8().toBase64());
    QString authResp = lire();                 // 235 OK  ou  535 erreur

    if (!authResp.contains("235")) {
        QMessageBox::critical(this, "Erreur authentification",
                              "Gmail a refusé la connexion.\n\n"
                              "Vérifiez :\n"
                              "1. L'adresse Gmail dans le code\n"
                              "2. L'App Password (pas votre vrai mot de passe)\n"
                              "   → myaccount.google.com/apppasswords\n\n"
                              "Réponse Gmail : " + authResp.trimmed());
        socket.disconnectFromHost();
        return;
    }

    envoyer("MAIL FROM:<" + smtpUser + ">");
    lire();                                    // 250 OK

    envoyer("RCPT TO:<" + dest + ">");
    QString rcptResp = lire();                 // 250 OK ou 550 erreur

    if (!rcptResp.contains("250")) {
        QMessageBox::critical(this, "Erreur destinataire",
                              "Adresse email invalide ou refusée :\n" + dest +
                                  "\n\nRéponse : " + rcptResp.trimmed());
        socket.disconnectFromHost();
        return;
    }

    envoyer("DATA");
    lire();                                    // 354 Start input

    // Message complet
    QString message =
        "From: Smart Oil Press <" + smtpUser + ">\r\n"
                                               "To: " + dest + "\r\n"
                 "Subject: Code de verification - Smart Oil Press\r\n"
                 "Content-Type: text/plain; charset=UTF-8\r\n"
                 "\r\n" +
        corps + "\r\n.";

    envoyer(message);
    QString dataResp = lire();                // 250 OK

    if (dataResp.contains("250")) {
        qDebug() << "✅ Email envoyé à" << dest;
    } else {
        QMessageBox::warning(this, "Avertissement",
                             "Problème lors de l'envoi.\n"
                             "Réponse : " + dataResp.trimmed());
    }

    envoyer("QUIT");
    socket.disconnectFromHost();
}

void MainWindow::on_btn_mdp_oublie_clicked()
{
    // ══════════════════════════════════════════
    // DIALOG 1 — Email
    // ══════════════════════════════════════════
    QDialog dlgEmail(this);
    dlgEmail.setWindowTitle("Mot de passe oublié");
    dlgEmail.setFixedSize(400, 200);
    dlgEmail.setStyleSheet(
        "QDialog { background-color: #f5f5f0; }"
        "QLabel  { color: #556B2F; font-weight: bold; font-size: 13px; }"
        "QLineEdit { border: 2px solid #D4AF37; border-radius: 6px;"
        "            padding: 8px; font-size: 13px; }"
        "QPushButton { background-color: #556B2F; color: white;"
        "              border-radius: 6px; padding: 8px 16px;"
        "              font-size: 13px; font-weight: bold; }"
        "QPushButton:hover { background-color: #6B8E23; }"
        );

    QVBoxLayout *layoutEmail = new QVBoxLayout(&dlgEmail);
    QLabel      *lblEmail    = new QLabel("Entrez votre email :");
    QLineEdit   *inputEmail  = new QLineEdit();
    inputEmail->setPlaceholderText("exemple@gmail.com");

    QHBoxLayout *btnsEmail   = new QHBoxLayout();
    QPushButton *btnEnvoyer  = new QPushButton("Envoyer le code");
    QPushButton *btnAnnuler1 = new QPushButton("Annuler");
    btnAnnuler1->setStyleSheet(
        "background-color: #aaa; color: white;"
        "border-radius: 6px; padding: 8px 16px;");

    btnsEmail->addWidget(btnEnvoyer);
    btnsEmail->addWidget(btnAnnuler1);

    layoutEmail->addWidget(lblEmail);
    layoutEmail->addWidget(inputEmail);
    layoutEmail->addLayout(btnsEmail);

    connect(btnAnnuler1, &QPushButton::clicked,
            &dlgEmail, &QDialog::reject);
    connect(btnEnvoyer, &QPushButton::clicked, [&]() {
        QString email = inputEmail->text().trimmed();
        if (email.isEmpty()) {
            QMessageBox::warning(&dlgEmail, "Erreur",
                                 "Entrez votre email !"); return; }

        QSqlQuery q;
        q.prepare("SELECT ID_EMP FROM SMART.EMPLOYE "
                  "WHERE TRIM(UPPER(EMAIL)) = TRIM(UPPER(:e))");
        q.bindValue(":e", email);
        q.exec();
        if (!q.next()) {
            QMessageBox::warning(&dlgEmail, "Erreur",
                                 "Aucun compte avec cet email !"); return; }

        dlgEmail.accept();
    });

    if (dlgEmail.exec() != QDialog::Accepted) return;
    QString email = inputEmail->text().trimmed();

    // Générer et envoyer le code
    QString code = QString::number(
        QRandomGenerator::global()->bounded(100000, 999999));
    envoyerEmail(email, code);
    QMessageBox::information(this, "Code envoyé",
                             "Un code a été envoyé à :\n" + email);

    // ══════════════════════════════════════════
    // DIALOG 2 — Code
    // ══════════════════════════════════════════
    QDialog dlgCode(this);
    dlgCode.setWindowTitle("Vérification du code");
    dlgCode.setFixedSize(400, 200);
    dlgCode.setStyleSheet(
        "QDialog { background-color: #f5f5f0; }"
        "QLabel  { color: #556B2F; font-weight: bold; font-size: 13px; }"
        "QLineEdit { border: 2px solid #D4AF37; border-radius: 6px;"
        "            padding: 8px; font-size: 13px; }"
        "QPushButton { background-color: #556B2F; color: white;"
        "              border-radius: 6px; padding: 8px 16px;"
        "              font-size: 13px; font-weight: bold; }"
        "QPushButton:hover { background-color: #6B8E23; }"
        );

    QVBoxLayout *layoutCode = new QVBoxLayout(&dlgCode);
    QLabel      *lblCode    = new QLabel("Entrez le code reçu par email :");
    QLineEdit   *inputCode  = new QLineEdit();
    inputCode->setPlaceholderText("Code à 6 chiffres");
    inputCode->setMaxLength(6);

    QHBoxLayout *btnsCode   = new QHBoxLayout();
    QPushButton *btnVerif   = new QPushButton("Vérifier");
    QPushButton *btnAnnuler2 = new QPushButton("Annuler");
    btnAnnuler2->setStyleSheet(
        "background-color: #aaa; color: white;"
        "border-radius: 6px; padding: 8px 16px;");

    btnsCode->addWidget(btnVerif);
    btnsCode->addWidget(btnAnnuler2);

    layoutCode->addWidget(lblCode);
    layoutCode->addWidget(inputCode);
    layoutCode->addLayout(btnsCode);

    connect(btnAnnuler2, &QPushButton::clicked,
            &dlgCode, &QDialog::reject);
    connect(btnVerif, &QPushButton::clicked, [&]() {
        if (inputCode->text().trimmed() != code) {
            QMessageBox::warning(&dlgCode, "Erreur",
                                 "Code incorrect !"); return; }
        dlgCode.accept();
    });

    if (dlgCode.exec() != QDialog::Accepted) return;
    QMessageBox::information(this, "Succès", "Code correct !");

    // ══════════════════════════════════════════
    // DIALOG 3 — Nouveau mot de passe
    // ══════════════════════════════════════════
    QDialog dlgMdp(this);
    dlgMdp.setWindowTitle("Nouveau mot de passe");
    dlgMdp.setFixedSize(400, 230);
    dlgMdp.setStyleSheet(
        "QDialog { background-color: #f5f5f0; }"
        "QLabel  { color: #556B2F; font-weight: bold; font-size: 13px; }"
        "QLineEdit { border: 2px solid #D4AF37; border-radius: 6px;"
        "            padding: 8px; font-size: 13px; }"
        "QPushButton { background-color: #556B2F; color: white;"
        "              border-radius: 6px; padding: 8px 16px;"
        "              font-size: 13px; font-weight: bold; }"
        "QPushButton:hover { background-color: #6B8E23; }"
        );

    QVBoxLayout *layoutMdp  = new QVBoxLayout(&dlgMdp);
    QLabel      *lblNouv    = new QLabel("Nouveau mot de passe :");
    QLineEdit   *inputNouv  = new QLineEdit();
    inputNouv->setEchoMode(QLineEdit::Password);
    inputNouv->setPlaceholderText("Minimum 6 caractères");

    QLabel      *lblConf    = new QLabel("Confirmer mot de passe :");
    QLineEdit   *inputConf  = new QLineEdit();
    inputConf->setEchoMode(QLineEdit::Password);
    inputConf->setPlaceholderText("Répétez le mot de passe");

    QHBoxLayout *btnsMdp    = new QHBoxLayout();
    QPushButton *btnChanger = new QPushButton("Changer");
    QPushButton *btnAnnuler3 = new QPushButton("Annuler");
    btnAnnuler3->setStyleSheet(
        "background-color: #aaa; color: white;"
        "border-radius: 6px; padding: 8px 16px;");

    btnsMdp->addWidget(btnChanger);
    btnsMdp->addWidget(btnAnnuler3);

    layoutMdp->addWidget(lblNouv);
    layoutMdp->addWidget(inputNouv);
    layoutMdp->addWidget(lblConf);
    layoutMdp->addWidget(inputConf);
    layoutMdp->addLayout(btnsMdp);

    connect(btnAnnuler3, &QPushButton::clicked,
            &dlgMdp, &QDialog::reject);
    connect(btnChanger, &QPushButton::clicked, [&]() {
        QString nouv = inputNouv->text();
        QString conf = inputConf->text();
        if (nouv.length() < 6) {
            QMessageBox::warning(&dlgMdp, "Erreur",
                                 "Minimum 6 caractères !"); return; }
        if (nouv != conf) {
            QMessageBox::warning(&dlgMdp, "Erreur",
                                 "Les mots de passe ne correspondent pas !"); return; }
        dlgMdp.accept();
    });

    if (dlgMdp.exec() != QDialog::Accepted) return;

    // Mettre à jour dans BD
    QSqlQuery query;
    query.prepare(
        "UPDATE SMART.EMPLOYE SET MOT_DE_PASSE = :mdp "
        "WHERE TRIM(UPPER(EMAIL)) = TRIM(UPPER(:email))"
        );
    query.bindValue(":mdp",   inputNouv->text());
    query.bindValue(":email", email);

    if (query.exec()) {
        QMessageBox::information(this, "Succès",
                                 "Mot de passe changé avec succès !\n"
                                 "Connectez-vous avec le nouveau mot de passe.");
    } else {
        QMessageBox::critical(this, "Erreur",
                              "Erreur : " + query.lastError().text());
    }
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

QString MainWindow::interrogerOllama(const QString &prompt)
{
    // ✅ Construire l'historique complet
    QString conversationComplete = "";
    for (auto &msg : historiqueChat) {
        if (msg.first == "user")
            conversationComplete += "Utilisateur: " + msg.second + "\n";
        else
            conversationComplete += "Assistant: " + msg.second + "\n";
    }
    conversationComplete += "Utilisateur: " + prompt + "\n";

    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl("http://localhost:11434/api/generate"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject body;
    body["model"]  = "llama3.2";
    body["prompt"] = conversationComplete;
    body["stream"] = false;
    body["system"] = "Tu es un assistant RH pour une huilerie tunisienne "
                     "appelee Smart Oil Press. Reponds uniquement en francais. "
                     "Sois precis et professionnel. Souviens-toi de tout ce "
                     "qui a ete dit dans cette conversation.";

    QNetworkReply *reply = manager.post(
        request, QJsonDocument(body).toJson());

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished,
                     &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        return "Erreur : Ollama ne repond pas.";
    }

    QJsonObject response = QJsonDocument::fromJson(
                               reply->readAll()).object();
    reply->deleteLater();

    QString reponse = response["response"].toString().trimmed();

    // ✅ Sauvegarder dans l'historique
    historiqueChat.append({"user",      prompt});
    historiqueChat.append({"assistant", reponse});

    return reponse;
}

// ══════════════════════════════════════════
// Récupérer infos employé depuis BD
// ══════════════════════════════════════════
QString MainWindow::getInfosEmploye(const QString &question)
{
    // Chercher un nom dans la question
    QSqlQuery query;
    query.exec(
        "SELECT ID_EMP, CIN, NOM, PRENOM, DEPARTEMENT, POSTE, "
        "TYPE_CONTRAT, SALAIRE, PRIMES, STATUT, "
        "HEURE_ENTREE, HEURE_SORTIE "
        "FROM SMART.EMPLOYE"
        );

    QString employes = "";
    while (query.next()) {
        QString nom    = query.value(2).toString();
        QString prenom = query.value(3).toString();

        // Vérifier si le nom/prénom est mentionné dans la question
        if (question.contains(nom, Qt::CaseInsensitive) ||
            question.contains(prenom, Qt::CaseInsensitive)) {

            // Calculer heures travaillées
            QString heureEntree = query.value(10).toString();
            QString heureSortie = query.value(11).toString();
            QString heuresTravaillees = "Non disponible";

            if (!heureEntree.isEmpty() && !heureSortie.isEmpty()) {
                QTime entree = QTime::fromString(heureEntree, "HH:mm");
                QTime sortie = QTime::fromString(heureSortie, "HH:mm");
                int minutes  = entree.secsTo(sortie) / 60;
                int heures   = minutes / 60;
                int mins     = minutes % 60;
                heuresTravaillees = QString("%1h%2").arg(heures).arg(mins);
            }

            employes += QString(
                            "Employe trouve :\n"
                            "- ID         : %1\n"
                            "- CIN        : %2\n"
                            "- Nom        : %3\n"
                            "- Prenom     : %4\n"
                            "- Departement: %5\n"
                            "- Poste      : %6\n"
                            "- Contrat    : %7\n"
                            "- Salaire    : %8 DT\n"
                            "- Primes     : %9 DT\n"
                            "- Statut     : %10\n"
                            "- Heure entree: %11\n"
                            "- Heure sortie: %12\n"
                            "- Heures travaillees: %13\n"
                            )
                            .arg(query.value(0).toString())
                            .arg(query.value(1).toString())
                            .arg(nom)
                            .arg(prenom)
                            .arg(query.value(4).toString())
                            .arg(query.value(5).toString())
                            .arg(query.value(6).toString())
                            .arg(query.value(7).toString())
                            .arg(query.value(8).toString())
                            .arg(query.value(9).toString())
                            .arg(heureEntree.isEmpty() ? "Non renseignee" : heureEntree)
                            .arg(heureSortie.isEmpty() ? "Non renseignee" : heureSortie)
                            .arg(heuresTravaillees);
        }
    }

    if (employes.isEmpty())
        employes = "Aucun employe trouve avec ce nom dans la base de donnees.";

    return employes;
}

// ══════════════════════════════════════════
// Bouton Chatbot → Popup
// ══════════════════════════════════════════
void MainWindow::on_btn_chatbot_clicked()
{
    // ── Créer le Dialog ──────────────────────
    QDialog *dlg = new QDialog(this);
    dlg->setWindowTitle("Chatbot - Gestion Employes");
    dlg->setFixedSize(600, 500);
    dlg->setStyleSheet(
        "QDialog    { background-color: #f5f5f0; }"
        "QTextEdit  { background-color: white; border: 2px solid #D4AF37;"
        "             border-radius: 8px; padding: 8px; font-size: 13px; }"
        "QLineEdit  { border: 2px solid #D4AF37; border-radius: 8px;"
        "             padding: 8px; font-size: 13px; }"
        "QPushButton { background-color: #556B2F; color: white;"
        "              border-radius: 8px; padding: 8px 16px;"
        "              font-size: 13px; font-weight: bold; }"
        "QPushButton:hover { background-color: #6B8E23; }"
        "QLabel { color: #556B2F; font-weight: bold; font-size: 14px; }"
        );

    // ── Layout principal ─────────────────────
    QVBoxLayout *mainLayout = new QVBoxLayout(dlg);

    // Titre
    QLabel *lblTitre = new QLabel("Assistant Employes - Smart Oil Press");
    lblTitre->setAlignment(Qt::AlignCenter);
    lblTitre->setStyleSheet(
        "font-size: 15px; font-weight: bold; color: #556B2F;"
        "padding: 8px; background-color: #e8f0d8; border-radius: 6px;");

    // Zone de conversation
    QTextEdit *chatArea = new QTextEdit();
    chatArea->setReadOnly(true);
    chatArea->setMinimumHeight(320);
    chatArea->append(
        "<b style='color:#556B2F;'>Assistant :</b> "
        "Bonjour ! Je suis votre assistant pour la gestion des employes. "
        "Posez-moi une question sur un employe, par exemple :<br>"
        "- <i>Quelles sont les infos de Mohamed ?</i><br>"
        "- <i>Combien d'heures a travaille Ali ?</i><br>"
        "- <i>Quelle prime suggeres-tu pour Sara ?</i>"
        );

    // Zone de saisie
    QHBoxLayout *inputLayout = new QHBoxLayout();
    QLineEdit   *inputMsg    = new QLineEdit();
    inputMsg->setPlaceholderText("Posez votre question ici...");
    QPushButton *btnEnvoyer  = new QPushButton("Envoyer");
    QPushButton *btnFermer   = new QPushButton("Fermer");
    btnFermer->setStyleSheet(
        "background-color: #888; color: white; border-radius: 8px;"
        "padding: 8px 16px; font-size: 13px;");

    inputLayout->addWidget(inputMsg);
    inputLayout->addWidget(btnEnvoyer);

    mainLayout->addWidget(lblTitre);
    mainLayout->addWidget(chatArea);
    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(btnFermer);

    // ── Fermer ───────────────────────────────
    connect(btnFermer, &QPushButton::clicked, dlg, &QDialog::close);

    // ── Envoyer message ──────────────────────
    auto envoyerMessage = [&]() {
        QString question = inputMsg->text().trimmed();
        if (question.isEmpty()) return;

        // Afficher la question
        chatArea->append(
            "<br><b style='color:#1565C0;'>Vous :</b> " + question);
        chatArea->append(
            "<b style='color:#556B2F;'>Assistant :</b> "
            "<i>Recherche en cours...</i>");
        inputMsg->clear();
        QApplication::processEvents();

        // Récupérer infos depuis BD
        QString infosEmploye = getInfosEmploye(question);

        // Construire le prompt pour Ollama
        QString prompt = QString(
                             "Tu es un assistant RH pour une huilerie tunisienne appelee Smart Oil Press. "
                             "Reponds uniquement en francais. Sois precis et professionnel.\n\n"
                             "Voici les informations de la base de donnees :\n%1\n\n"
                             "Question : %2\n\n"
                             "Si la question concerne les primes, analyse le salaire et les heures "
                             "travaillees et fais une recommandation precise avec un montant en DT. "
                             "Si la question concerne les heures, calcule et explique."
                             ).arg(infosEmploye).arg(question);

        // Interroger Ollama
        QString reponse = interrogerOllama(prompt);

        // Supprimer le "Recherche en cours..."
        QTextCursor cursor = chatArea->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
        cursor.deletePreviousChar();

        // Afficher la réponse
        chatArea->append(
            "<b style='color:#556B2F;'>Assistant :</b> " + reponse + "<br>");

        // Scroll vers le bas
        chatArea->verticalScrollBar()->setValue(
            chatArea->verticalScrollBar()->maximum());
    };

    // Envoyer avec bouton ou touche Entrée
    connect(btnEnvoyer, &QPushButton::clicked, envoyerMessage);
    connect(inputMsg,   &QLineEdit::returnPressed, envoyerMessage);

    dlg->exec();

    QPushButton *btnNouveau = new QPushButton("Nouvelle conversation");
    btnNouveau->setStyleSheet(
        "background-color: #D4AF37; color: white; border-radius: 8px;"
        "padding: 8px 16px; font-size: 13px;");

    // Ajouter dans le layout
    mainLayout->addWidget(btnNouveau);

    // Effacer l'historique et le chat
    connect(btnNouveau, &QPushButton::clicked, [&]() {
        historiqueChat.clear();
        chatArea->clear();
        chatArea->append(
            "<b style='color:#556B2F;'>Assistant :</b> "
            "Nouvelle conversation demarree ! Comment puis-je vous aider ?");
    });
}

void MainWindow::on_btn_face_login_clicked()
{
    QString cin = ui->lineEdit->text().trimmed();

    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Saisissez votre CIN d'abord !");
        return;
    }

    // Chercher photo dans BD
    QSqlQuery query;
    query.prepare("SELECT PHOTO_PATH, NOM, PRENOM FROM EMPLOYE WHERE CIN = :cin");
    query.bindValue(":cin", cin.toInt());

    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "Erreur", "CIN introuvable !");
        return;
    }

    QString photoPath = query.value(0).toString();
    QString nom       = query.value(1).toString();
    QString prenom    = query.value(2).toString();

    if (photoPath.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucune photo pour cet employé !");
        return;
    }

    QMessageBox::information(this, "Face ID", "📷 Regardez la webcam...");

    // Lancer script Python
    QString scriptPath = "C:/Users/user/OneDrive - ESPRIT/Documents/Integration_PROJET/face_login.py";
    QProcess *process = new QProcess(this);
    process->start("py", QStringList() << "-3.11" << scriptPath << photoPath);

    connect(process, &QProcess::finished, this,
            [=](int, QProcess::ExitStatus) {
                QString result = process->readAllStandardOutput().trimmed();
                process->deleteLater();

                // Garder seulement la dernière ligne (ignorer warnings)
                QStringList lines = result.split("\n");
                QString lastLine = lines.last().trimmed();

                if (lastLine == "OK") {
                    QMessageBox::information(this, "Accès autorisé",
                                             "Bonjour " + prenom + " " + nom + " ! 👋");
                    ui->stackedWidget->setCurrentWidget(ui->dashboard);
                } else if (lastLine == "NO_FILE") {
                    QMessageBox::critical(this, "Erreur", "Photo introuvable !");
                } else if (lastLine == "NO_CAM") {
                    QMessageBox::critical(this, "Erreur", "Webcam non disponible !");
                } else if (lastLine == "NO_FACE") {
                    QMessageBox::warning(this, "Échec", "Aucun visage détecté !");
                } else {
                    QMessageBox::warning(this, "Accès refusé", "Visage non reconnu ❌");
                }
            });
}
void MainWindow::on_btn_prendre_photo_clicked()
{
    QString cin = ui->lineEdit_cin_4->text().trimmed();
    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Saisissez le CIN d'abord !");
        return;
    }

    QString dossier   = QCoreApplication::applicationDirPath() + "/photos/";
    QString photoPath = dossier + cin + ".jpg";

    // Créer dossier si inexistant
    QDir().mkpath(dossier);

    // Capturer via Python
    QString script = QString(
                         "import cv2\n"
                         "cap = cv2.VideoCapture(0)\n"
                         "ret, frame = cap.read()\n"
                         "cap.release()\n"
                         "cv2.imwrite(r'%1', frame)\n"
                         "print('OK')\n"
                         ).arg(photoPath);

    QProcess *p = new QProcess(this);
    p->start("py", QStringList() << "-3.11" << "-c" << script);
    p->waitForFinished(5000);

    // Sauvegarder chemin en BD
    QSqlQuery q;
    q.prepare("UPDATE EMPLOYE SET PHOTO_PATH = :path WHERE CIN = :cin");
    q.bindValue(":path", photoPath);
    q.bindValue(":cin", cin.toInt());
    q.exec();

    QMessageBox::information(this, "Photo", "✅ Photo enregistrée !");
    p->deleteLater();
}
void MainWindow::on_btn_login_3_clicked()
{


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
    QString nom = ui->recherche1_2->text().trimmed();

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
    QString cin = ui->lineEdit->text().trimmed();
    QString mdp = ui->lineEdit_5->text().trimmed();

    if (cin.isEmpty() || mdp.isEmpty()) {
        QMessageBox::warning(this, "Erreur",
                             "Remplissez tous les champs !");
        return;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT ID_EMP FROM SMART.EMPLOYE "
        "WHERE TRIM(CIN) = :cin AND TRIM(MOT_DE_PASSE) = :mdp"
        );
    query.bindValue(":cin", cin);
    query.bindValue(":mdp", mdp);

    if (!query.exec()) {
        qDebug() << "ERREUR SQL :" << query.lastError().text();
        QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
        return;
    }

    if (query.next()) {
        // ✅ Connexion réussie
        ui->stackedWidget->setCurrentWidget(ui->dashboard);
    } else {
        // Debug : afficher ce qui est en BD pour ce CIN
        QSqlQuery dbg;
        dbg.prepare("SELECT CIN, MOT_DE_PASSE FROM SMART.EMPLOYE WHERE TRIM(CIN) = :cin");
        dbg.bindValue(":cin", cin);
        dbg.exec();
        if (dbg.next()) {
            qDebug() << "CIN en BD     :" << dbg.value(0).toString();
            qDebug() << "MDP en BD     :" << dbg.value(1).toString();
            qDebug() << "MDP saisi     :" << mdp;
            qDebug() << "Correspondance:" << (dbg.value(1).toString().trimmed() == mdp);
        } else {
            qDebug() << "CIN introuvable en BD :" << cin;
        }

        QMessageBox::warning(this, "Erreur",
                             "CIN ou mot de passe incorrect !");
    }
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

// ─────────────────────────────────────────────────────────────────────────────
// 🔍 RECHERCHE PAR CLIENT
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::on_vente_rechercheClientBtn_clicked()
{
    QString texte = ui->vente_rechercheClientEdit->text().trimmed();
    if (texte.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un ID client.");
        return;
    }

    bool ok;
    int idClient = texte.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Erreur", "L'ID client doit être un nombre entier.");
        return;
    }

    QSqlQueryModel* model = Vente::rechercherParClient(idClient);
    ui->tableView_vente->setModel(model);
    ui->tableView_vente->resizeColumnsToContents();

    if (model->rowCount() == 0)
        QMessageBox::information(this, "Résultat",
                                 "Aucune vente trouvée pour le client ID : "
                                 + QString::number(idClient));
}

// 🔍 RECHERCHE PAR DATE
void MainWindow::on_vente_rechercheDateBtn_clicked()
{
    QDate date = ui->vente_dateRechercheEdit->date();
    QSqlQueryModel* model = Vente::rechercherParDate(date);
    ui->tableView_vente->setModel(model);
    ui->tableView_vente->resizeColumnsToContents();
}

// 🔃 TRI
void MainWindow::on_vente_triBox_currentIndexChanged(int index)
{
    Vente::TriType type;
    bool asc = true;
    switch (index) {
    case 0: type = Vente::ParDate;    asc = true;  break;
    case 1: type = Vente::ParDate;    asc = false; break;
    case 2: type = Vente::ParMontant; asc = true;  break;
    case 3: type = Vente::ParMontant; asc = false; break;
    default: return;
    }
    QSqlQueryModel* model = Vente::listerTries(type, asc);
    ui->tableView_vente->setModel(model);
    ui->tableView_vente->resizeColumnsToContents();
}

// ─────────────────────────────────────────────────────────────────────────────
// 📄 EXPORTS
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::on_vente_exportCSVBtn_clicked()
{
    QString fichier = QFileDialog::getSaveFileName(this, "Exporter en CSV", "", "Fichiers CSV (*.csv)");
    if (fichier.isEmpty()) return;
    if (!fichier.endsWith(".csv")) fichier += ".csv";
    if (Vente::exporterCSV(fichier))
        QMessageBox::information(this, "Export CSV", "Export réussi !");
    else
        QMessageBox::critical(this, "Export CSV", "Échec de l'export.");
}

void MainWindow::on_vente_exportPDFBtn_clicked()
{
    QString fichier = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "Fichiers PDF (*.pdf)");
    if (fichier.isEmpty()) return;
    if (!fichier.endsWith(".pdf")) fichier += ".pdf";
    if (Vente::exporterPDF(fichier, "Liste des ventes"))
        QMessageBox::information(this, "Export PDF", "Export réussi !");
    else
        QMessageBox::critical(this, "Export PDF", "Échec de l'export.");
}

// ─────────────────────────────────────────────────────────────────────────────
// 📊 ANALYSE CA
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::on_vente_analyserCABtn_clicked()
{
    QDate debut = ui->vente_dateDebutAnalyse->date();
    QDate fin   = ui->vente_dateFinAnalyse->date();
    Vente::StatsCA stats = Vente::calculerChiffreAffaires(debut, fin);

    QString msg = QString("📊 Chiffre d'affaires du %1 au %2\n\n"
                          "Total          : %3 DT\n"
                          "Moyenne/vente  : %4 DT\n"
                          "Minimum        : %5 DT\n"
                          "Maximum        : %6 DT\n"
                          "Nombre ventes  : %7")
                      .arg(debut.toString("dd/MM/yyyy"))
                      .arg(fin.toString("dd/MM/yyyy"))
                      .arg(stats.total,   0, 'f', 2)
                      .arg(stats.moyenne, 0, 'f', 2)
                      .arg(stats.min,     0, 'f', 2)
                      .arg(stats.max,     0, 'f', 2)
                      .arg(stats.nbVentes);
    QMessageBox::information(this, "Analyse du CA", msg);
}

// ⚠️ DÉTECTION ANOMALIES
void MainWindow::on_vente_detecterAnomaliesBtn_clicked()
{
    QDate debut = ui->vente_dateDebutAnalyse->date();
    QDate fin   = ui->vente_dateFinAnalyse->date();
    QVector<Vente> anomalies = Vente::detecterAnomalies(debut, fin);

    if (anomalies.isEmpty()) {
        QMessageBox::information(this, "Anomalies", "Aucune vente anormale détectée.");
        return;
    }
    QString liste;
    for (const Vente& v : anomalies) {
        liste += QString("Vente %1 — Client %2 — Montant %3 DT\n")
                     .arg(v.getIdVente())
                     .arg(v.getIdClient())
                     .arg(v.getMontantPaye(), 0, 'f', 2);
    }
    QMessageBox::information(this, "Ventes anormales détectées", liste);
}

// 📈 PRÉVISION
void MainWindow::on_vente_previsionBtn_clicked()
{
    double prev = Vente::prevoirVentesMoisProchain();
    QMessageBox::information(this, "Prévision CA",
                             QString("Prévision du CA pour les 30 prochains jours :\n%1 DT")
                                 .arg(prev, 0, 'f', 2));
}

// 📊 GRAPHIQUE (QtCharts) — courbes sur toute la plage de dates disponibles
void MainWindow::on_vente_graphiqueCABtn_clicked()
{
    // Récupérer la date min et max directement depuis la BD
    QSqlQuery rangeQuery(QSqlDatabase::database());
    rangeQuery.exec("SELECT TO_CHAR(MIN(DATE_VENTE),'YYYY-MM-DD'), "
                    "TO_CHAR(MAX(DATE_VENTE),'YYYY-MM-DD') FROM SMART.VENTE");

    QDate debut, fin;
    if (rangeQuery.next()) {
        debut = QDate::fromString(rangeQuery.value(0).toString(), "yyyy-MM-dd");
        fin   = QDate::fromString(rangeQuery.value(1).toString(), "yyyy-MM-dd");
    }

    if (!debut.isValid() || !fin.isValid()) {
        QMessageBox::information(this, "Graphique", "Aucune vente en base de données.");
        return;
    }

    QVector<QPair<QDate, double>> data = Vente::chiffreAffairesParJour(debut, fin);

    if (data.isEmpty()) {
        QMessageBox::information(this, "Graphique", "Aucune donnée disponible.");
        return;
    }

    // ── Série en courbe lisse ──────────────────────────────────────────────
    QSplineSeries *series = new QSplineSeries();
    series->setName("CA quotidien (DT)");
    QPen pen(QColor("#556B2F"));
    pen.setWidth(2);
    series->setPen(pen);

    qint64 minMs = LLONG_MAX, maxMs = LLONG_MIN;
    double maxVal = 0;

    for (const auto& point : data) {
        qint64 ms = point.first.startOfDay().toMSecsSinceEpoch();
        series->append(ms, point.second);
        if (ms < minMs) minMs = ms;
        if (ms > maxMs) maxMs = ms;
        if (point.second > maxVal) maxVal = point.second;
    }

    // ── Graphique ──────────────────────────────────────────────────────────
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(QString("Évolution du CA  —  %1  →  %2")
                        .arg(debut.toString("dd/MM/yyyy"))
                        .arg(fin.toString("dd/MM/yyyy")));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setBackgroundBrush(QBrush(Qt::white));
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Axe X : dates
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("dd/MM/yy");
    axisX->setTitleText("Date");
    axisX->setRange(QDateTime::fromMSecsSinceEpoch(minMs),
                    QDateTime::fromMSecsSinceEpoch(maxMs));
    axisX->setTickCount(qMin((int)data.size(), 10));
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Axe Y : montants
    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText("Montant (DT)");
    axisY->setRange(0, maxVal * 1.2);
    axisY->setLabelFormat("%.2f");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // ── Fenêtre d'affichage ────────────────────────────────────────────────
    QWidget *win = new QWidget(nullptr, Qt::Window);
    win->setWindowTitle(QString("Graphique CA  (%1 points)").arg(data.size()));
    win->resize(1000, 600);
    win->setAttribute(Qt::WA_DeleteOnClose);

    QVBoxLayout *layout = new QVBoxLayout(win);
    layout->setContentsMargins(10, 10, 10, 10);

    // Infos résumé en haut
    QLabel *lblInfo = new QLabel(
        QString("  %1 jours de données  |  Total : %2 DT  |  Moyenne/jour : %3 DT")
            .arg(data.size())
            .arg(QString::number(
                     std::accumulate(data.begin(), data.end(), 0.0,
                                     [](double s, const QPair<QDate,double>& p){ return s + p.second; }),
                     'f', 2))
            .arg(QString::number(
                     std::accumulate(data.begin(), data.end(), 0.0,
                                     [](double s, const QPair<QDate,double>& p){ return s + p.second; })
                     / data.size(), 'f', 2))
        );
    lblInfo->setStyleSheet("background:#556B2F; color:white; padding:6px; font-weight:bold;");
    layout->addWidget(lblInfo);

    QChartView *chartView = new QChartView(chart, win);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(chartView);

    win->setLayout(layout);
    win->show();
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



void MainWindow::on_btn_connecter_arduino_clicked()
{
    bool ok;
    QString port = QInputDialog::getText(
        this, "Connexion Arduino",
        "Entrez le port (ex: COM3 ou COM4):",
        QLineEdit::Normal, "COM3", &ok
        );
    if (!ok || port.isEmpty()) return;

    if (m_arduino->connecter(port)) {
        QMessageBox::information(this, "Arduino", "Connecté sur " + port);
    } else {
        QMessageBox::critical(this, "Arduino", "Impossible de se connecter sur " + port);
    }
}

void MainWindow::on_btn_ecrire_carte_clicked()
{
    // Récupérer le CIN sélectionné dans la tableView
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Erreur",
                             "Sélectionne un employé d'abord !");
        return;
    }

    // CIN est colonne 1
    QString cin = ui->tableView->model()
                      ->index(index.row(), 1).data().toString();

    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "CIN introuvable !");
        return;
    }

    if (!m_arduino->estConnecte()) {
        QMessageBox::warning(this, "Arduino",
                             "Arduino non connecté !");
        return;
    }

    // Envoyer le CIN à l'Arduino
    m_arduino->envoyerDonnees(cin);

    QMessageBox::information(this, "Arduino",
                             "CIN " + cin + " envoyé !\nScanne la carte maintenant 🃏");
}

void MainWindow::onCarteDetectee(const QString &cin)
{
    // Log dans le textEdit arduino
    ui->message_2->append(
        "🃏 Carte détectée : " + cin);
}

void MainWindow::onEmployeIdentifie(const QString &cin,
                                    const QString &nom,
                                    const QString &prenom)
{
    // Récupérer l'heure actuelle
    QString heure = QTime::currentTime().toString("HH:mm");

    // Popup avec nom, prénom et heure
    QMessageBox msgBox;
    msgBox.setWindowTitle("Pointage Présence");
    msgBox.setText(
        "<b style='font-size:16px'>✅ Accès autorisé</b><br><br>"
        "<b>Nom :</b> " + nom + "<br>"
                "<b>Prénom :</b> " + prenom + "<br>"
                   "<b>Heure :</b> " + heure
        );
    msgBox.setIconPixmap(QPixmap(":/images/images/lg-removebg-preview.png")
                             .scaled(80, 80, Qt::KeepAspectRatio));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();

    // Log dans textEdit
    ui->message_2->append("✅ " + nom + " " + prenom + " — " + heure);

    refreshTable();
}

void MainWindow::onEmployeInconnu(const QString &cin)
{
    QString heure = QTime::currentTime().toString("HH:mm");

    QMessageBox msgBox;
    msgBox.setWindowTitle("Accès Refusé");
    msgBox.setText(
        "<b style='font-size:16px'>❌ Accès refusé</b><br><br>"
        "<b>CIN :</b> " + cin + "<br>"
                "<b>Heure :</b> " + heure + "<br><br>"
                  "<i>Employé non reconnu dans le système</i>"
        );
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();

    // Log dans textEdit
    ui->message_2->append("❌ CIN inconnu : " + cin + " — " + heure);
}

void MainWindow::onMessageArduino(const QString &msg)
{
    ui->message_2->append("[Arduino] " + msg);
}

void MainWindow::on_recherche2_2_clicked()
{
    QString motcle = ui->recherche2->text().trimmed();

    bool isParticulier = ui->particulier_6->isChecked();
    bool isProfessionnel = ui->profisionnel_6->isChecked();

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


void MainWindow::on_appliquer_clicked()
{
    QDate date = ui->date_6->date();

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
void MainWindow::afficher_historique()
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



void MainWindow::on_exporter1_clicked()
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




void MainWindow::on_appliquer2_clicked()
{
    QSqlQuery query;

    QString type;

    if (ui->particulier_5->isChecked())
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


void MainWindow::on_appliquer3_2_clicked()
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


void MainWindow::on_exporter_2_clicked()
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


void MainWindow::on_executer_clicked()
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

void MainWindow::afficherCourbe()
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
void MainWindow::afficherCourbeDansTable2()
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


void MainWindow::on_classement_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_classement_clients);
}


void MainWindow::on_export_2_clicked()
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

