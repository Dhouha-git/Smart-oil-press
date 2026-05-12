#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "circlestatwidget.h"
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
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>
#include <QDate>

#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QTimer>

#include <numeric>
#include <QNetworkReply>
#include <climits>
#include <QStatusBar>
#include <QTextBrowser>
#include <QPainter>
#include <algorithm>
#include <QProgressBar>
#include <QDialog>
#include <QScrollArea>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLineSeries>






void MainWindow::loadDashboardStats()
{
    QSqlQuery q;

    int nbEmployes = 0, nbActifs = 0, nbConge = 0, nbSuspendus = 0;
    if (q.exec("SELECT COUNT(*) FROM EMPLOYE"))
        if (q.next()) nbEmployes = q.value(0).toInt();
    if (q.exec("SELECT COUNT(*) FROM EMPLOYE WHERE STATUT='Actif'"))
        if (q.next()) nbActifs = q.value(0).toInt();
    if (q.exec("SELECT COUNT(*) FROM EMPLOYE WHERE STATUT='En conge'"))
        if (q.next()) nbConge = q.value(0).toInt();
    if (q.exec("SELECT COUNT(*) FROM EMPLOYE WHERE STATUT='Suspendu'"))
        if (q.next()) nbSuspendus = q.value(0).toInt();

    int nbAgriculteurs = 0;
    if (q.exec("SELECT COUNT(*) FROM AGRICULTEUR"))
        if (q.next()) nbAgriculteurs = q.value(0).toInt();

    int nbClients = 0, nbGold = 0, nbSilver = 0;
    if (q.exec("SELECT COUNT(*) FROM CLIENT"))
        if (q.next()) nbClients = q.value(0).toInt();
    if (q.exec("SELECT COUNT(*) FROM CLIENT WHERE TYPE_CLIENT='Gold'"))
        if (q.next()) nbGold = q.value(0).toInt();
    if (q.exec("SELECT COUNT(*) FROM CLIENT WHERE TYPE_CLIENT='Silver'"))
        if (q.next()) nbSilver = q.value(0).toInt();

    int nbVentes = 0;
    double ca = 0.0;
    if (q.exec("SELECT COUNT(*), NVL(SUM(MONTANT_PAYE),0) FROM VENTE"))
        if (q.next()) {
            nbVentes = q.value(0).toInt();
            ca       = q.value(1).toDouble();
        }

    QList<int> ventesParMois(12, 0);
    QStringList moisLabels = {"Jan","Fév","Mar","Avr","Mai","Jun",
                               "Jul","Aoû","Sep","Oct","Nov","Déc"};
    if (q.exec("SELECT TO_NUMBER(TO_CHAR(DATE_VENTE,'MM')), COUNT(*) "
               "FROM VENTE "
               "WHERE EXTRACT(YEAR FROM DATE_VENTE)=EXTRACT(YEAR FROM SYSDATE) "
               "GROUP BY TO_NUMBER(TO_CHAR(DATE_VENTE,'MM')) "
               "ORDER BY 1"))
        while (q.next()) {
            int m = q.value(0).toInt();
            if (m >= 1 && m <= 12) ventesParMois[m-1] = q.value(1).toInt();
        }

    double huile = 0.0, rendement = 0.0;
    if (q.exec("SELECT NVL(SUM(QUANTITE_HUILE),0), NVL(AVG(RENDEMENT),0) FROM PRODUCTION"))
        if (q.next()) {
            huile     = q.value(0).toDouble();
            rendement = q.value(1).toDouble();
        }

    setupDashboard(nbEmployes, nbActifs, nbConge, nbSuspendus,
                   nbAgriculteurs,
                   nbClients, nbGold, nbSilver,
                   nbVentes, ca,
                   huile, rendement,
                   ventesParMois, moisLabels);
}

// ================================================================

