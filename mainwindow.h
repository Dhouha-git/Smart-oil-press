#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "employe.h"
#include "agriculteur.h"
#include "client.h"
#include "production.h"
#include "vente.h"
#include <QString>
#include <QMainWindow>
#include "arduino.h"

#include <QTcpSocket>
#include <QRandomGenerator>
#include <QSslSocket>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QDateTime>
#include <QDir>
#include <QTextEdit>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>
#include <QNetworkAccessManager>
#include <QNetworkReply>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    int id_selected;
     int id_vente_selected = 0; // ✅ vente
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_btn_login_3_clicked();

    void on_btn_home_2_clicked();

    void on_btn_employes_2_clicked();

    void on_btn_clients_2_clicked();

    void on_btn_production_2_clicked();

    void on_btn_ventes_2_clicked();

    void on_btn_ajouter_clicked();

    void on_btn_modifier_clicked();

    void on_analyse_clicked();

    void on_pdf_clicked();

    void on_btn_annuler_3_clicked();

    void on_btn_retour_3_clicked();

    void on_btn_agriculteurs_2_clicked();

    void on_btn_agriculteurs_13_clicked();

    void on_btn_clients_13_clicked();

    void on_btn_employes_13_clicked();

    void on_btn_home_13_clicked();

    void on_btn_production_13_clicked();

    void on_btn_ventes_13_clicked();

    void on_btn_agriculteurs_5_clicked();

    void on_btn_clients_5_clicked();

    void on_btn_employes_5_clicked();

    void on_btn_home_5_clicked();

    void on_btn_production_5_clicked();

    void on_btn_ventes_5_clicked();

    void on_btn_agriculteurs_8_clicked();

    void on_btn_clients_8_clicked();

    void on_btn_employes_8_clicked();

    void on_btn_home_8_clicked();

    void on_btn_production_8_clicked();

    void on_btn_ventes_8_clicked();

    void on_btn_agriculteurs_4_clicked();

    void on_btn_clients_4_clicked();

    void on_btn_employes_4_clicked();

    void on_btn_home_4_clicked();

    void on_btn_production_4_clicked();

    void on_btn_ventes_4_clicked();

    void on_btn_agriculteurs_10_clicked();

    void on_btn_clients_10_clicked();

    void on_btn_employes_10_clicked();

    void on_btn_home_10_clicked();

    void on_btn_production_10_clicked();

    void on_btn_ventes_10_clicked();

    void on_btn_agriculteurs_7_clicked();

    void on_btn_clients_7_clicked();

    void on_btn_employes_7_clicked();

    void on_btn_home_7_clicked();

    void on_btn_production_7_clicked();

    void on_btn_ventes_7_clicked();

    void on_btn_agriculteurs_3_clicked();

    void on_btn_clients_3_clicked();

    void on_btn_employes_3_clicked();

    void on_btn_home_3_clicked();

    void on_btn_production_3_clicked();

    void on_btn_ventes_3_clicked();

    void on_btn_agriculteurs_9_clicked();

    void on_btn_clients_9_clicked();

    void on_btn_employes_9_clicked();

    void on_btn_home_9_clicked();

    void on_btn_production_9_clicked();

    void on_btn_ventes_9_clicked();

    void on_btn_agriculteurs_6_clicked();

    void on_btn_clients_6_clicked();

    void on_btn_employes_6_clicked();

    void on_btn_home_6_clicked();

    void on_btn_production_6_clicked();

    void on_btn_ventes_6_clicked();

    void on_btn_agriculteurs_11_clicked();

    void on_btn_clients_11_clicked();

    void on_btn_employes_11_clicked();

    void on_btn_home_11_clicked();

    void on_btn_production_11_clicked();

    void on_btn_ventes_11_clicked();

    void on_btn_agriculteurs_12_clicked();

    void on_btn_clients_12_clicked();

    void on_btn_employes_12_clicked();

    void on_btn_home_12_clicked();

    void on_btn_production_12_clicked();

    void on_btn_ventes_12_clicked();

    void on_connecter_clicked();

    void on_btn_mdp_oublie_clicked();
    void on_historique_clicked();

    void on_btn_retour_5_clicked();

    void on_btn_retour_6_clicked();

    void on_btn_retour_7_clicked();

    void on_btn_retour_8_clicked();

    void on_btn_retour_9_clicked();

    void on_btn_retour_10_clicked();

    void on_btn_retour_4_clicked();
    void on_modifierVente_clicked();
    void on_annulerVente_clicked();
    void on_btn_annuler_4_clicked();
    void on_btn_valider_3_clicked();
    void on_btn_valider_4_clicked();
    void on_btn_supprimer_clicked();
    void on_pushButton_10_clicked();
    void on_lineEdit_recherche_textChanged(const QString &);
    void on_comboBox_filtre_currentTextChanged(const QString &);
    void on_comboBox_tri_currentTextChanged(const QString &);
    void on_btn_pdf_employes_clicked();
    void on_ajouterVente();
    void on_afficherVente();
    void on_listerVentes();
    void on_resetFormulaire();
    void on_tableView_vente_clicked(const QModelIndex &index);

    void on_exporter_excel_employes_clicked();
    void on_btn_analyse_employes_clicked();
    void on_btn_graphiques_agri_clicked();
    void on_btn_chatbot_clicked();

    void on_btn_face_login_clicked();
    void on_btn_prendre_photo_clicked();

    void on_ajouter_3_clicked();
    void on_supprimer_3_clicked();   // ← adaptez au vrai nom du bouton
    void on_modifier_3_clicked();    // ← adaptez au vrai nom du bouton
    void on_recherche1_2_clicked();       // ← adaptez au vrai nom du bouton
    void on_tri_2_clicked();

    void on_tableView_client_clicked(const QModelIndex &index);

    void on_btn_chatbot_global_clicked();



    // ── Agriculteur ──────────────────────────────────────
    void on_btn_ajouter_2_clicked();
    void on_btn_modifier_agri_clicked();
    void on_btn_supprimer_agri_clicked();
    void on_btn_recherche_agri_clicked();
    void on_btn_tri_agri_clicked();
    void on_TABLEAG_clicked(const QModelIndex &index);
    void on_TABLEAG_doubleClicked(const QModelIndex &index);
    void on_metier_clicked();
    void on_pushButton_20_clicked();
    void on_pushButton_21_clicked();

    void on_btn_stats_agri_clicked();
    void on_btn_alertes_agri_clicked();
    void on_btn_pdf_agri_clicked();
    void on_comboBox_region_agri_currentTextChanged(const QString &region);
    void chargerRegionsAgri();


    void onCarteDetectee(const QString &cin);
    void onEmployeIdentifie(const QString &cin, const QString &nom, const QString &prenom);
    void onEmployeInconnu(const QString &cin);
    void onMessageArduino(const QString &msg);
    void on_btn_connecter_arduino_clicked();   // bouton de connexion dans ton UI
    void on_btn_ecrire_carte_clicked();
    void on_recherche2_2_clicked();

    void on_appliquer_clicked();

    void on_exporter1_clicked();


    void on_appliquer2_clicked();

    void on_appliquer3_2_clicked();

    void on_exporter_2_clicked();

    void on_executer_clicked();

    void on_classement_clicked();
    void afficher_historique();

    void on_export_2_clicked();

        // ── Ventes : Recherche & Tri ──────────────────────────────
        void on_vente_rechercheClientBtn_clicked();
        void on_vente_rechercheDateBtn_clicked();
        void on_vente_triBox_currentIndexChanged(int index);

        // ── Ventes : Exports ─────────────────────────────────────
        void on_vente_exportCSVBtn_clicked();
        void on_vente_exportPDFBtn_clicked();

        // ── Ventes : Analyses ────────────────────────────────────
        void on_vente_analyserCABtn_clicked();
        void on_vente_detecterAnomaliesBtn_clicked();
        void on_vente_previsionBtn_clicked();
        void on_vente_graphiqueCABtn_clicked();

        void on_rendement_clicked();
        void on_anomalies_clicked();
        void on_graphique_clicked();

        // Slot pour le bouton ajouter
        void on_ajouter_clicked();

        // Slots pour les autres boutons du CRUD
        void on_modifier_clicked();
        void on_suprimer_clicked();
        void on_afficher_clicked();
        void on_tableWidget_cellClicked(int row, int column);

        // Slot pour l'export PDF
        void on_pushButton_5_clicked();

        // Slot pour afficher les données dans le tableWidget
        void afficherOperations();

        void effacerFiltreRecherche();

        // Slots pour le Chatbot

        void onOllamaResponse(QNetworkReply *reply);

        // Slots pour l'IA Observation
        void genererObservationIntelligente();
        void onOllamaObservationResponse(QNetworkReply *reply);

        // Arduino
        void onConnectArduinoClicked();
        void onDisconnectArduinoClicked();
        void onArduinoStatut(const QString &msg, bool connecte);
        void onArduinoFormulaire(QString idOp, QString idAgri, QString idMach);
        void onGreenCountChanged(int count);
        void onBlackCountChanged(int count);
        void recalculateAndAutoUpdateOliveWeight();

        void on_VERIFIER_clicked();

        void on_btn_logout_2_clicked();

    private:
    Ui::MainWindow *ui;
    QWidget* createSidebar(QWidget *parent);
    Employe Etmp;
    Agriculteur Atmp;
    Client Ctmp;
    Production Ptmp;
    Vente Vtmp;
    void refreshTable();
    void refreshTableAgriculteur();
    void refreshTableClient();
    void refreshTableProduction();
    void refreshTableVente();
    void updateRecherche();
    void remplirTableau(QSqlQuery query);
    void viderFormulaire();
    void viderFormulaireClient();
    void viderChampsProduction();
    void envoyerEmail(QString dest, QString code);
    void exporterTableViewExcel(QAbstractItemModel* model, const QString& titre);
    void afficherCourbeDansTable2();
    void afficherCourbe();
    void ouvrirChatbot();
    QString interrogerOllama(const QString &prompt);
    QString getInfosEmploye(const QString &question);
    QList<QPair<QString,QString>> historiqueChat;
    QChart *chart;
    QLineSeries *series;
    QTimer *timer;

    // ── Sidebar unique ────────────────────────────────────────

    void loadDashboardStats();
    // ── Production ────────────────────────────────────────────

    void rafraichirGrilleProduction(const QString &needle);

    // Méthode pour vider les champs après ajout
    void viderChamps();

    // Cerveau du chatbot local
    void traiterQuestionChatbot(const QString &question);

    QNetworkAccessManager *managerOllama;
    QNetworkAccessManager *managerEmail;
    QNetworkAccessManager *managerObservation; // Manager dédié à la génération d'observation

    // Fonction d'alerte Email
    void envoyerAlerteEmail(double rendement, const QString &idOperation);

    Arduino *m_arduino;   // ← ajouter
    int m_greenCount = 0; // green box count
    int m_blackCount = 0; // black box count
    void setupArduinoUI();
    void updateArduinoCountersLabel();
    //dashbord
    struct DashboardData; // forward declaration
    void setupDashboard(int nbEmp, int nbActifs, int nbConge, int nbSusp,
                        int nbAgri,
                        int nbCli, int nbGold, int nbSilver,
                        int nbVentes, double ca,
                        double huile, double rendement,
                        QList<int> ventesParMois, QStringList moisLabels);

    QString getContexteGlobal(const QString &question);


};
#endif // MAINWINDOW_H
