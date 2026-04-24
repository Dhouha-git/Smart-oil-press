#ifndef CLIENT_H
#define CLIENT_H


#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>
#include <QString>  // pour QString
#include <QDate>
#include <QSqlTableModel>
#include <QSqlError>
#include <QPrinter>  // 🔹 obligatoire
#include <QTextEdit>
#include <QTimer>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QMediaRecorder>
#include <QMediaCaptureSession>
#include <QAudioInput>
#include <QIODevice>
#include <QNetworkAccessManager>
#include <QMap>
#include <QVariantList>


namespace Ui {
class client;

}

QT_END_NAMESPACE

class client : public QMainWindow
{
    Q_OBJECT

public:
    client(QWidget *parent = nullptr);
    ~client();

private slots:
    void on_analyse_clicked();

    void on_chatbot_clicked();

    void on_retour_clicked();

    void on_historique_clicked();

    void on_classement_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();
private slots:
    void on_ajouter_clicked();
    void on_modifier_clicked();
    void on_recherche1_2_clicked();
void on_tri_2_clicked();
    void on_annuler_clicked();
void on_annuler1_clicked();
void on_pdf_clicked();
 void on_recherche2_2_clicked();
    void on_supprimer_clicked();
 void afficher_historique();








    void on_appliquer_clicked();

 void on_exporter1_clicked();

    void on_appliquer2_clicked();

 void on_appliquer3_clicked();

    void on_exporter2_clicked();

 void on_pushButton_4_clicked();

    void on_envoyer_clicked();

 /*void on_qr_clicked();*/


    void on_fichier_clicked();

    void on_nouvelle_discussion_clicked();

    void on_exporter2_2_clicked();

    void on_executer_clicked();

    void on_mode_clicked();

    void on_recherche_2_clicked();

private:
    Ui::client *ui;
     QTextEdit *chatBox;
    static const int MAX_DISCUSSIONS = 20;
     QLineSeries *seriesAchat;
     QLineSeries *seriesCumul;
     QMediaRecorder *recorder;
     QAudioInput *audioInput;
     QMediaCaptureSession captureSession;
     QMap<QString, QVariantList> conversations;
     QString fileName;
private:
    void verifierSaisie();
    void ajouterMessage(const QString &id, const QString &message);
    void updateListeConversations(const QString &id, const QString &lastMsg);
    void chargerClients();

    // ---- Attributs du client ----
    int id;
    QString nom;
    QString prenom;
    QString telephone;
    QString email;
    QString adresse;
    QDate date_inscription;
    QString type_client;
    QString type_huile_prefere;
    QString emballage_prefere;
    QString total_achat;
    QString points_fidelite;
    QSqlTableModel *model;
      int getNextId();
    QTimer *timer;
      QChart *chart;
      QLineSeries *series;

public:
    client(    int id,
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
           QString point);

    bool ajouter();
    bool modifier();
    static bool supprimer(int id);
    static void afficher(QSqlTableModel *model);
      void remplirTableau();
    void ajouterDiscussionRecente(const QString &nom, const QString &message);
void afficherCourbeDansTable2();
    void afficherCourbe();
 void applyTheme(QString mode);

};
#endif // CLIENT_H