void MainWindow::setupDashboard(int nbEmp, int nbActifs, int nbConge, int nbSusp,
                                int nbAgri, int nbCli, int nbGold, int nbSilver,
                                int nbVentes, double ca, double huile, double rendement,
                                QList<int> ventesParMois, QStringList moisLabels)
{
    // ── Zone de contenu (à droite de la sidebar du .ui) ───────
    QWidget *contentZone = ui->dashboard->findChild<QWidget*>("dashboard_content");
    if (!contentZone) {
        contentZone = new QWidget(ui->dashboard);
        contentZone->setObjectName("dashboard_content");
    }

    // Taille dynamique
    int sidebarW = 220;
    contentZone->setGeometry(sidebarW, 0,
                              ui->dashboard->width()  - sidebarW,
                              ui->dashboard->height());
    contentZone->setStyleSheet("background: #F4F6F0;");

    // ── Nettoyer l'ancien contenu ─────────────────────────────
    qDeleteAll(contentZone->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));
    if (contentZone->layout()) {
        delete contentZone->layout();
    }

    // ── ScrollArea ────────────────────────────────────────────
    QScrollArea *scrollArea = new QScrollArea(contentZone);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(
        "QScrollArea { background: #F4F6F0; border: none; }"
        "QScrollBar:vertical { width: 8px; background: #e0e0e0; border-radius: 4px; }"
        "QScrollBar::handle:vertical { background: #556B2F; border-radius: 4px; }"
    );

    QWidget *content = new QWidget();
    content->setStyleSheet("background: #F4F6F0;");
    QVBoxLayout *contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(12, 10, 12, 12);
    contentLayout->setSpacing(12);

    // ════════════════════════════════════════════════════════════
    // HEADER : Avatar + Nom + Titre
    // ════════════════════════════════════════════════════════════
    QFrame *headerBar = new QFrame(content);
    headerBar->setFixedHeight(64);
    headerBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    headerBar->setStyleSheet("QFrame { background: #556B2F; border-radius: 12px; }");

    QHBoxLayout *headerLayout = new QHBoxLayout(headerBar);
    headerLayout->setContentsMargins(16, 8, 16, 8);
    headerLayout->setSpacing(12);

    // Titre à gauche
    QLabel *titleLbl = new QLabel("🏠  Tableau de bord — Smart Oil Press");
    titleLbl->setStyleSheet("color: white; font-size: 15px; font-weight: bold; background: transparent;");
    headerLayout->addWidget(titleLbl);
    headerLayout->addStretch();

    // Avatar cercle DM
    QLabel *avatarLbl = new QLabel();
    avatarLbl->setFixedSize(42, 42);

    QPixmap avatarPixmap(42, 42);
    avatarPixmap.fill(Qt::transparent);
    QPainter painter(&avatarPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor("#D4AF37"));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, 42, 42);
    painter.setPen(QColor("#2C3E1A"));
    painter.setFont(QFont("Arial", 13, QFont::Bold));
    painter.drawText(QRect(0, 0, 42, 42), Qt::AlignCenter, "DM");
    painter.end();

    // Masque circulaire
    QPixmap circularPixmap(42, 42);
    circularPixmap.fill(Qt::transparent);
    QPainter maskPainter(&circularPixmap);
    maskPainter.setRenderHint(QPainter::Antialiasing);
    maskPainter.setBrush(Qt::white);
    maskPainter.setPen(Qt::NoPen);
    maskPainter.drawEllipse(0, 0, 42, 42);
    maskPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    maskPainter.drawPixmap(0, 0, avatarPixmap);
    maskPainter.end();

    avatarLbl->setPixmap(circularPixmap);
    avatarLbl->setStyleSheet("background: transparent;");

    // Nom + rôle
    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(1);
    infoLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *nameLbl = new QLabel("Dhouha Mhamdi");
    nameLbl->setStyleSheet("color: white; font-size: 12px; font-weight: bold; background: transparent;");
    QLabel *roleLbl = new QLabel("Administratrice");
    roleLbl->setStyleSheet("color: #D4AF37; font-size: 10px; background: transparent;");

    infoLayout->addWidget(nameLbl);
    infoLayout->addWidget(roleLbl);

    headerLayout->addWidget(avatarLbl);
    headerLayout->addLayout(infoLayout);

    contentLayout->addWidget(headerBar);

    // ════════════════════════════════════════════════════════════
    // LIGNE 1 — 5 CERCLES KPI
    // ════════════════════════════════════════════════════════════
    auto makeCircle = [&](const QString &label, const QString &value,
                          const QString &subtitle, const QColor &color, int percent) {
        // Forcer un minimum de 10% pour que le cercle soit toujours visible
        int displayPct = (percent <= 0) ? 10 : qMin(percent, 100);

        CircleStatWidget *w = new CircleStatWidget(label, value, subtitle, color, displayPct, content);
        w->setMinimumSize(130, 160);
        w->setMaximumSize(160, 185);
        return w;
    };

    // Pourcentage sécurisé
    auto pct = [](int val, int total) -> int {
        if (total <= 0) return 0;
        return qMin((val * 100) / total, 100);
    };

    QHBoxLayout *circleRow = new QHBoxLayout();
    circleRow->setSpacing(10);
    circleRow->setContentsMargins(14, 16, 14, 16);

    // Employés — % actifs
    circleRow->addWidget(makeCircle(
        "Employés", QString::number(nbEmp),
        QString("%1 actifs · %2 congé").arg(nbActifs).arg(nbConge),
        QColor("#378ADD"),
        pct(nbActifs, qMax(nbEmp, 1))));

    // Agriculteurs
    circleRow->addWidget(makeCircle(
        "Agriculteurs", QString::number(nbAgri),
        "Inscrits cette saison",
        QColor("#639922"),
        qMin(qMax(nbAgri * 5, 10), 100)));

    // Clients — TOUJOURS afficher un arc (min 15%)
    int clientPct = pct(nbGold + nbSilver, qMax(nbCli, 1));
    circleRow->addWidget(makeCircle(
        "Clients", QString::number(nbCli),
        QString("Gold %1 · Silver %2").arg(nbGold).arg(nbSilver),
        QColor("#D4537E"),
        qMax(clientPct, 15)));  // ← minimum 15% pour toujours afficher l'arc

    // Ventes
    circleRow->addWidget(makeCircle(
        "Ventes", QString::number(nbVentes),
        "Transactions totales",
        QColor("#BA7517"),
        qMin(qMax(nbVentes * 2, 10), 100)));

    // Production
    int prodPct = (int)qMin(qMax(huile / 10.0, 10.0), 100.0);
    circleRow->addWidget(makeCircle(
        "Production", QString("%1 t").arg(huile, 0, 'f', 1),
        "Huile cette saison",
        QColor("#1D9E75"),
        prodPct));

    QFrame *circleFrame = new QFrame(content);
    circleFrame->setStyleSheet(
        "QFrame { background: white; border-radius: 12px; border: 1px solid #e0e0e0; }");
    circleFrame->setLayout(circleRow);
    contentLayout->addWidget(circleFrame);

    // ════════════════════════════════════════════════════════════
    // LIGNE 2 — GRAPHIQUES
    // ════════════════════════════════════════════════════════════
    QHBoxLayout *chartRow = new QHBoxLayout();
    chartRow->setSpacing(12);

    // ── Donut — Statut des employés ──────────────────────────
    {
        QFrame *card = new QFrame(content);
        card->setMinimumWidth(280);
        card->setStyleSheet(
            "QFrame { background: white; border-radius: 12px; border: 1px solid #e0e0e0; }");
        QVBoxLayout *cl = new QVBoxLayout(card);
        cl->setContentsMargins(10, 8, 10, 8);

        QPieSeries *s = new QPieSeries();
        // Toujours mettre au moins 1 pour éviter un donut vide
        s->append("Actifs",    qMax(nbActifs,  1));
        s->append("En congé",  qMax(nbConge,   0));
        s->append("Suspendus", qMax(nbSusp,    0));
        s->setHoleSize(0.50);

        QList<QColor> cols = {QColor("#378ADD"), QColor("#AAAAAA"), QColor("#E24B4A")};
        for (int i = 0; i < s->count(); ++i) {
            s->slices().at(i)->setBrush(cols[i]);
            s->slices().at(i)->setBorderColor(Qt::white);
            s->slices().at(i)->setBorderWidth(2);
        }

        QChart *chart = new QChart();
        chart->addSeries(s);
        chart->setTitle("Statut des employés");
        chart->setTitleFont(QFont("Arial", 9, QFont::Bold));
        chart->legend()->setAlignment(Qt::AlignBottom);
        chart->legend()->setFont(QFont("Arial", 8));
        chart->setAnimationOptions(QChart::SeriesAnimations);
        chart->setMargins(QMargins(4, 4, 4, 4));
        chart->setBackgroundVisible(false);

        QChartView *v = new QChartView(chart, card);
        v->setRenderHint(QPainter::Antialiasing);
        v->setMinimumHeight(220);
        v->setMaximumHeight(260);
        v->setBackgroundBrush(Qt::transparent);
        cl->addWidget(v);
        chartRow->addWidget(card);
    }

    // ── Bar Chart — Ventes par mois ──────────────────────────
    {
        QFrame *card = new QFrame(content);
        card->setMinimumWidth(280);
        card->setStyleSheet(
            "QFrame { background: white; border-radius: 12px; border: 1px solid #e0e0e0; }");
        QVBoxLayout *cl = new QVBoxLayout(card);
        cl->setContentsMargins(10, 8, 10, 8);

        QBarSet *set = new QBarSet("Ventes");
        set->setColor(QColor("#BA7517"));
        set->setBorderColor(QColor("#BA7517"));
        for (int i = 0; i < 12; ++i) *set << ventesParMois[i];

        QBarSeries *s = new QBarSeries();
        s->append(set);

        QChart *chart = new QChart();
        chart->addSeries(s);
        chart->setTitle("Ventes par mois");
        chart->setTitleFont(QFont("Arial", 9, QFont::Bold));
        chart->setAnimationOptions(QChart::SeriesAnimations);
        chart->legend()->setVisible(false);
        chart->setMargins(QMargins(4, 4, 4, 4));
        chart->setBackgroundVisible(false);

        QBarCategoryAxis *axX = new QBarCategoryAxis();
        axX->append(moisLabels);
        axX->setLabelsFont(QFont("Arial", 7));
        chart->addAxis(axX, Qt::AlignBottom);
        s->attachAxis(axX);

        // ── Axe Y : entiers uniquement, range propre ──────────
        int maxVal = *std::max_element(ventesParMois.begin(), ventesParMois.end());
        int yMax   = qMax(maxVal + 1, 5);  // minimum 5 pour éviter axe vide

        QValueAxis *axY = new QValueAxis();
        axY->setRange(0, yMax);
        axY->setTickCount(qMin(yMax + 1, 6));
        axY->setLabelFormat("%i");          // entiers, pas de décimales
        axY->setLabelsFont(QFont("Arial", 7));
        chart->addAxis(axY, Qt::AlignLeft);
        s->attachAxis(axY);

        QChartView *v = new QChartView(chart, card);
        v->setRenderHint(QPainter::Antialiasing);
        v->setMinimumHeight(220);
        v->setMaximumHeight(260);
        v->setBackgroundBrush(Qt::transparent);
        cl->addWidget(v);
        chartRow->addWidget(card);
    }

    contentLayout->addLayout(chartRow);
    // ════════════════════════════════════════════════════════════
    // LIGNE 3 — ACTIONS RAPIDES
    // ════════════════════════════════════════════════════════════
    QLabel *actionsTitle = new QLabel("⚡  Actions rapides");
    actionsTitle->setStyleSheet(
        "color: #556B2F; font-size: 13px; font-weight: bold; background: transparent;");
    contentLayout->addWidget(actionsTitle);

    QHBoxLayout *actionsRow = new QHBoxLayout();
    actionsRow->setSpacing(10);

    // Helper : fabrique un bouton action
    auto makeActionBtn = [&](const QString &icon, const QString &label,
                              const QString &color, QWidget *targetPage) {
        QPushButton *btn = new QPushButton();
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        btn->setFixedHeight(52);
        btn->setText(icon + "  " + label);
        btn->setStyleSheet(QString(
            "QPushButton {"
            "  background: %1;"
            "  color: white;"
            "  border-radius: 10px;"
            "  font-size: 12px;"
            "  font-weight: bold;"
            "  border: none;"
            "  text-align: center;"
            "}"
            "QPushButton:hover {"
            "  background: white;"
            "  color: %1;"
            "  border: 2px solid %1;"
            "}"
            "QPushButton:pressed {"
            "  opacity: 0.8;"
            "}").arg(color));

        connect(btn, &QPushButton::clicked, this, [=]() {
            ui->stackedWidget->setCurrentWidget(targetPage);
        });
        return btn;
    };

    actionsRow->addWidget(makeActionBtn("👤", "Ajouter employé",      "#378ADD", ui->page_ajout_employe));
    actionsRow->addWidget(makeActionBtn("🌿", "Ajouter agriculteur",  "#639922", ui->page_agriculteurs));
    actionsRow->addWidget(makeActionBtn("🛒", "Ajouter client",       "#D4537E", ui->page_clients_2));
    actionsRow->addWidget(makeActionBtn("💰", "Ajouter vente",        "#BA7517", ui->page_ventes));
    actionsRow->addWidget(makeActionBtn("🏭", "Ajouter production",   "#1D9E75", ui->page_production));

    QFrame *actionsFrame = new QFrame(content);
    actionsFrame->setStyleSheet(
        "QFrame { background: white; border-radius: 12px; border: 1px solid #e0e0e0; }");
    QVBoxLayout *actFl = new QVBoxLayout(actionsFrame);
    actFl->setContentsMargins(14, 12, 14, 12);
    actFl->addLayout(actionsRow);

    contentLayout->addWidget(actionsFrame);
    contentLayout->addStretch();

    // ── Finaliser ─────────────────────────────────────────────
    scrollArea->setWidget(content);

    QVBoxLayout *zoneLayout = new QVBoxLayout(contentZone);
    zoneLayout->setContentsMargins(0, 0, 0, 0);
    zoneLayout->addWidget(scrollArea);

    contentZone->show();
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ── Démarrer sur login ─────────────────────────────────────
    ui->stackedWidget->setCurrentWidget(ui->login);

    // ── Charger le dashboard quand on y arrive ─────────────────
    connect(ui->stackedWidget, &QStackedWidget::currentChanged,
            this, [=](int) {
        if (ui->stackedWidget->currentWidget() == ui->dashboard)
            loadDashboardStats();
    });

    // ── Sidebar du DASHBOARD (sidebar_3) ──────────────────────
    connect(ui->btn_home_3,         &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->dashboard);
    });
    connect(ui->btn_employes_3,     &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->page_employes);
    });
    connect(ui->btn_agriculteurs_3, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->page_agriculteurs);
    });
    connect(ui->btn_clients_3,      &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->page_clients_2);
    });
    connect(ui->btn_production_3,   &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->page_production);
    });
    connect(ui->btn_ventes_3,       &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->page_ventes);
    });
    connect(ui->btn_logout_3,       &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->login);
    });

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
    //ui->label_54->setPixmap(
       // QPixmap(":/images/images/lg-removebg-preview.png"));
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

    //chatbot
    // ── Bouton Chatbot Global (cercle + robot) ──────────────────
    QPushButton *btnChatbotGlobal = new QPushButton("🤖", this);
    btnChatbotGlobal->setFixedSize(60, 60);
    btnChatbotGlobal->setCursor(Qt::PointingHandCursor);
    btnChatbotGlobal->setToolTip("Assistant Intelligent");
    btnChatbotGlobal->setStyleSheet(
        "QPushButton {"
        "  background-color: #556B2F;"
        "  color: white;"
        "  border-radius: 30px;"          /* cercle parfait */
        "  font-size: 28px;"              /* taille du robot */
        "  border: 3px solid #D4AF37;"    /* bordure dorée */
        "}"
        "QPushButton:hover {"
        "  background-color: #D4AF37;"
        "  border: 3px solid #556B2F;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #3a4d1f;"
        "}"
    );

    // Position : coin bas droite de la fenêtre
    btnChatbotGlobal->setParent(this);
    btnChatbotGlobal->raise();   // toujours au-dessus
    btnChatbotGlobal->move(
        this->width()  - 80,   // 80px depuis la droite
        this->height() - 80    // 80px depuis le bas
    );


    connect(btnChatbotGlobal, &QPushButton::clicked,
            this, &MainWindow::on_btn_chatbot_global_clicked);






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


    // ── Ventes avancées ──────────────────────────────────────────
    connect(ui->vente_rechercheClientBtn,   &QPushButton::clicked,
            this, &MainWindow::on_vente_rechercheClientBtn_clicked);
    connect(ui->vente_rechercheDateBtn,     &QPushButton::clicked,
            this, &MainWindow::on_vente_rechercheDateBtn_clicked);
    connect(ui->vente_triBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_vente_triBox_currentIndexChanged);
    connect(ui->vente_exportCSVBtn,         &QPushButton::clicked,
            this, &MainWindow::on_vente_exportCSVBtn_clicked);
    connect(ui->vente_exportPDFBtn,         &QPushButton::clicked,
            this, &MainWindow::on_vente_exportPDFBtn_clicked);
    connect(ui->pushButton_12,              &QPushButton::clicked,
            this, &MainWindow::on_listerVentes);
    connect(ui->vente_analyserCABtn,        &QPushButton::clicked,
            this, &MainWindow::on_vente_analyserCABtn_clicked);
    connect(ui->vente_detecterAnomaliesBtn, &QPushButton::clicked,
            this, &MainWindow::on_vente_detecterAnomaliesBtn_clicked);
    connect(ui->vente_previsionBtn,         &QPushButton::clicked,
            this, &MainWindow::on_vente_previsionBtn_clicked);
    connect(ui->vente_graphiqueCABtn,       &QPushButton::clicked,
            this, &MainWindow::on_vente_graphiqueCABtn_clicked);

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
    ui->pushButton_recherche->show();
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


        // Agrandir le tableau pour qu'il occupe l'espace libéré (jusqu'aux boutons)
        int tableY = ui->tableWidget->y();
        ui->tableWidget->resize(ui->tableWidget->width(), targetY - tableY - 15);
    }


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


    // Arduino
    m_arduino = new Arduino(this);
    connect(m_arduino, &Arduino::statutChanged,    this, &MainWindow::onArduinoStatut);
    connect(m_arduino, &Arduino::formulaireRecu,   this, &MainWindow::onArduinoFormulaire);
    connect(m_arduino, &Arduino::greenCountChanged, this, &MainWindow::onGreenCountChanged);
    connect(m_arduino, &Arduino::blackCountChanged, this, &MainWindow::onBlackCountChanged);
    setupArduinoUI();



}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    // Repositionner le bouton chatbot au resize
    QPushButton *btn = this->findChild<QPushButton*>("btnChatbotGlobal");
    if (btn) {
        btn->move(this->width() - 80, this->height() - 80);
    }

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
        QMessageBox::warning(this, "Erreur", "Remplissez tous les champs !");
        return;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT ID_EMP FROM SMART.EMPLOYE "
        "WHERE TRIM(CIN) = :cin AND MOT_DE_PASSE = :mdp"
    );
    query.bindValue(":cin", cin);
    query.bindValue(":mdp", mdp);

    if (!query.exec()) {
        qDebug() << "ERREUR SQL :" << query.lastError().text();
        return;
    }

    if (query.next()) {
        // ── Login réussi ──────────────────────────────────────
        QPushButton *btn = this->findChild<QPushButton*>("btnChatbotGlobal");
        if (btn) btn->setVisible(true);

        ui->stackedWidget->setCurrentWidget(ui->dashboard);
        QTimer::singleShot(100, this, [this]() {
            loadDashboardStats();
        });
    } else {
        QMessageBox::warning(this, "Erreur", "CIN ou mot de passe incorrect !");
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
    ui->stackedWidget->setCurrentWidget(ui->page_analyse_client);
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
    ui->TABLEAG->resizeColumnsToContents();
    ui->TABLEAG->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::on_btn_tri_agri_clicked()
{
    static bool ascending = true;
    QString order = ascending ? "ASC" : "DESC";
    ui->TABLEAG->setModel(Atmp.trier("VOLUME_LIVRAISON " + order));
    ui->TABLEAG->resizeColumnsToContents();
    ui->TABLEAG->horizontalHeader()->setStretchLastSection(true);
    ascending = !ascending;
}

void MainWindow::refreshTableAgriculteur()
{
    ui->TABLEAG->setModel(Atmp.afficher());

    // ── Adapter les colonnes automatiquement ──────────────────
    ui->TABLEAG->resizeColumnsToContents();
    ui->TABLEAG->horizontalHeader()->setStretchLastSection(true);
    ui->TABLEAG->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->TABLEAG->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->TABLEAG->setAlternatingRowColors(true);
    ui->TABLEAG->verticalHeader()->setVisible(false);

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


//_______________________RFID__________________________

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
void MainWindow::on_exporter1_clicked()
{
     afficher_historique();
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

// ================================================================
// SLOT 1 — Afficher Statistiques & Graphiques
// Connecte au bouton "btn_stats_agri" ou "pushButton_20"
// ================================================================
void MainWindow::on_btn_stats_agri_clicked()
{
    // Créer un dialog avec onglets
    QDialog *dlg = new QDialog(this);
    dlg->setWindowTitle("📊 Statistiques Agriculteurs");
    dlg->setMinimumSize(900, 600);
    dlg->setStyleSheet(
        "QDialog { background-color: #f5f5f0; }"
        "QTabWidget::pane { border: 2px solid #D4AF37; border-radius: 6px; }"
        "QTabBar::tab { background: #556B2F; color: white; padding: 8px 16px; "
        "               border-radius: 4px; margin: 2px; font-weight: bold; }"
        "QTabBar::tab:selected { background: #D4AF37; color: black; }"
        "QLabel { color: #333; font-weight: bold; }"
        "QPushButton { background-color: #556B2F; color: white; border-radius: 6px; "
        "              padding: 8px 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #6B8E23; }"
        );

    QVBoxLayout *mainLayout = new QVBoxLayout(dlg);

    // Titre
    QLabel *titre = new QLabel("📊 Analyse des Agriculteurs — Smart Oil Press");
    titre->setStyleSheet("font-size:16px; font-weight:bold; color:#556B2F; "
                         "padding:10px; background:#e8f0d8; border-radius:6px;");
    titre->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titre);

    // Stats globales
    QMap<QString, double> stats = Agriculteur::statsGlobales();

    QHBoxLayout *kpiRow = new QHBoxLayout();
    auto makeKPI = [&](const QString& lbl, const QString& val, const QString& bg) {
        QFrame *f = new QFrame();
        f->setStyleSheet(QString("QFrame{background:%1;border-radius:8px;border:1px solid #ccc;}").arg(bg));
        f->setMinimumHeight(70);
        QVBoxLayout *l = new QVBoxLayout(f);
        l->setContentsMargins(10,8,10,8);
        QLabel *ll = new QLabel(lbl);
        ll->setStyleSheet("font-size:10px;color:#555;background:transparent;");
        QLabel *vl = new QLabel(val);
        vl->setStyleSheet("font-size:20px;font-weight:bold;color:#1a1a2e;background:transparent;");
        vl->setAlignment(Qt::AlignCenter);
        l->addWidget(ll);
        l->addWidget(vl);
        return f;
    };

    kpiRow->addWidget(makeKPI("Total Agriculteurs",
                              QString::number((int)stats.value("total",0)),
                              "#EAF3DE"));
    kpiRow->addWidget(makeKPI("Volume Total (L)",
                              QString::number(stats.value("volume_total",0),'f',1),
                              "#E8F4FD"));
    kpiRow->addWidget(makeKPI("Volume Moyen (L)",
                              QString::number(stats.value("volume_moyen",0),'f',1),
                              "#FEF9E7"));
    kpiRow->addWidget(makeKPI("Nb Régions",
                              QString::number((int)stats.value("nb_regions",0)),
                              "#F5EEF8"));
    kpiRow->addWidget(makeKPI("⚠️ Alertes Volume",
                              QString::number((int)stats.value("alertes_volume",0)),
                              "#FDEDEC"));
    kpiRow->addWidget(makeKPI("⏰ Alertes Retard",
                              QString::number((int)stats.value("alertes_retard",0)),
                              "#FDEDEC"));
    mainLayout->addLayout(kpiRow);

    // Onglets graphiques
    QTabWidget *tabs = new QTabWidget();

    // ── Onglet 1 : Volume par Région (Bar Chart) ──────────────────
    {
        QMap<QString, double> volRegion = Agriculteur::volumeParRegion();
        QBarSet *set = new QBarSet("Volume (L)");
        set->setColor(QColor("#556B2F"));

        QStringList categories;
        for (auto it = volRegion.begin(); it != volRegion.end(); ++it) {
            categories << it.key();
            *set << it.value();
        }

        QBarSeries *series = new QBarSeries();
        series->append(set);

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Volume livré par Région (L)");
        chart->setTitleFont(QFont("Arial", 11, QFont::Bold));
        chart->setAnimationOptions(QChart::SeriesAnimations);
        chart->legend()->setVisible(false);

        QBarCategoryAxis *axX = new QBarCategoryAxis();
        axX->append(categories);
        chart->addAxis(axX, Qt::AlignBottom);
        series->attachAxis(axX);

        QValueAxis *axY = new QValueAxis();
        axY->setLabelFormat("%.0f L");
        chart->addAxis(axY, Qt::AlignLeft);
        series->attachAxis(axY);

        QChartView *view = new QChartView(chart);
        view->setRenderHint(QPainter::Antialiasing);
        tabs->addTab(view, "🗺️ Volume / Région");
    }

    // ── Onglet 2 : Répartition par Type d'Olive (Pie Chart) ──────
    {
        QMap<QString, int> typeOlive = Agriculteur::countParTypeOlive();
        QPieSeries *series = new QPieSeries();
        series->setHoleSize(0.4);

        QList<QColor> cols = {QColor("#556B2F"), QColor("#D4AF37"), QColor("#8B4513"),
                              QColor("#2E8B57"), QColor("#B8860B")};
        int idx = 0;
        for (auto it = typeOlive.begin(); it != typeOlive.end(); ++it) {
            QPieSlice *slice = series->append(
                QString("%1 (%2)").arg(it.key()).arg(it.value()),
                it.value()
                );
            slice->setBrush(cols[idx % cols.size()]);
            slice->setLabelVisible(true);
            idx++;
        }

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Répartition par Type d'Olive");
        chart->setTitleFont(QFont("Arial", 11, QFont::Bold));
        chart->setAnimationOptions(QChart::SeriesAnimations);
        chart->legend()->setAlignment(Qt::AlignRight);

        QChartView *view = new QChartView(chart);
        view->setRenderHint(QPainter::Antialiasing);
        tabs->addTab(view, "🫒 Type d'Olive");
    }

    // ── Onglet 3 : Top 10 Agriculteurs (table) ───────────────────
    {
        QWidget *topWidget = new QWidget();
        QVBoxLayout *topLayout = new QVBoxLayout(topWidget);

        QLabel *topTitle = new QLabel("🏆 Top 10 Agriculteurs par Volume Livré");
        topTitle->setStyleSheet("font-size:13px; font-weight:bold; color:#556B2F; padding:8px;");
        topLayout->addWidget(topTitle);

        QTableWidget *topTable = new QTableWidget();
        topTable->setColumnCount(6);
        topTable->setHorizontalHeaderLabels({"#", "CIN", "Nom", "Prénom", "Région", "Volume (L)"});
        topTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        topTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        topTable->setAlternatingRowColors(true);
        topTable->setStyleSheet(
            "QTableWidget { border: 2px solid #D4AF37; border-radius: 6px; }"
            "QHeaderView::section { background-color: #556B2F; color: white; "
            "                       padding: 6px; font-weight: bold; }"
            "QTableWidget::item:alternate { background-color: #f0f4e8; }"
            );

        QList<QMap<QString,QString>> top = Agriculteur::topAgriculteurs(10);
        topTable->setRowCount(top.size());

        for (int i = 0; i < top.size(); i++) {
            auto &row = top[i];
            topTable->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
            topTable->setItem(i, 1, new QTableWidgetItem(row["cin"]));
            topTable->setItem(i, 2, new QTableWidgetItem(row["nom"]));
            topTable->setItem(i, 3, new QTableWidgetItem(row["prenom"]));
            topTable->setItem(i, 4, new QTableWidgetItem(row["region"]));
            topTable->setItem(i, 5, new QTableWidgetItem(row["volume"] + " L"));

            // Médailles pour top 3
            if (i == 0) topTable->item(i,0)->setForeground(QColor("#FFD700"));
            else if (i == 1) topTable->item(i,0)->setForeground(QColor("#C0C0C0"));
            else if (i == 2) topTable->item(i,0)->setForeground(QColor("#CD7F32"));

            topTable->item(i,0)->setTextAlignment(Qt::AlignCenter);
        }

        topLayout->addWidget(topTable);
        tabs->addTab(topWidget, "🏆 Top 10");
    }

    mainLayout->addWidget(tabs);

    // Boutons bas
    QHBoxLayout *btns = new QHBoxLayout();
    QPushButton *btnPDF    = new QPushButton("📄 Exporter PDF");
    QPushButton *btnFermer = new QPushButton("Fermer");
    btnFermer->setStyleSheet("background-color:#888; color:white; border-radius:6px; padding:8px 16px;");

    btns->addStretch();
    btns->addWidget(btnPDF);
    btns->addWidget(btnFermer);
    mainLayout->addLayout(btns);

    connect(btnFermer, &QPushButton::clicked, dlg, &QDialog::close);
    connect(btnPDF, &QPushButton::clicked, [=]() {
        QString chemin = QFileDialog::getSaveFileName(
            dlg, "Exporter PDF Agriculteurs",
            QDir::homePath() + "/agriculteurs_" +
                QDateTime::currentDateTime().toString("yyyyMMdd_HHmm") + ".pdf",
            "PDF (*.pdf)"
            );
        if (chemin.isEmpty()) return;

        if (Agriculteur::exporterPDF(chemin)) {
            QMessageBox::information(dlg, "Succès",
                                     "PDF exporté avec succès !\n" + chemin);
        } else {
            QMessageBox::critical(dlg, "Erreur", "Impossible de créer le PDF.");
        }
    });

    dlg->exec();
    delete dlg;
}

// ================================================================
// SLOT 2 — Afficher Alertes
// Connecte au bouton "btn_alertes_agri"
// ================================================================
void MainWindow::on_btn_alertes_agri_clicked()
{
    QDialog *dlg = new QDialog(this);
    dlg->setWindowTitle("🔔 Alertes Agriculteurs");
    dlg->setMinimumSize(850, 500);
    dlg->setStyleSheet(
        "QDialog { background-color: #fff8f0; }"
        "QTabWidget::pane { border: 2px solid #E74C3C; border-radius: 6px; }"
        "QTabBar::tab { background: #E74C3C; color: white; padding: 8px 16px; "
        "               border-radius: 4px; margin: 2px; font-weight: bold; }"
        "QTabBar::tab:selected { background: #C0392B; }"
        "QPushButton { background-color: #556B2F; color: white; border-radius: 6px; "
        "              padding: 8px 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #6B8E23; }"
        );

    QVBoxLayout *mainLayout = new QVBoxLayout(dlg);

    QLabel *titre = new QLabel("⚠️ Centre d'Alertes — Agriculteurs");
    titre->setStyleSheet("font-size:15px; font-weight:bold; color:#C0392B; "
                         "padding:10px; background:#FDEDEC; border-radius:6px;");
    titre->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titre);

    QTabWidget *tabs = new QTabWidget();

    // Helper pour créer une tab avec QTableView
    auto makeAlertTab = [&](QSqlQueryModel* model, const QString& emptyMsg) -> QWidget* {
        QWidget *w = new QWidget();
        QVBoxLayout *l = new QVBoxLayout(w);

        if (model->rowCount() == 0) {
            QLabel *ok = new QLabel("✅ " + emptyMsg);
            ok->setStyleSheet("font-size:14px; color:#27AE60; padding:20px; font-weight:bold;");
            ok->setAlignment(Qt::AlignCenter);
            l->addWidget(ok);
        } else {
            QLabel *nb = new QLabel(QString("⚠️ %1 agriculteur(s) concerné(s)").arg(model->rowCount()));
            nb->setStyleSheet("color:#E74C3C; font-weight:bold; padding:6px;");
            l->addWidget(nb);

            QTableView *tv = new QTableView();
            tv->setModel(model);
            tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
            tv->setAlternatingRowColors(true);
            tv->setSelectionBehavior(QAbstractItemView::SelectRows);
            tv->setStyleSheet(
                "QTableView { border: 1px solid #E74C3C; border-radius:4px; }"
                "QHeaderView::section { background:#E74C3C; color:white; padding:6px; font-weight:bold; }"
                "QTableView::item:alternate { background:#FEF9E7; }"
                );
            l->addWidget(tv);
        }
        return w;
    };

    // Onglet 1 : Volume faible
    {
        QWidget *w = new QWidget();
        QVBoxLayout *l = new QVBoxLayout(w);

        QHBoxLayout *seuilRow = new QHBoxLayout();
        QLabel *seuilLbl = new QLabel("Seuil d'alerte (L) :");
        seuilLbl->setStyleSheet("font-weight:bold;");
        QDoubleSpinBox *seuilSpin = new QDoubleSpinBox();
        seuilSpin->setRange(0, 99999);
        seuilSpin->setValue(100.0);
        seuilSpin->setSuffix(" L");
        QPushButton *btnAppliquer = new QPushButton("🔍 Appliquer");
        seuilRow->addWidget(seuilLbl);
        seuilRow->addWidget(seuilSpin);
        seuilRow->addWidget(btnAppliquer);
        seuilRow->addStretch();
        l->addLayout(seuilRow);

        QTableView *tv = new QTableView();
        tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tv->setAlternatingRowColors(true);
        tv->setSelectionBehavior(QAbstractItemView::SelectRows);
        tv->setStyleSheet(
            "QTableView { border: 1px solid #E74C3C; border-radius:4px; }"
            "QHeaderView::section { background:#E74C3C; color:white; padding:6px; font-weight:bold; }"
            "QTableView::item:alternate { background:#FEF9E7; }"
            );

        // Affichage initial
        tv->setModel(Agriculteur::alertesVolumefaible(100.0));

        connect(btnAppliquer, &QPushButton::clicked, [=]() {
            tv->setModel(Agriculteur::alertesVolumefaible(seuilSpin->value()));
        });

        l->addWidget(tv);
        tabs->addTab(w, "📉 Volume Faible");
    }

    // Onglet 2 : Dates dépassées
    tabs->addTab(
        makeAlertTab(Agriculteur::alertesDateDepassee(),
                     "Aucun retard de livraison détecté"),
        "⏰ Retards Livraison"
        );

    // Onglet 3 : Inactifs
    {
        QWidget *w = new QWidget();
        QVBoxLayout *l = new QVBoxLayout(w);

        QHBoxLayout *joursRow = new QHBoxLayout();
        QLabel *joursLbl = new QLabel("Inactif depuis (jours) :");
        joursLbl->setStyleSheet("font-weight:bold;");
        QSpinBox *joursSpin = new QSpinBox();
        joursSpin->setRange(1, 3650);
        joursSpin->setValue(180);
        joursSpin->setSuffix(" jours");
        QPushButton *btnAppliquer2 = new QPushButton("🔍 Appliquer");
        joursRow->addWidget(joursLbl);
        joursRow->addWidget(joursSpin);
        joursRow->addWidget(btnAppliquer2);
        joursRow->addStretch();
        l->addLayout(joursRow);

        QTableView *tv2 = new QTableView();
        tv2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        tv2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tv2->setAlternatingRowColors(true);
        tv2->setSelectionBehavior(QAbstractItemView::SelectRows);
        tv2->setStyleSheet(
            "QTableView { border: 1px solid #E74C3C; border-radius:4px; }"
            "QHeaderView::section { background:#E74C3C; color:white; padding:6px; font-weight:bold; }"
            );

        tv2->setModel(Agriculteur::alertesInactifs(180));

        connect(btnAppliquer2, &QPushButton::clicked, [=]() {
            tv2->setModel(Agriculteur::alertesInactifs(joursSpin->value()));
        });

        l->addWidget(tv2);
        tabs->addTab(w, "😴 Agriculteurs Inactifs");
    }

    mainLayout->addWidget(tabs);

    QPushButton *btnFermer = new QPushButton("Fermer");
    btnFermer->setStyleSheet("background-color:#888; color:white; border-radius:6px; padding:8px 16px;");
    mainLayout->addWidget(btnFermer, 0, Qt::AlignRight);

    connect(btnFermer, &QPushButton::clicked, dlg, &QDialog::close);

    dlg->exec();
    delete dlg;
}

// ================================================================
// SLOT 3 — Export PDF Direct
// Connecte au bouton "btn_pdf_agri"
// ================================================================
void MainWindow::on_btn_pdf_agri_clicked()
{
    QString chemin = QFileDialog::getSaveFileName(
        this,
        "Exporter Rapport Agriculteurs en PDF",
        QDir::homePath() + "/agriculteurs_" +
            QDateTime::currentDateTime().toString("yyyyMMdd_HHmm") + ".pdf",
        "PDF (*.pdf)"
        );

    if (chemin.isEmpty()) return;

    if (Agriculteur::exporterPDF(chemin)) {
        QMessageBox::information(this, "Succès",
                                 "📄 PDF exporté avec succès !\n\nFichier :\n" + chemin);
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le PDF.");
    }
}

// ================================================================
// SLOT 4 — Filtre par Région
// Connecte au comboBox_region_agri (currentTextChanged)
// ================================================================
void MainWindow::on_comboBox_region_agri_currentTextChanged(const QString &region)
{
    if (region.isEmpty() || region == "Toutes les régions") {
        ui->TABLEAG->setModel(Atmp.afficher());
    } else {
        ui->TABLEAG->setModel(Agriculteur::filtrerParRegion(region));
    }
}

// ================================================================
// SLOT 5 — Charger régions dans comboBox (à appeler dans refreshTableAgriculteur)
// ================================================================
void MainWindow::chargerRegionsAgri()
{
    // Supposant que tu as un comboBox nommé comboBox_region_agri dans l'UI
    // Sinon adapte le nom
    // ui->comboBox_region_agri->clear();
    // ui->comboBox_region_agri->addItem("Toutes les régions");
    // for (const QString &r : Agriculteur::listeRegions()) {
    //     ui->comboBox_region_agri->addItem(r);
    // }
}



// ─── RECHERCHE PAR CLIENT ─────────────────────────────────────
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
                                 "Aucune vente trouvée pour le client ID : " + QString::number(idClient));
}

