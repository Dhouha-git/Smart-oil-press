#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>
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

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
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
    void on_btn_chatbot_clicked();
    void on_btn_retour_chat_clicked();
    void on_btn_envoyer_chat_clicked();
    void onOllamaResponse(QNetworkReply *reply);

    // Slots pour l'IA Observation
    void genererObservationIntelligente();
    void onOllamaObservationResponse(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;

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
};

#endif // MAINWINDOW_H