// ─── RECHERCHE PAR DATE ───────────────────────────────────────
void MainWindow::on_vente_rechercheDateBtn_clicked()
{
    QDate date = ui->vente_dateRechercheEdit->date();
    QSqlQueryModel* model = Vente::rechercherParDate(date);
    ui->tableView_vente->setModel(model);
    ui->tableView_vente->resizeColumnsToContents();
}

// ─── TRI ──────────────────────────────────────────────────────
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

// ─── EXPORT CSV ───────────────────────────────────────────────
void MainWindow::on_vente_exportCSVBtn_clicked()
{
    QString fichier = QFileDialog::getSaveFileName(
        this, "Exporter en CSV", "", "Fichiers CSV (*.csv)");
    if (fichier.isEmpty()) return;
    if (!fichier.endsWith(".csv")) fichier += ".csv";
    if (Vente::exporterCSV(fichier))
        QMessageBox::information(this, "Export CSV", "Export réussi !");
    else
        QMessageBox::critical(this, "Export CSV", "Échec de l'export.");
}

// ─── EXPORT PDF ───────────────────────────────────────────────
void MainWindow::on_vente_exportPDFBtn_clicked()
{
    QString fichier = QFileDialog::getSaveFileName(
        this, "Exporter en PDF", "", "Fichiers PDF (*.pdf)");
    if (fichier.isEmpty()) return;
    if (!fichier.endsWith(".pdf")) fichier += ".pdf";
    if (Vente::exporterPDF(fichier, "Liste des ventes"))
        QMessageBox::information(this, "Export PDF", "Export réussi !");
    else
        QMessageBox::critical(this, "Export PDF", "Échec de l'export.");
}

// ─── ANALYSE CA ───────────────────────────────────────────────
void MainWindow::on_vente_analyserCABtn_clicked()
{
    QDate debut = ui->vente_dateDebutAnalyse->date();
    QDate fin   = ui->vente_dateFinAnalyse->date();
    Vente::StatsCA stats = Vente::calculerChiffreAffaires(debut, fin);
    QString msg = QString(
                      "📊 Chiffre d'affaires du %1 au %2\n\n"
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

// ─── ANOMALIES ────────────────────────────────────────────────
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
    for (const Vente& v : anomalies)
        liste += QString("Vente %1 — Client %2 — Montant %3 DT\n")
                     .arg(v.getIdVente()).arg(v.getIdClient())
                     .arg(v.getMontantPaye(), 0, 'f', 2);
    QMessageBox::information(this, "Ventes anormales détectées", liste);
}

// ─── PREVISION ────────────────────────────────────────────────
void MainWindow::on_vente_previsionBtn_clicked()
{
    double prev = Vente::prevoirVentesMoisProchain();
    QMessageBox::information(this, "Prévision CA",
                             QString("Prévision du CA pour les 30 prochains jours :\n%1 DT")
                                 .arg(prev, 0, 'f', 2));
}

// ─── GRAPHIQUE CA ─────────────────────────────────────────────
void MainWindow::on_vente_graphiqueCABtn_clicked()
{
    QSqlQuery rangeQuery;
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

    QLineSeries *series = new QLineSeries();
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

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(QString("Évolution du CA — %1 → %2")
                        .arg(debut.toString("dd/MM/yyyy"))
                        .arg(fin.toString("dd/MM/yyyy")));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("dd/MM/yy");
    axisX->setTitleText("Date");
    axisX->setRange(QDateTime::fromMSecsSinceEpoch(minMs),
                    QDateTime::fromMSecsSinceEpoch(maxMs));
    axisX->setTickCount(qMin((int)data.size(), 10));
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText("Montant (DT)");
    axisY->setRange(0, maxVal * 1.2);
    axisY->setLabelFormat("%.2f");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QWidget *win = new QWidget(nullptr, Qt::Window);
    win->setWindowTitle("Graphique CA");
    win->resize(1000, 600);
    win->setAttribute(Qt::WA_DeleteOnClose);

    QVBoxLayout *layout = new QVBoxLayout(win);
    layout->setContentsMargins(10, 10, 10, 10);

    double total = std::accumulate(data.begin(), data.end(), 0.0,
                                   [](double s, const QPair<QDate,double>& p){ return s + p.second; });

    QLabel *lblInfo = new QLabel(QString("  %1 jours  |  Total : %2 DT  |  Moyenne/jour : %3 DT")
                                     .arg(data.size())
                                     .arg(QString::number(total, 'f', 2))
                                     .arg(QString::number(total / data.size(), 'f', 2)));
    lblInfo->setStyleSheet("background:#556B2F; color:white; padding:6px; font-weight:bold;");
    layout->addWidget(lblInfo);

    QChartView *chartView = new QChartView(chart, win);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(chartView);
    win->show();
}


void MainWindow::on_VERIFIER_clicked()
{

        // Métier avancé: Rappel des dates de livraison les plus proches (dans les 2 jours)
        // Se déclenche uniquement sur le bouton VERIFIER
        QSqlQuery query;
        // Sélectionne les livraisons entre aujourd'hui et aujourd'hui + 2 jours
        query.prepare("SELECT CIN, NOM, PRENOM, TELEPHONE, REGION, DATE_LIVRAISON "
                      "FROM AGRICULTEUR "
                      "WHERE DATE_LIVRAISON >= TRUNC(SYSDATE) "
                      "AND DATE_LIVRAISON <= TRUNC(SYSDATE) + 2 "
                      "ORDER BY DATE_LIVRAISON ASC");

        if (query.exec()) {
            ui->tableWidget_3->setRowCount(0); // clear existing rows
            ui->tableWidget_3->setColumnCount(6);
            ui->tableWidget_3->setHorizontalHeaderLabels({"CIN", "Nom", "Prénom", "Téléphone", "Région", "Date Livraison"});

            int row = 0;
            while (query.next()) {
                ui->tableWidget_3->insertRow(row);
                ui->tableWidget_3->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
                ui->tableWidget_3->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
                ui->tableWidget_3->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
                ui->tableWidget_3->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
                ui->tableWidget_3->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));

                // Formatage de la date en chaîne lisible
                QString dateStr = query.value(5).toDate().toString("dd/MM/yyyy");
                ui->tableWidget_3->setItem(row, 5, new QTableWidgetItem(dateStr));

                row++;
            }

            // Ajustement automatique des colonnes
            ui->tableWidget_3->resizeColumnsToContents();

            if (row == 0) {
                QMessageBox::information(this, "Rappel Livraisons", "Aucune livraison prévue dans les 2 prochains jours.");
            }
        } else {
            qDebug() << "Erreur Rappel Livraisons :" << query.lastError().text();
        }
}



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
            ui->stackedWidget->setCurrentWidget(ui->page_production);
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
                        .arg(q.value(0).toString(),
                             QString::number(q.value(1).toDouble(), 'f', 2));
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
                              .arg(idOperation, QString::number(rendement, 'f', 2));

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
                        .arg(rendement, temperature, duree, typeHuile);

    QJsonObject jsonBody;
    jsonBody["model"] = "llama3.2";
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

void MainWindow::onConnectArduinoClicked()
{
    QWidget *page = ui->stackedWidget->widget(0);
    QComboBox *cb = page->findChild<QComboBox *>("cb_arduino_port");
    if (!cb || cb->currentText().isEmpty()) {
        QMessageBox::warning(this, "Arduino", "Aucun port série disponible !");
        return;
    }
    if (!m_arduino->connecter(cb->currentText())) {
        QMessageBox::critical(this, "Arduino", "Connexion échouée !");
    }
}

void MainWindow::onDisconnectArduinoClicked()
{
    m_arduino->deconnecter();
}

void MainWindow::onArduinoStatut(const QString &msg, bool connecte)
{
    QWidget *page = ui->stackedWidget->widget(0);
    QLabel *lbl = page->findChild<QLabel *>("lbl_arduino_status");
    if (lbl) {
        lbl->setText(msg);
        lbl->setStyleSheet(connecte
                               ? "color: #44cc44; font-weight: 700;"
                               : "color: #cc4444; font-weight: 700;");
    }
}

void MainWindow::onArduinoFormulaire(QString idOpStr, QString idAgriStr, QString idMachStr)
{
    ui->ID_Operation->setText(idOpStr);
    ui->ID_Agriculteur->setText(idAgriStr);
    ui->ID_machin->setText(idMachStr);

    int idOp = 0, idAgri = 0, idMach = 0;
    parsePositiveIntId(idOpStr, &idOp);
    parsePositiveIntId(idAgriStr, &idAgri);
    parsePositiveIntId(idMachStr, &idMach);

    int totalQuantity = (m_blackCount * 10) + (m_greenCount * 15);
    double oilQuantity = totalQuantity * 0.15; // 1 kg = 0.15 L

    ui->Quantite_olives->setText(QString::number(totalQuantity));
    ui->Quantite_Huile->setText(QString::number(oilQuantity, 'f', 2)); // Afficher 2 décimales

    bool dbUpdated = false;
    bool dbInserted = false;
    QString lastError = "";
    Connection conn;
    if (conn.createConnection()) {
        QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
        QSqlQuery checkQuery(db);
        QString sqlCheck = QString("SELECT COUNT(*) FROM PRODUCTION WHERE ID_OPERATION = %1").arg(idOp);
        if (checkQuery.exec(sqlCheck) && checkQuery.next()) {
            if (checkQuery.value(0).toInt() > 0) {
                QSqlQuery updateQuery(db);
                // Mise à jour de la quantité d'olives ET de la quantité d'huile
                QString sqlUpdate = QString("UPDATE PRODUCTION SET QUANTITE_OLIVES = %1, QUANTITE_HUILE = %2 WHERE ID_OPERATION = %3")
                                        .arg(totalQuantity)
                                        .arg(oilQuantity)
                                        .arg(idOp);
                if (updateQuery.exec(sqlUpdate)) {
                    dbUpdated = true;
                    db.commit();
                } else {
                    lastError = updateQuery.lastError().text();
                }
            } else {
                // Si l'ID n'existe pas, on insère une nouvelle ligne avec olives et huile
                QSqlQuery insertQuery(db);
                QString dateStr = QDate::currentDate().toString("dd/MM/yyyy");
                QString sqlInsert = QString("INSERT INTO PRODUCTION (ID_OPERATION, DATE_PRODUCTION, QUANTITE_OLIVES, QUANTITE_HUILE) VALUES (%1, TO_DATE('%2', 'DD/MM/YYYY'), %3, %4)")
                                        .arg(idOp)
                                        .arg(dateStr)
                                        .arg(totalQuantity)
                                        .arg(oilQuantity);
                if (insertQuery.exec(sqlInsert)) {
                    dbInserted = true;
                    db.commit();
                } else {
                    lastError = insertQuery.lastError().text();
                }
            }
            afficherOperations();
        }
    }

    QString infoMsg = QString("Formulaire lu et enregistré automatiquement !\n\n"
                              "ID Opération : %1\n"
                              "ID Agriculteur : %2\n"
                              "ID Machine : %3\n\n"
                              "Quantité d'olives initiale : %4 kg\n\n"
                              "%5")
                          .arg(idOp)
                          .arg(idAgri)
                          .arg(idMach)
                          .arg(totalQuantity)
                          .arg(dbInserted ? "✅ Nouvelle opération insérée en base." :
                              (dbUpdated ? "✅ Opération existante mise à jour en base." : "❌ Erreur BD: " + lastError));

    QMessageBox::information(this, "Arduino ✅", infoMsg);
}

void MainWindow::recalculateAndAutoUpdateOliveWeight()
{
    int totalQuantity = (m_blackCount * 10) + (m_greenCount * 15);
    ui->Quantite_olives->setText(QString::number(totalQuantity));

    QString idOpStr = ui->ID_Operation->text().trimmed();
    if (!idOpStr.isEmpty()) {
        int idOp = 0;
        if (parsePositiveIntId(idOpStr, &idOp)) {
            Connection conn;
            if (conn.createConnection()) {
                QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
                QSqlQuery query(db);
                QString sqlUpdate = QString("UPDATE PRODUCTION SET QUANTITE_OLIVES = %1 WHERE ID_OPERATION = %2").arg(totalQuantity).arg(idOp);
                if (query.exec(sqlUpdate)) {
                    db.commit();
                    if (query.numRowsAffected() > 0) {
                        afficherOperations();
                    }
                } else {
                    qDebug() << "Update Error: " << query.lastError().text();
                }
            }
        }
    }
}

void MainWindow::onGreenCountChanged(int count)
{
    m_greenCount = count;
    updateArduinoCountersLabel();
    recalculateAndAutoUpdateOliveWeight();
}

void MainWindow::onBlackCountChanged(int count)
{
    m_blackCount = count;
    updateArduinoCountersLabel();
    recalculateAndAutoUpdateOliveWeight();
}

void MainWindow::updateArduinoCountersLabel()
{
    QWidget *page = ui->stackedWidget->widget(0);
    if (!page) return;

    QLabel *lbl = page->findChild<QLabel *>("lbl_arduino_counters");
    if (lbl) {
        lbl->setText(QString("🟢 Vert: %1   ⚫ Noir: %2").arg(m_greenCount).arg(m_blackCount));
    }
}

void MainWindow::setupArduinoUI()
{
    // Chercher la page Arduino dans le stackedWidget
    QWidget *page = ui->page_production;
    if (!page) return;

    // --- Label Statut ---
    QLabel *lblStatus = new QLabel("⚫ Arduino déconnecté", page);
    lblStatus->setObjectName("lbl_arduino_status");
    lblStatus->setGeometry(680, 10, 300, 25); // Déplacé en haut à droite
    lblStatus->setStyleSheet("color: #cc4444; font-weight: 700;");

    // --- Label Compteurs ---
    QLabel *lblCounters = new QLabel("🟢 Vert: 0   ⚫ Noir: 0", page);
    lblCounters->setObjectName("lbl_arduino_counters");
    lblCounters->setGeometry(680, 40, 300, 25); // Déplacé en haut à droite
    lblCounters->setStyleSheet("color: #333; font-weight: 600; font-size: 14px;");

    // --- ComboBox Ports série ---
    QComboBox *cbPort = new QComboBox(page);
    cbPort->setObjectName("cb_arduino_port");
    cbPort->setGeometry(680, 70, 100, 30); // Déplacé en haut à droite

    // Remplir avec les ports disponibles
    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : ports) {
        cbPort->addItem(info.portName());
    }

    // --- Bouton Connecter ---
    QPushButton *btnConnect = new QPushButton("🔌 Connecter", page);
    btnConnect->setObjectName("btn_connect_arduino");
    btnConnect->setGeometry(790, 70, 110, 30); // Déplacé en haut à droite
    btnConnect->setStyleSheet(
        "QPushButton { background-color: #2e7d32; color: white; "
        "font-weight: bold; border-radius: 5px; }"
        "QPushButton:hover { background-color: #43a047; }"
        );
    connect(btnConnect, &QPushButton::clicked, this, &MainWindow::onConnectArduinoClicked);

    // --- Bouton Déconnecter ---
    QPushButton *btnDisconnect = new QPushButton("❌ Déconnexion", page);
    btnDisconnect->setObjectName("btn_disconnect_arduino");
    btnDisconnect->setGeometry(910, 70, 120, 30); // Déplacé en haut à droite
    btnDisconnect->setStyleSheet(
        "QPushButton { background-color: #c62828; color: white; "
        "font-weight: bold; border-radius: 5px; }"
        "QPushButton:hover { background-color: #e53935; }"
        );
    connect(btnDisconnect, &QPushButton::clicked, this, &MainWindow::onDisconnectArduinoClicked);
}


// ================================================================
// CHATBOT GLOBAL — Smart Oil Press
// À ajouter dans mainwindow.cpp
// Bouton : connectez n'importe quel bouton à on_btn_chatbot_global_clicked()
// ================================================================

// ----------------------------------------------------------------
// HELPER PRIVÉ : récupérer toutes les données de la BD
// (remplace getInfosEmploye — couvre tous les modules)
// ----------------------------------------------------------------
QString MainWindow::getContexteGlobal(const QString &question)
{
    QString contexte = "";

    // ── 1. EMPLOYES ──────────────────────────────────────────────
    {
        QSqlQuery q;
        q.exec("SELECT ID_EMP, CIN, NOM, PRENOM, DEPARTEMENT, POSTE, "
               "SALAIRE, PRIMES, STATUT FROM SMART.EMPLOYE");

        QString section = "";
        while (q.next()) {
            QString nom    = q.value(2).toString();
            QString prenom = q.value(3).toString();

            // Toujours inclure si la question mentionne le nom/prénom
            // OU si la question parle d'employés en général
            bool mentionné = question.contains(nom,    Qt::CaseInsensitive)
                          || question.contains(prenom, Qt::CaseInsensitive)
                          || question.contains("employ", Qt::CaseInsensitive)
                          || question.contains("salaire", Qt::CaseInsensitive)
                          || question.contains("prime",   Qt::CaseInsensitive)
                          || question.contains("statut",  Qt::CaseInsensitive);

            if (mentionné) {
                section += QString("EMPLOYE: ID=%1 | CIN=%2 | Nom=%3 %4 | "
                                   "Dept=%5 | Poste=%6 | Salaire=%7 DT | "
                                   "Primes=%8 DT | Statut=%9\n")
                               .arg(q.value(0).toString())
                               .arg(q.value(1).toString())
                               .arg(nom, prenom)
                               .arg(q.value(4).toString())
                               .arg(q.value(5).toString())
                               .arg(q.value(6).toString())
                               .arg(q.value(7).toString())
                               .arg(q.value(8).toString());
            }
        }
        if (!section.isEmpty())
            contexte += "=== EMPLOYES ===\n" + section + "\n";
    }

    // ── 2. AGRICULTEURS ──────────────────────────────────────────
    {
        QSqlQuery q;
        q.exec("SELECT CIN, NOM, PRENOM, REGION, TYPE_OLIVE, "
               "VOLUME_LIVRAISON, DATE_LIVRAISON FROM SMART.AGRICULTEUR");

        QString section = "";
        while (q.next()) {
            QString nom    = q.value(1).toString();
            QString prenom = q.value(2).toString();

            bool mentionné = question.contains(nom,    Qt::CaseInsensitive)
                          || question.contains(prenom, Qt::CaseInsensitive)
                          || question.contains("agri",   Qt::CaseInsensitive)
                          || question.contains("olive",  Qt::CaseInsensitive)
                          || question.contains("region", Qt::CaseInsensitive)
                          || question.contains("livrai", Qt::CaseInsensitive);

            if (mentionné) {
                section += QString("AGRICULTEUR: CIN=%1 | Nom=%2 %3 | "
                                   "Region=%4 | Type Olive=%5 | "
                                   "Volume=%6 L | Date Livraison=%7\n")
                               .arg(q.value(0).toString())
                               .arg(nom, prenom)
                               .arg(q.value(3).toString())
                               .arg(q.value(4).toString())
                               .arg(q.value(5).toString())
                               .arg(q.value(6).toDate().toString("dd/MM/yyyy"));
            }
        }
        if (!section.isEmpty())
            contexte += "=== AGRICULTEURS ===\n" + section + "\n";
    }

    // ── 3. CLIENTS ───────────────────────────────────────────────
    {
        QSqlQuery q;
        q.exec("SELECT ID, NOM, PRENOM, TYPE_CLIENT, "
               "TOTAL_ACHAT, POINTS_FIDELITE FROM SMART.CLIENT");

        QString section = "";
        while (q.next()) {
            QString nom    = q.value(1).toString();
            QString prenom = q.value(2).toString();

            bool mentionné = question.contains(nom,    Qt::CaseInsensitive)
                          || question.contains(prenom, Qt::CaseInsensitive)
                          || question.contains("client",  Qt::CaseInsensitive)
                          || question.contains("achat",   Qt::CaseInsensitive)
                          || question.contains("fidelit", Qt::CaseInsensitive)
                          || question.contains("point",   Qt::CaseInsensitive);

            if (mentionné) {
                section += QString("CLIENT: ID=%1 | Nom=%2 %3 | "
                                   "Type=%4 | Total Achat=%5 DT | "
                                   "Points Fidelite=%6\n")
                               .arg(q.value(0).toString())
                               .arg(nom, prenom)
                               .arg(q.value(3).toString())
                               .arg(q.value(4).toString())
                               .arg(q.value(5).toString());
            }
        }
        if (!section.isEmpty())
            contexte += "=== CLIENTS ===\n" + section + "\n";
    }

    // ── 4. PRODUCTIONS ───────────────────────────────────────────
    {
        QSqlQuery q;
        q.exec("SELECT ID_OPERATION, DATE_PRODUCTION, ID_AGRI, ID_EMP, "
               "QUANTITE_OLIVES, QUANTITE_HUILE, RENDEMENT, TYPE_HUILE, "
               "TEMPERATURE_MOYENNE, DUREE_PRESSAGE "
               "FROM PRODUCTION ORDER BY DATE_PRODUCTION DESC");

        QString section = "";
        int count = 0;
        while (q.next() && count < 20) { // limiter à 20 dernières
            bool mentionné = question.contains("product",   Qt::CaseInsensitive)
                          || question.contains("rendement", Qt::CaseInsensitive)
                          || question.contains("huile",     Qt::CaseInsensitive)
                          || question.contains("olive",     Qt::CaseInsensitive)
                          || question.contains("pressage",  Qt::CaseInsensitive)
                          || question.contains("temperatur",Qt::CaseInsensitive)
                          || question.contains(q.value(0).toString()); // ID op

            if (mentionné) {
                section += QString("PRODUCTION: ID=%1 | Date=%2 | "
                                   "AgriCIN=%3 | EmpID=%4 | "
                                   "Olives=%5 kg | Huile=%6 L | "
                                   "Rendement=%7% | Type=%8 | "
                                   "Temp=%9°C | Duree=%10 min\n")
                               .arg(q.value(0).toString())
                               .arg(q.value(1).toDate().toString("dd/MM/yyyy"))
                               .arg(q.value(2).toString())
                               .arg(q.value(3).toString())
                               .arg(q.value(4).toString())
                               .arg(q.value(5).toString())
                               .arg(q.value(6).toString())
                               .arg(q.value(7).toString())
                               .arg(q.value(8).toString())
                               .arg(q.value(9).toString());
                count++;
            }
        }
        if (!section.isEmpty())
            contexte += "=== PRODUCTIONS (20 dernières) ===\n" + section + "\n";
    }

    // ── 5. VENTES ────────────────────────────────────────────────
    {
        QSqlQuery q;
        q.exec("SELECT ID_VENTE, ID_CLIENT, PRIX_UNITAIRE, "
               "DATE_VENTE, QUANTITE_VENDUE, MONTANT_PAYE "
               "FROM SMART.VENTE ORDER BY DATE_VENTE DESC");

        QString section = "";
        int count = 0;
        while (q.next() && count < 20) {
            bool mentionné = question.contains("vente",    Qt::CaseInsensitive)
                          || question.contains("chiffre",  Qt::CaseInsensitive)
                          || question.contains("montant",  Qt::CaseInsensitive)
                          || question.contains("prix",     Qt::CaseInsensitive)
                          || question.contains("revenu",   Qt::CaseInsensitive);

            if (mentionné) {
                section += QString("VENTE: ID=%1 | Client=%2 | "
                                   "Prix=%3 DT | Date=%4 | "
                                   "Qte=%5 | Montant=%6 DT\n")
                               .arg(q.value(0).toString())
                               .arg(q.value(1).toString())
                               .arg(q.value(2).toString())
                               .arg(q.value(3).toDate().toString("dd/MM/yyyy"))
                               .arg(q.value(4).toString())
                               .arg(q.value(5).toString());
                count++;
            }
        }
        if (!section.isEmpty())
            contexte += "=== VENTES (20 dernières) ===\n" + section + "\n";
    }

    // Si rien trouvé → donner un résumé général
    if (contexte.isEmpty()) {
        QSqlQuery q;

        q.exec("SELECT COUNT(*) FROM SMART.EMPLOYE");
        int nbEmp = q.next() ? q.value(0).toInt() : 0;

        q.exec("SELECT COUNT(*) FROM SMART.AGRICULTEUR");
        int nbAgri = q.next() ? q.value(0).toInt() : 0;

        q.exec("SELECT COUNT(*) FROM SMART.CLIENT");
        int nbCli = q.next() ? q.value(0).toInt() : 0;

        q.exec("SELECT COUNT(*) FROM PRODUCTION");
        int nbProd = q.next() ? q.value(0).toInt() : 0;

        q.exec("SELECT COUNT(*), NVL(SUM(MONTANT_PAYE),0) FROM SMART.VENTE");
        int nbVentes = 0; double ca = 0;
        if (q.next()) { nbVentes = q.value(0).toInt(); ca = q.value(1).toDouble(); }

        q.exec("SELECT AVG(RENDEMENT) FROM PRODUCTION");
        double rend = q.next() ? q.value(0).toDouble() : 0;

        contexte = QString(
            "=== RÉSUMÉ GLOBAL Smart Oil Press ===\n"
            "Employés      : %1\n"
            "Agriculteurs  : %2\n"
            "Clients       : %3\n"
            "Productions   : %4 opérations | Rendement moyen : %5%\n"
            "Ventes        : %6 | Chiffre d'affaires : %7 DT\n"
        ).arg(nbEmp).arg(nbAgri).arg(nbCli)
         .arg(nbProd).arg(QString::number(rend,'f',1))
         .arg(nbVentes).arg(QString::number(ca,'f',2));
    }

    return contexte;
}

// ----------------------------------------------------------------
// SLOT PRINCIPAL : ouvrir le chatbot global
// Connectez ce slot à votre bouton chatbot global
// ----------------------------------------------------------------
void MainWindow::on_btn_chatbot_global_clicked()
{
    QDialog *dlg = new QDialog(this);
    dlg->setWindowTitle("🤖 Assistant Smart Oil Press");
    dlg->setFixedSize(700, 560);
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
        "QComboBox { border: 2px solid #D4AF37; border-radius: 6px;"
        "            padding: 4px 8px; font-size: 12px; }"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(dlg);
    mainLayout->setSpacing(8);
    mainLayout->setContentsMargins(12, 12, 12, 12);

    // ── Titre ────────────────────────────────────────────────────
    QLabel *lblTitre = new QLabel("🤖  Assistant Intelligent — Smart Oil Press");
    lblTitre->setAlignment(Qt::AlignCenter);
    lblTitre->setStyleSheet(
        "font-size: 14px; font-weight: bold; color: #556B2F;"
        "padding: 8px; background-color: #e8f0d8; border-radius: 6px;");
    mainLayout->addWidget(lblTitre);

    // ── Filtre module ────────────────────────────────────────────
    QHBoxLayout *filterRow = new QHBoxLayout();
    QLabel *lblModule = new QLabel("Module :");
    lblModule->setStyleSheet("font-size: 12px; font-weight: bold;");
    QComboBox *cbModule = new QComboBox();
    cbModule->addItems({
        "🌐 Tous les modules",
        "👷 Employés",
        "🌿 Agriculteurs",
        "👥 Clients",
        "⚙️  Production",
        "🛒 Ventes"
    });
    cbModule->setFixedWidth(200);
    filterRow->addWidget(lblModule);
    filterRow->addWidget(cbModule);
    filterRow->addStretch();

    // Bouton reset conversation
    QPushButton *btnReset = new QPushButton("🔄 Nouvelle conversation");
    btnReset->setStyleSheet(
        "background-color: #D4AF37; color: white; border-radius: 8px;"
        "padding: 6px 12px; font-size: 12px; font-weight: bold;");
    btnReset->setFixedHeight(32);
    filterRow->addWidget(btnReset);
    mainLayout->addLayout(filterRow);

    // ── Zone de conversation ─────────────────────────────────────
    QTextEdit *chatArea = new QTextEdit();
    chatArea->setReadOnly(true);
    chatArea->setMinimumHeight(360);
    chatArea->append(
        "<b style='color:#556B2F;'>🤖 Assistant :</b> "
        "Bonjour ! Je suis votre assistant pour <b>Smart Oil Press</b>.<br>"
        "Je peux répondre sur tous les modules :<br>"
        "- <i>Quel est le rendement moyen de production ?</i><br>"
        "- <i>Combien de clients gold avons-nous ?</i><br>"
        "- <i>Quelles sont les ventes du mois ?</i><br>"
        "- <i>Quel agriculteur a livré le plus d'olives ?</i><br>"
        "- <i>Quels employés sont en congé ?</i>"
    );
    mainLayout->addWidget(chatArea);

    // ── Zone de saisie ───────────────────────────────────────────
    QHBoxLayout *inputLayout = new QHBoxLayout();
    QLineEdit *inputMsg = new QLineEdit();
    inputMsg->setPlaceholderText("Posez votre question ici...");
    inputMsg->setMinimumHeight(38);

    QPushButton *btnEnvoyer = new QPushButton("📤 Envoyer");
    btnEnvoyer->setMinimumHeight(38);
    btnEnvoyer->setFixedWidth(110);

    inputLayout->addWidget(inputMsg);
    inputLayout->addWidget(btnEnvoyer);
    mainLayout->addLayout(inputLayout);

    // ── Bouton fermer ────────────────────────────────────────────
    QPushButton *btnFermer = new QPushButton("✖  Fermer");
    btnFermer->setStyleSheet(
        "background-color: #888; color: white; border-radius: 8px;"
        "padding: 6px 16px; font-size: 13px;");
    btnFermer->setFixedHeight(34);
    mainLayout->addWidget(btnFermer, 0, Qt::AlignRight);

    // ── Connexions ───────────────────────────────────────────────
    connect(btnFermer, &QPushButton::clicked, dlg, &QDialog::close);

    connect(btnReset, &QPushButton::clicked, [&]() {
        historiqueChat.clear();
        chatArea->clear();
        chatArea->append(
            "<b style='color:#556B2F;'>🤖 Assistant :</b> "
            "Nouvelle conversation démarrée ! Comment puis-je vous aider ?");
    });

    // ── Logique d'envoi ──────────────────────────────────────────
    auto envoyerMessage = [&]() {
        QString question = inputMsg->text().trimmed();
        if (question.isEmpty()) return;

        // Afficher la question
        chatArea->append(
            "<br><b style='color:#1565C0;'>🧑 Vous :</b> " + question);
        chatArea->append(
            "<b style='color:#556B2F;'>🤖 Assistant :</b> "
            "<i style='color:#999;'>Analyse en cours...</i>");
        inputMsg->clear();
        QApplication::processEvents();

        // Récupérer le contexte selon le module sélectionné
        QString moduleChoisi = cbModule->currentText();
        QString questionAvecModule = question;

        // Injecter le module dans la question pour orienter getContexteGlobal
        if (moduleChoisi.contains("Employés"))
            questionAvecModule += " employ salaire prime statut";
        else if (moduleChoisi.contains("Agriculteurs"))
            questionAvecModule += " agri olive region livraison";
        else if (moduleChoisi.contains("Clients"))
            questionAvecModule += " client achat fidelite point";
        else if (moduleChoisi.contains("Production"))
            questionAvecModule += " production rendement huile pressage";
        else if (moduleChoisi.contains("Ventes"))
            questionAvecModule += " vente montant chiffre prix";

        QString contexte = getContexteGlobal(questionAvecModule);

        // Construire le prompt
        QString prompt = QString(
            "Tu es un assistant expert pour une huilerie tunisienne appelée Smart Oil Press. "
            "Réponds UNIQUEMENT en français. Sois précis, professionnel et concis.\n\n"
            "=== DONNÉES DE LA BASE DE DONNÉES ===\n"
            "%1\n"
            "=== FIN DES DONNÉES ===\n\n"
            "Question : %2\n\n"
            "Instructions :\n"
            "- Base-toi UNIQUEMENT sur les données fournies ci-dessus\n"
            "- Si la donnée n'existe pas, dis-le clairement\n"
            "- Pour les calculs (rendement moyen, CA total...), calcule toi-même\n"
            "- Sois direct et structuré dans ta réponse"
        ).arg(contexte, question);

        // Appeler Ollama
        QString reponse = interrogerOllama(prompt);

        // Supprimer "Analyse en cours..."
        QTextCursor cursor = chatArea->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
        cursor.deletePreviousChar();

        // Afficher la réponse
        chatArea->append(
            "<b style='color:#556B2F;'>🤖 Assistant :</b> " + reponse + "<br>");

        // Scroll vers le bas
        chatArea->verticalScrollBar()->setValue(
            chatArea->verticalScrollBar()->maximum());
    };

    connect(btnEnvoyer, &QPushButton::clicked, envoyerMessage);
    connect(inputMsg, &QLineEdit::returnPressed, envoyerMessage);

    dlg->exec();
    delete dlg;
}

void MainWindow::on_btn_logout_2_clicked()
{

}

// ================================================================
// ANALYSE EMPLOYÉS — Popup QDialog
// Performance & Absences & Ancienneté
// ================================================================
// Colle cette fonction dans mainwindow.cpp
// Et déclare-la dans mainwindow.h :
//   void on_btn_analyse_employes_clicked();
// ================================================================

void MainWindow::on_btn_analyse_employes_clicked()
{
    // ── Connexion DB ──────────────────────────────────────────
    Connection conn;
    if (!conn.createConnection()) {
        QMessageBox::critical(this, "Erreur", "Impossible de se connecter à la base de données !");
        return;
    }

    QSqlQuery q;

    // ── 1. Stats générales statut ─────────────────────────────
    int nbTotal = 0, nbActifs = 0, nbConge = 0, nbSusp = 0;
    if (q.exec("SELECT COUNT(*) FROM EMPLOYE"))
        if (q.next()) nbTotal = q.value(0).toInt();
    if (q.exec("SELECT COUNT(*) FROM EMPLOYE WHERE STATUT='Actif'"))
        if (q.next()) nbActifs = q.value(0).toInt();
    if (q.exec("SELECT COUNT(*) FROM EMPLOYE WHERE STATUT='En conge'"))
        if (q.next()) nbConge = q.value(0).toInt();
    if (q.exec("SELECT COUNT(*) FROM EMPLOYE WHERE STATUT='Suspendu'"))
        if (q.next()) nbSusp = q.value(0).toInt();

    // ── 2. Ancienneté moyenne ─────────────────────────────────
    double anciennetemoyenne = 0.0;
    if (q.exec("SELECT AVG(MONTHS_BETWEEN(SYSDATE, DATE_EMBAUCHE)/12) FROM EMPLOYE WHERE DATE_EMBAUCHE IS NOT NULL"))
        if (q.next()) anciennetemoyenne = q.value(0).toDouble();

    // ── 3. Employé le plus ancien ─────────────────────────────
    QString plusAncienNom = "-", plusAncienDate = "-";
    double plusAncienAns = 0.0;
    if (q.exec("SELECT NOM, PRENOM, DATE_EMBAUCHE, "
               "MONTHS_BETWEEN(SYSDATE, DATE_EMBAUCHE)/12 AS ANS "
               "FROM EMPLOYE WHERE DATE_EMBAUCHE IS NOT NULL "
               "ORDER BY DATE_EMBAUCHE ASC FETCH FIRST 1 ROWS ONLY")) {
        if (q.next()) {
            plusAncienNom  = q.value(0).toString() + " " + q.value(1).toString();
            QDate d = q.value(2).toDate();
            plusAncienDate = d.isValid() ? d.toString("dd/MM/yyyy") : q.value(2).toString();
            plusAncienAns  = q.value(3).toDouble();
        }
    }

    // ── 4. Employé le plus récent ─────────────────────────────
    QString plusRecentNom = "-", plusRecentDate = "-";
    if (q.exec("SELECT NOM, PRENOM, DATE_EMBAUCHE "
               "FROM EMPLOYE WHERE DATE_EMBAUCHE IS NOT NULL "
               "ORDER BY DATE_EMBAUCHE DESC FETCH FIRST 1 ROWS ONLY")) {
        if (q.next()) {
            plusRecentNom  = q.value(0).toString() + " " + q.value(1).toString();
            QDate d = q.value(2).toDate();
            plusRecentDate = d.isValid() ? d.toString("dd/MM/yyyy") : q.value(2).toString();
        }
    }

    // ── 5. TOP 3 employés en congé le plus souvent ────────────
    //    (on liste les employés avec statut En conge, triés par nom)
    QStringList enCongeList;
    if (q.exec("SELECT NOM, PRENOM, POSTE, DATE_EMBAUCHE "
               "FROM EMPLOYE WHERE STATUT='En conge' "
               "ORDER BY NOM ASC")) {
        while (q.next()) {
            QDate d = q.value(3).toDate();
            double ans = d.isValid()
                ? d.daysTo(QDate::currentDate()) / 365.25
                : 0.0;
            enCongeList << QString("• %1 %2  —  %3  (%4 ans)")
                               .arg(q.value(0).toString())
                               .arg(q.value(1).toString())
                               .arg(q.value(2).toString())
                               .arg(ans, 0, 'f', 1);
        }
    }

    // ── 6. Répartition par département ───────────────────────
    QList<QPair<QString,int>> depts;
    if (q.exec("SELECT DEPARTEMENT, COUNT(*) FROM EMPLOYE "
               "WHERE DEPARTEMENT IS NOT NULL "
               "GROUP BY DEPARTEMENT ORDER BY COUNT(*) DESC")) {
        while (q.next())
            depts << qMakePair(q.value(0).toString(), q.value(1).toInt());
    }

    // ── 7. Salaire moyen ──────────────────────────────────────
    double salaireMoyen = 0.0;
    if (q.exec("SELECT AVG(SALAIRE) FROM EMPLOYE WHERE SALAIRE IS NOT NULL"))
        if (q.next()) salaireMoyen = q.value(0).toDouble();

    // ════════════════════════════════════════════════════════════
    // CONSTRUCTION DU DIALOG
    // ════════════════════════════════════════════════════════════
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("📊 Analyse des Employés");
    dialog->setMinimumSize(620, 580);
    dialog->setStyleSheet("QDialog { background: #F4F6F0; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    // ── Header ────────────────────────────────────────────────
    QFrame *header = new QFrame(dialog);
    header->setFixedHeight(54);
    header->setStyleSheet("QFrame { background: #556B2F; border-radius: 10px; }");
    QHBoxLayout *hL = new QHBoxLayout(header);
    hL->setContentsMargins(16, 0, 16, 0);
    QLabel *headerLbl = new QLabel("📊  Performance & Absences — Employés");
    headerLbl->setStyleSheet("color: white; font-size: 14px; font-weight: bold; background: transparent;");
    hL->addWidget(headerLbl);
    hL->addStretch();
    QLabel *dateLbl = new QLabel(QDate::currentDate().toString("dd/MM/yyyy"));
    dateLbl->setStyleSheet("color: #D4AF37; font-size: 11px; background: transparent;");
    hL->addWidget(dateLbl);
    mainLayout->addWidget(header);

    // ── ScrollArea ────────────────────────────────────────────
    QScrollArea *scroll = new QScrollArea(dialog);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setStyleSheet("QScrollArea { background: #F4F6F0; border: none; }"
                          "QScrollBar:vertical { width: 7px; background: #ddd; border-radius: 3px; }"
                          "QScrollBar::handle:vertical { background: #556B2F; border-radius: 3px; }");

    QWidget *scrollContent = new QWidget();
    scrollContent->setStyleSheet("background: #F4F6F0;");
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);
    scrollLayout->setSpacing(10);
    scrollLayout->setContentsMargins(0, 0, 6, 0);

    // Helper : crée une card blanche avec titre
    auto makeCard = [&](const QString &titre) -> QPair<QFrame*, QVBoxLayout*> {
        QFrame *card = new QFrame(scrollContent);
        card->setStyleSheet("QFrame { background: white; border-radius: 10px; border: 1px solid #e0e0e0; }");
        QVBoxLayout *cl = new QVBoxLayout(card);
        cl->setContentsMargins(14, 10, 14, 12);
        cl->setSpacing(6);
        if (!titre.isEmpty()) {
            QLabel *t = new QLabel(titre);
            t->setStyleSheet("color: #556B2F; font-size: 12px; font-weight: bold; background: transparent; border: none;");
            cl->addWidget(t);
            QFrame *sep = new QFrame();
            sep->setFrameShape(QFrame::HLine);
            sep->setStyleSheet("background: #e8e8e8; border: none; max-height: 1px;");
            cl->addWidget(sep);
        }
        return {card, cl};
    };

    // Helper : ligne stat
    auto addStat = [](QVBoxLayout *cl, const QString &label,
                      const QString &value, const QString &color = "#333") {
        QHBoxLayout *row = new QHBoxLayout();
        QLabel *lbl = new QLabel(label);
        lbl->setStyleSheet("color: #555; font-size: 11px; background: transparent; border: none;");
        QLabel *val = new QLabel(value);
        val->setStyleSheet(QString("color: %1; font-size: 12px; font-weight: bold;"
                                   " background: transparent; border: none;").arg(color));
        val->setAlignment(Qt::AlignRight);
        row->addWidget(lbl);
        row->addStretch();
        row->addWidget(val);
        cl->addLayout(row);
    };

    // ── CARD 1 : Vue d'ensemble ───────────────────────────────
    {
        auto [card, cl] = makeCard("👥  Vue d'ensemble");

        // Mini badges colorés
        QHBoxLayout *badgeRow = new QHBoxLayout();
        auto makeBadge = [](const QString &txt, const QString &bg) {
            QLabel *b = new QLabel(txt);
            b->setAlignment(Qt::AlignCenter);
            b->setFixedHeight(36);
            b->setStyleSheet(QString("background: %1; color: white; border-radius: 8px;"
                                     " font-size: 11px; font-weight: bold; border: none;").arg(bg));
            return b;
        };
        badgeRow->addWidget(makeBadge(QString("Total\n%1").arg(nbTotal),   "#556B2F"));
        badgeRow->addWidget(makeBadge(QString("Actifs\n%1").arg(nbActifs), "#378ADD"));
        badgeRow->addWidget(makeBadge(QString("Congé\n%1").arg(nbConge),   "#AAAAAA"));
        badgeRow->addWidget(makeBadge(QString("Suspendus\n%1").arg(nbSusp),"#E24B4A"));
        cl->addLayout(badgeRow);

        addStat(cl, "Salaire moyen",
                QString("%1 DT").arg(salaireMoyen, 0, 'f', 2), "#BA7517");

        scrollLayout->addWidget(card);
    }

    // ── CARD 2 : Ancienneté ───────────────────────────────────
    {
        auto [card, cl] = makeCard("📅  Ancienneté");

        addStat(cl, "Ancienneté moyenne",
                QString("%1 ans").arg(anciennetemoyenne, 0, 'f', 1), "#556B2F");
        addStat(cl, "Employé le plus ancien",
                QString("%1  (%2 ans)").arg(plusAncienNom).arg(plusAncienAns, 0, 'f', 1),
                "#378ADD");
        addStat(cl, "Date d'embauche", plusAncienDate, "#555");
        addStat(cl, "Recrutement le plus récent", plusRecentNom, "#1D9E75");
        addStat(cl, "Date d'embauche", plusRecentDate, "#555");

        scrollLayout->addWidget(card);
    }

    // ── CARD 3 : Employés en congé ────────────────────────────
    {
        auto [card, cl] = makeCard(QString("🏖️  Employés actuellement en congé  (%1)").arg(nbConge));

        if (enCongeList.isEmpty()) {
            QLabel *none = new QLabel("Aucun employé en congé actuellement.");
            none->setStyleSheet("color: #888; font-size: 11px; background: transparent; border: none;");
            cl->addWidget(none);
        } else {
            for (const QString &ligne : enCongeList) {
                QLabel *l = new QLabel(ligne);
                l->setStyleSheet("color: #444; font-size: 11px; background: transparent; border: none;");
                cl->addWidget(l);
            }
        }

        scrollLayout->addWidget(card);
    }

    // ── CARD 4 : Répartition par département ──────────────────
    {
        auto [card, cl] = makeCard("🏢  Répartition par département");

        if (depts.isEmpty()) {
            QLabel *none = new QLabel("Aucune donnée de département disponible.");
            none->setStyleSheet("color: #888; font-size: 11px; background: transparent; border: none;");
            cl->addWidget(none);
        } else {
            QList<QString> colors = {"#378ADD","#556B2F","#D4537E","#BA7517","#1D9E75","#9B59B6"};
            int ci = 0;
            for (auto &[dept, cnt] : depts) {
                QHBoxLayout *row = new QHBoxLayout();

                QLabel *nomDept = new QLabel(dept);
                nomDept->setStyleSheet("color: #444; font-size: 11px; background: transparent; border: none;");

                // Barre de progression
                QProgressBar *bar = new QProgressBar();
                bar->setRange(0, qMax(nbTotal, 1));
                bar->setValue(cnt);
                bar->setFixedHeight(14);
                bar->setTextVisible(false);
                QString c = colors[ci % colors.size()];
                bar->setStyleSheet(QString(
                    "QProgressBar { background: #eee; border-radius: 6px; border: none; }"
                    "QProgressBar::chunk { background: %1; border-radius: 6px; }").arg(c));

                QLabel *valLbl = new QLabel(QString("%1").arg(cnt));
                valLbl->setFixedWidth(24);
                valLbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                valLbl->setStyleSheet(QString("color: %1; font-weight: bold; font-size: 11px;"
                                              " background: transparent; border: none;").arg(c));

                row->addWidget(nomDept, 2);
                row->addWidget(bar, 5);
                row->addWidget(valLbl, 1);
                cl->addLayout(row);
                ci++;
            }
        }

        scrollLayout->addWidget(card);
    }

    // ── CARD 5 : Graphique donut Qt Charts ───────────────────
    {
        auto [card, cl] = makeCard("📈  Graphique statuts");

        QPieSeries *series = new QPieSeries();
        if (nbActifs > 0) series->append("Actifs",    nbActifs);
        if (nbConge  > 0) series->append("En congé",  nbConge);
        if (nbSusp   > 0) series->append("Suspendus", nbSusp);
        if (series->count() == 0) series->append("Aucun", 1);
        series->setHoleSize(0.45);

        QList<QColor> cols = {QColor("#378ADD"), QColor("#AAAAAA"), QColor("#E24B4A")};
        for (int i = 0; i < series->count(); ++i) {
            series->slices().at(i)->setBrush(cols[i % cols.size()]);
            series->slices().at(i)->setBorderColor(Qt::white);
            series->slices().at(i)->setBorderWidth(2);
        }

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setAnimationOptions(QChart::SeriesAnimations);
        chart->legend()->setAlignment(Qt::AlignBottom);
        chart->legend()->setFont(QFont("Arial", 8));
        chart->setMargins(QMargins(0, 0, 0, 0));
        chart->setBackgroundVisible(false);

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setFixedHeight(200);
        chartView->setBackgroundBrush(Qt::transparent);
        cl->addWidget(chartView);

        scrollLayout->addWidget(card);
    }

    scrollLayout->addStretch();
    scroll->setWidget(scrollContent);
    mainLayout->addWidget(scroll);

    // ── Bouton Fermer ─────────────────────────────────────────
    QPushButton *btnClose = new QPushButton("✖  Fermer");
    btnClose->setFixedHeight(38);
    btnClose->setCursor(Qt::PointingHandCursor);
    btnClose->setStyleSheet(
        "QPushButton { background: #556B2F; color: white; border-radius: 8px;"
        " font-size: 12px; font-weight: bold; border: none; }"
        "QPushButton:hover { background: #6B8A3A; }");
    connect(btnClose, &QPushButton::clicked, dialog, &QDialog::accept);
    mainLayout->addWidget(btnClose);

    dialog->exec();
    delete dialog;
}

// ================================================================
// GRAPHIQUES AGRICULTEURS — Popup QDialog
// 3 graphiques : Bar + Pie + Line
// ================================================================
// 1. Déclare dans mainwindow.h :
//      void on_btn_graphiques_agri_clicked();
//
// 2. Dans le .ui, objectName du bouton : btn_graphiques_agri
// ================================================================

void MainWindow::on_btn_graphiques_agri_clicked()
{
    Connection conn;
    if (!conn.createConnection()) {
        QMessageBox::critical(this, "Erreur", "Impossible de se connecter à la base de données !");
        return;
    }

    // ════════════════════════════════════════════════════════════
    // DONNÉES — Volume par Région
    // ════════════════════════════════════════════════════════════
    QStringList regions;
    QList<double> volumes;
    {
        QSqlQuery q;
        q.exec("SELECT REGION, SUM(VOLUME_LIVRAISON) "
               "FROM AGRICULTEUR "
               "WHERE REGION IS NOT NULL "
               "GROUP BY REGION "
               "ORDER BY SUM(VOLUME_LIVRAISON) DESC");
        while (q.next()) {
            regions << q.value(0).toString();
            volumes << q.value(1).toDouble();
        }
    }

    // ════════════════════════════════════════════════════════════
    // DONNÉES — Répartition par Type d'Olive
    // ════════════════════════════════════════════════════════════
    QMap<QString, int> typeOlives;
    {
        QSqlQuery q;
        q.exec("SELECT TYPE_OLIVE, COUNT(*) "
               "FROM AGRICULTEUR "
               "WHERE TYPE_OLIVE IS NOT NULL "
               "GROUP BY TYPE_OLIVE "
               "ORDER BY COUNT(*) DESC");
        while (q.next())
            typeOlives[q.value(0).toString()] = q.value(1).toInt();
    }

    // ════════════════════════════════════════════════════════════
    // DONNÉES — Évolution livraisons par mois
    // ════════════════════════════════════════════════════════════
    QList<QPair<QString, double>> livraisonsMois;
    {
        QSqlQuery q;
        q.exec("SELECT TO_CHAR(DATE_LIVRAISON, 'MM/YYYY') AS MOIS, "
               "SUM(VOLUME_LIVRAISON) "
               "FROM AGRICULTEUR "
               "WHERE DATE_LIVRAISON IS NOT NULL "
               "GROUP BY TO_CHAR(DATE_LIVRAISON, 'MM/YYYY') "
               "ORDER BY MIN(DATE_LIVRAISON)");
        while (q.next())
            livraisonsMois << qMakePair(q.value(0).toString(), q.value(1).toDouble());
    }

    // ════════════════════════════════════════════════════════════
    // CONSTRUCTION DU DIALOG
    // ════════════════════════════════════════════════════════════
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("📊 Graphiques Agriculteurs");
    dialog->setMinimumSize(850, 620);
    dialog->setStyleSheet(
        "QDialog { background: #F4F6F0; }"
        "QTabWidget::pane { border: 2px solid #D4AF37; border-radius: 8px; background: white; }"
        "QTabBar::tab { background: #556B2F; color: white; padding: 8px 18px;"
        "               border-radius: 4px; margin: 2px; font-weight: bold; font-size: 12px; }"
        "QTabBar::tab:selected { background: #D4AF37; color: #2C3E1A; }"
        "QTabBar::tab:hover { background: #6B8A3A; }"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
    mainLayout->setContentsMargins(14, 14, 14, 14);
    mainLayout->setSpacing(10);

    // ── Header ────────────────────────────────────────────────
    QFrame *header = new QFrame(dialog);
    header->setFixedHeight(54);
    header->setStyleSheet("QFrame { background: #556B2F; border-radius: 10px; }");
    QHBoxLayout *hL = new QHBoxLayout(header);
    hL->setContentsMargins(16, 0, 16, 0);

    QLabel *headerLbl = new QLabel("📊  Statistiques & Graphiques — Agriculteurs");
    headerLbl->setStyleSheet("color: white; font-size: 14px; font-weight: bold; background: transparent;");
    hL->addWidget(headerLbl);
    hL->addStretch();

    QLabel *dateLbl = new QLabel(QDate::currentDate().toString("dd/MM/yyyy"));
    dateLbl->setStyleSheet("color: #D4AF37; font-size: 11px; background: transparent;");
    hL->addWidget(dateLbl);
    mainLayout->addWidget(header);

    // ── KPI rapides ───────────────────────────────────────────
    {
        QSqlQuery q;
        int nbTotal = 0; double volTotal = 0.0;
        if (q.exec("SELECT COUNT(*), NVL(SUM(VOLUME_LIVRAISON),0) FROM AGRICULTEUR"))
            if (q.next()) { nbTotal = q.value(0).toInt(); volTotal = q.value(1).toDouble(); }

        QHBoxLayout *kpiRow = new QHBoxLayout();
        kpiRow->setSpacing(10);

        auto makeKPI = [](const QString &label, const QString &val, const QString &color) {
            QFrame *f = new QFrame();
            f->setFixedHeight(56);
            f->setStyleSheet(QString("QFrame { background: %1; border-radius: 8px; border: none; }").arg(color));
            QVBoxLayout *l = new QVBoxLayout(f);
            l->setContentsMargins(12, 4, 12, 4);
            l->setSpacing(0);
            QLabel *lv = new QLabel(val);
            lv->setStyleSheet("color: white; font-size: 18px; font-weight: bold; background: transparent;");
            lv->setAlignment(Qt::AlignCenter);
            QLabel *ll = new QLabel(label);
            ll->setStyleSheet("color: rgba(255,255,255,0.85); font-size: 10px; background: transparent;");
            ll->setAlignment(Qt::AlignCenter);
            l->addWidget(lv);
            l->addWidget(ll);
            return f;
        };

        kpiRow->addWidget(makeKPI("Total Agriculteurs", QString::number(nbTotal), "#556B2F"));
        kpiRow->addWidget(makeKPI("Volume Total (L)", QString::number(volTotal, 'f', 1), "#378ADD"));
        kpiRow->addWidget(makeKPI("Nb Régions", QString::number(regions.size()), "#BA7517"));
        kpiRow->addWidget(makeKPI("Types d'olive", QString::number(typeOlives.size()), "#1D9E75"));
        mainLayout->addLayout(kpiRow);
    }

    // ── Tab Widget ────────────────────────────────────────────
    QTabWidget *tabs = new QTabWidget(dialog);
    tabs->setDocumentMode(true);

    // ════════════════════════════════════════════════════════════
    // ONGLET 1 — Bar Chart : Volume par Région
    // ════════════════════════════════════════════════════════════
    {
        QWidget *tab = new QWidget();
        QVBoxLayout *tl = new QVBoxLayout(tab);
        tl->setContentsMargins(8, 8, 8, 8);

        if (regions.isEmpty()) {
            QLabel *empty = new QLabel("Aucune donnée de région disponible.");
            empty->setAlignment(Qt::AlignCenter);
            empty->setStyleSheet("color: #888; font-size: 13px;");
            tl->addWidget(empty);
        } else {
            QBarSet *set = new QBarSet("Volume (L)");
            set->setColor(QColor("#556B2F"));
            set->setBorderColor(QColor("#3a4d1f"));
            for (double v : volumes) *set << v;

            QBarSeries *series = new QBarSeries();
            series->append(set);

            QChart *chart = new QChart();
            chart->addSeries(series);
            chart->setTitle("Volume livré par Région (Litres)");
            chart->setTitleFont(QFont("Arial", 11, QFont::Bold));
            chart->setAnimationOptions(QChart::SeriesAnimations);
            chart->legend()->setVisible(false);
            chart->setBackgroundVisible(false);
            chart->setMargins(QMargins(6, 6, 6, 6));

            QBarCategoryAxis *axX = new QBarCategoryAxis();
            axX->append(regions);
            axX->setLabelsFont(QFont("Arial", 9));
            chart->addAxis(axX, Qt::AlignBottom);
            series->attachAxis(axX);

            double maxVol = volumes.isEmpty() ? 10 : *std::max_element(volumes.begin(), volumes.end());
            QValueAxis *axY = new QValueAxis();
            axY->setRange(0, maxVol * 1.15);
            axY->setLabelFormat("%.0f L");
            axY->setLabelsFont(QFont("Arial", 8));
            chart->addAxis(axY, Qt::AlignLeft);
            series->attachAxis(axY);

            QChartView *view = new QChartView(chart);
            view->setRenderHint(QPainter::Antialiasing);
            view->setBackgroundBrush(Qt::transparent);
            tl->addWidget(view);
        }
        tabs->addTab(tab, "🗺️  Volume / Région");
    }

    // ════════════════════════════════════════════════════════════
    // ONGLET 2 — Pie Chart : Répartition par Type d'Olive
    // ════════════════════════════════════════════════════════════
    {
        QWidget *tab = new QWidget();
        QVBoxLayout *tl = new QVBoxLayout(tab);
        tl->setContentsMargins(8, 8, 8, 8);

        if (typeOlives.isEmpty()) {
            QLabel *empty = new QLabel("Aucune donnée de type d'olive disponible.");
            empty->setAlignment(Qt::AlignCenter);
            empty->setStyleSheet("color: #888; font-size: 13px;");
            tl->addWidget(empty);
        } else {
            QPieSeries *series = new QPieSeries();
            series->setHoleSize(0.40);

            QList<QColor> cols = {
                QColor("#556B2F"), QColor("#D4AF37"), QColor("#378ADD"),
                QColor("#BA7517"), QColor("#1D9E75"), QColor("#D4537E"),
                QColor("#9B59B6"), QColor("#E67E22")
            };

            int idx = 0;
            for (auto it = typeOlives.begin(); it != typeOlives.end(); ++it, ++idx) {
                QPieSlice *slice = series->append(
                    QString("%1 (%2)").arg(it.key()).arg(it.value()),
                    it.value()
                );
                slice->setBrush(cols[idx % cols.size()]);
                slice->setBorderColor(Qt::white);
                slice->setBorderWidth(2);
                slice->setLabelVisible(true);
                slice->setLabelFont(QFont("Arial", 8));
            }

            QChart *chart = new QChart();
            chart->addSeries(series);
            chart->setTitle("Répartition par Type d'Olive");
            chart->setTitleFont(QFont("Arial", 11, QFont::Bold));
            chart->setAnimationOptions(QChart::SeriesAnimations);
            chart->legend()->setAlignment(Qt::AlignRight);
            chart->legend()->setFont(QFont("Arial", 9));
            chart->setBackgroundVisible(false);
            chart->setMargins(QMargins(6, 6, 6, 6));

            QChartView *view = new QChartView(chart);
            view->setRenderHint(QPainter::Antialiasing);
            view->setBackgroundBrush(Qt::transparent);
            tl->addWidget(view);
        }
        tabs->addTab(tab, "🫒  Type d'Olive");
    }

    // ════════════════════════════════════════════════════════════
    // ONGLET 3 — Line Chart : Évolution livraisons par mois
    // ════════════════════════════════════════════════════════════
    {
        QWidget *tab = new QWidget();
        QVBoxLayout *tl = new QVBoxLayout(tab);
        tl->setContentsMargins(8, 8, 8, 8);

        if (livraisonsMois.isEmpty()) {
            QLabel *empty = new QLabel("Aucune donnée de livraison disponible.");
            empty->setAlignment(Qt::AlignCenter);
            empty->setStyleSheet("color: #888; font-size: 13px;");
            tl->addWidget(empty);
        } else {
            QLineSeries *series = new QLineSeries();
            series->setName("Volume livré (L)");

            QPen pen(QColor("#556B2F"));
            pen.setWidth(3);
            series->setPen(pen);

            // Points visibles
            series->setPointsVisible(true);

            QStringList moisLabels;
            double maxVal = 0;
            for (int i = 0; i < livraisonsMois.size(); ++i) {
                series->append(i, livraisonsMois[i].second);
                moisLabels << livraisonsMois[i].first;
                if (livraisonsMois[i].second > maxVal)
                    maxVal = livraisonsMois[i].second;
            }

            // Aire sous la courbe
            QAreaSeries *areaSeries = new QAreaSeries(series);
            areaSeries->setName("Volume livré (L)");
            QPen areaPen(QColor("#556B2F"));
            areaPen.setWidth(2);
            areaSeries->setPen(areaPen);
            QLinearGradient gradient(QPointF(0,0), QPointF(0,1));
            gradient.setColorAt(0.0, QColor("#556B2F"));
            gradient.setColorAt(1.0, QColor("#556B2F22"));
            gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
            areaSeries->setBrush(gradient);

            QChart *chart = new QChart();
            chart->addSeries(areaSeries);
            chart->setTitle("Évolution des livraisons par mois (L)");
            chart->setTitleFont(QFont("Arial", 11, QFont::Bold));
            chart->setAnimationOptions(QChart::SeriesAnimations);
            chart->legend()->setVisible(false);
            chart->setBackgroundVisible(false);
            chart->setMargins(QMargins(6, 6, 6, 6));

            QBarCategoryAxis *axX = new QBarCategoryAxis();
            axX->append(moisLabels);
            axX->setLabelsFont(QFont("Arial", 8));
            axX->setLabelsAngle(-30);
            chart->addAxis(axX, Qt::AlignBottom);
            areaSeries->attachAxis(axX);

            QValueAxis *axY = new QValueAxis();
            axY->setRange(0, maxVal * 1.2);
            axY->setLabelFormat("%.0f L");
            axY->setLabelsFont(QFont("Arial", 8));
            chart->addAxis(axY, Qt::AlignLeft);
            areaSeries->attachAxis(axY);

            QChartView *view = new QChartView(chart);
            view->setRenderHint(QPainter::Antialiasing);
            view->setBackgroundBrush(Qt::transparent);
            tl->addWidget(view);
        }
        tabs->addTab(tab, "📈  Évolution Livraisons");
    }

    mainLayout->addWidget(tabs);

    // ── Bouton Fermer ─────────────────────────────────────────
    QPushButton *btnClose = new QPushButton("✖  Fermer");
    btnClose->setFixedHeight(38);
    btnClose->setCursor(Qt::PointingHandCursor);
    btnClose->setStyleSheet(
        "QPushButton { background: #556B2F; color: white; border-radius: 8px;"
        " font-size: 12px; font-weight: bold; border: none; }"
        "QPushButton:hover { background: #6B8A3A; }");
    connect(btnClose, &QPushButton::clicked, dialog, &QDialog::accept);
    mainLayout->addWidget(btnClose);

    dialog->exec();
    delete dialog;
}
