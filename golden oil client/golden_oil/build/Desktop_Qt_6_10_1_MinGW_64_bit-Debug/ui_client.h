/********************************************************************************
** Form generated from reading UI file 'client.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENT_H
#define UI_CLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_client
{
public:
    QWidget *centralwidget;
    QStackedWidget *stacked;
    QWidget *page_3;
    QFrame *frame_7;
    QPushButton *agriculteurs_3;
    QPushButton *ventes_3;
    QPushButton *production_3;
    QPushButton *employes_3;
    QPushButton *pushButton_4;
    QPushButton *parametre_3;
    QPushButton *deconnexion_3;
    QPushButton *aide_3;
    QLabel *label_26;
    QFrame *frame_8;
    QLabel *label_27;
    QLabel *label_29;
    QTextEdit *message_2;
    QPushButton *envoyer;
    QWidget *page;
    QFrame *frame_3;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLineEdit *nom_client;
    QLineEdit *id_client;
    QLineEdit *prenom_client;
    QLineEdit *tel;
    QLineEdit *email;
    QLineEdit *adresse;
    QLabel *label_4;
    QLabel *label_2;
    QRadioButton *particulier;
    QRadioButton *profisionnel;
    QPushButton *ajouter;
    QLabel *label_8;
    QDateEdit *date_inscription;
    QLabel *label_14;
    QCheckBox *checkBox;
    QCheckBox *checkBox_3;
    QLabel *label_15;
    QComboBox *emballage_prefere;
    QLabel *label_9;
    QLabel *label_16;
    QPushButton *modifier;
    QPushButton *annuler;
    QLabel *label_20;
    QLabel *label_21;
    QSpinBox *total_achat;
    QSpinBox *points_fidelite;
    QPushButton *afficher;
    QFrame *frame_2;
    QPushButton *agriculteurs;
    QPushButton *ventes;
    QPushButton *production;
    QPushButton *employes;
    QPushButton *parametre;
    QPushButton *deconnexion;
    QPushButton *aide;
    QLabel *label_12;
    QPushButton *pushButton_2;
    QPushButton *retour;
    QTableWidget *table;
    QFrame *frame_5;
    QLabel *label_28;
    QPushButton *supprimer;
    QPushButton *annuler1;
    QLineEdit *id;
    QLineEdit *recherche1;
    QPushButton *recherche1_2;
    QLineEdit *tri;
    QPushButton *tri_2;
    QFrame *frame;
    QPushButton *analyse;
    QPushButton *chatbot;
    QPushButton *pdf;
    QLabel *label_18;
    QWidget *page_2;
    QFrame *frame_4;
    QPushButton *production_2;
    QPushButton *employes_2;
    QPushButton *ventes_2;
    QPushButton *agriculteurs_2;
    QPushButton *deconnexion_2;
    QPushButton *pushButton_3;
    QLabel *label_10;
    QPushButton *parametre_2;
    QPushButton *aide_2;
    QLabel *label_17;
    QFrame *frame_6;
    QLabel *label_19;
    QLabel *label_11;
    QTableWidget *table1;
    QLineEdit *recherche2;
    QLabel *label_22;
    QPushButton *recherche2_2;
    QLabel *label_23;
    QDateEdit *date_2;
    QPushButton *appliquer;
    QLabel *label_24;
    QRadioButton *particulier_2;
    QRadioButton *profisionnel_2;
    QLabel *label_25;
    QCheckBox *vierge;
    QCheckBox *bio;
    QTableWidget *table2;
    QPushButton *pushButton;
    QPushButton *classement;
    QPushButton *historique;
    QWidget *page_4;
    QFrame *frame_9;
    QPushButton *agriculteurs_4;
    QPushButton *ventes_4;
    QPushButton *production_4;
    QPushButton *employes_4;
    QPushButton *pushButton_5;
    QPushButton *parametre_4;
    QPushButton *deconnexion_4;
    QPushButton *aide_4;
    QLabel *label_30;
    QFrame *frame_10;
    QLabel *label_31;
    QTableWidget *table3;
    QPushButton *exporter1;
    QWidget *page_5;
    QFrame *frame_11;
    QPushButton *agriculteurs_5;
    QPushButton *ventes_5;
    QPushButton *production_5;
    QPushButton *employes_5;
    QPushButton *pushButton_6;
    QPushButton *parametre_5;
    QPushButton *deconnexion_5;
    QPushButton *aide_5;
    QLabel *label_32;
    QTableWidget *table4;
    QTableWidget *table5;
    QLabel *label_33;
    QLabel *label_34;
    QFrame *frame_12;
    QLabel *label_35;
    QLabel *label_36;
    QRadioButton *particulier_3;
    QRadioButton *profisionnel_4;
    QLabel *label_37;
    QLCDNumber *points_2;
    QLabel *label_38;
    QPushButton *appliquer2;
    QLabel *label_39;
    QRadioButton *particulier_4;
    QRadioButton *profisionnel_5;
    QPushButton *appliquer3;
    QPushButton *exporter2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *client)
    {
        if (client->objectName().isEmpty())
            client->setObjectName("client");
        client->resize(1592, 793);
        centralwidget = new QWidget(client);
        centralwidget->setObjectName("centralwidget");
        stacked = new QStackedWidget(centralwidget);
        stacked->setObjectName("stacked");
        stacked->setGeometry(QRect(-40, -50, 1841, 1191));
        QFont font;
        font.setFamilies({QString::fromUtf8("Yu Gothic UI")});
        font.setPointSize(12);
        font.setBold(false);
        stacked->setFont(font);
        stacked->setStyleSheet(QString::fromUtf8("/* ================= STACKED WIDGET CONTAINER ================= */\n"
"QStackedWidget {\n"
"    background-color: transparent;\n"
"    border-radius: 16px;\n"
"}\n"
"\n"
"/* Chaque page du stacked (QWidget interne) */\n"
"QStackedWidget > QWidget {\n"
"    background-color: #FBFBF8;\n"
"    border-radius: 16px;\n"
"    border: 2px solid #C9D4B6;\n"
"}\n"
"\n"
"/* ================= TITRES DES PAGES ================= */\n"
"QLabel#pageTitle {\n"
"    color: #556B2F;\n"
"    font-size: 16pt;\n"
"    font-weight: bold;\n"
"    padding-bottom: 8px;\n"
"}\n"
"\n"
"/* ================= NAVIGATION (boutons menu gauche ou haut) ================= */\n"
"QPushButton#navBtn {\n"
"    background-color: transparent;\n"
"    color: #2F3E1E;\n"
"    border-radius: 10px;\n"
"    padding: 8px 14px;\n"
"    text-align: left;\n"
"    font-weight: 600;\n"
"}\n"
"\n"
"/* hover menu */\n"
"QPushButton#navBtn:hover {\n"
"    background-color: #E8EEDC;\n"
"}\n"
"\n"
"/* bouton actif (page courante) */\n"
"QPushButton#navBtn:checked {\n"
""
                        "    background-color: #6B8E23;\n"
"    color: white;\n"
"}\n"
"\n"
"/* ================= BARRE MENU LATERALE ================= */\n"
"QFrame#sideMenu {\n"
"    background-color: #EEF3E3;\n"
"    border-right: 2px solid #C9D4B6;\n"
"    border-radius: 0px;\n"
"}\n"
"\n"
"/* icone dans menu */\n"
"QPushButton#navBtn::icon {\n"
"    padding-left: 5px;\n"
"}\n"
"\n"
"/* ================= ANIMATION VISUELLE (effet s\303\251lection) ================= */\n"
"QPushButton#navBtn:pressed {\n"
"    background-color: #556B2F;\n"
"    color: white;\n"
"}\n"
"\n"
"\n"
""));
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        frame_7 = new QFrame(page_3);
        frame_7->setObjectName("frame_7");
        frame_7->setGeometry(QRect(-10, -20, 161, 851));
        frame_7->setStyleSheet(QString::fromUtf8("/* Couleur de fond du frame */\n"
"background-color: olive;\n"
"\n"
"/* Bordure fine et arrondie pour un effet moderne */\n"
"border: 1px solid #556B2F; \n"
"border-radius: 8px;\n"
"\n"
"/* Optionnel : ajoute un peu d'ombre pour plus de relief */\n"
"box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2);\n"
"\n"
"/* Style pour les labels et champs \303\240 l'int\303\251rieur */\n"
"label {\n"
"    font-family: Arial, sans-serif;\n"
"    font-size: 12pt;\n"
"    color: #fff; /* texte en blanc pour contraste */\n"
"}\n"
"\n"
"input, select, textarea {\n"
"    border-radius: 4px;\n"
"    border: 1px solid #ccc;\n"
"    padding: 3px 5px;\n"
"}\n"
"/* Couleur de fond du frame */\n"
"background-color: olive;\n"
"\n"
"/* Bordure fine et arrondie pour un effet moderne */\n"
"border: 1px solid #556B2F; \n"
"border-radius: 8px;\n"
"\n"
"/* Optionnel : ajoute un peu d'ombre pour plus de relief */\n"
"box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2);\n"
"\n"
"/* Style pour les labels et champs \303\240 l'int\303\251rieur */\n"
"label {\n"
""
                        "    font-family: Arial, sans-serif;\n"
"    font-size: 12pt;\n"
"    color: #fff; /* texte en blanc pour contraste */\n"
"}\n"
"\n"
"input, select, textarea {\n"
"    border-radius: 4px;\n"
"    border: 1px solid #ccc;\n"
"    padding: 3px 5px;\n"
"}\n"
""));
        frame_7->setFrameShape(QFrame::Shape::StyledPanel);
        frame_7->setFrameShadow(QFrame::Shadow::Raised);
        agriculteurs_3 = new QPushButton(frame_7);
        agriculteurs_3->setObjectName("agriculteurs_3");
        agriculteurs_3->setGeometry(QRect(0, 300, 181, 42));
        ventes_3 = new QPushButton(frame_7);
        ventes_3->setObjectName("ventes_3");
        ventes_3->setGeometry(QRect(0, 340, 181, 42));
        production_3 = new QPushButton(frame_7);
        production_3->setObjectName("production_3");
        production_3->setGeometry(QRect(-10, 380, 181, 42));
        employes_3 = new QPushButton(frame_7);
        employes_3->setObjectName("employes_3");
        employes_3->setGeometry(QRect(-10, 260, 181, 42));
        pushButton_4 = new QPushButton(frame_7);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(0, 140, 181, 42));
        parametre_3 = new QPushButton(frame_7);
        parametre_3->setObjectName("parametre_3");
        parametre_3->setGeometry(QRect(-10, 660, 181, 42));
        deconnexion_3 = new QPushButton(frame_7);
        deconnexion_3->setObjectName("deconnexion_3");
        deconnexion_3->setGeometry(QRect(0, 750, 181, 42));
        aide_3 = new QPushButton(frame_7);
        aide_3->setObjectName("aide_3");
        aide_3->setGeometry(QRect(0, 700, 181, 51));
        label_26 = new QLabel(frame_7);
        label_26->setObjectName("label_26");
        label_26->setGeometry(QRect(-10, 10, 181, 131));
        label_26->setStyleSheet(QString::fromUtf8("QLabel#label_logo {\n"
"    background-color: transparent;\n"
"    padding: 10px;\n"
"}\n"
"\n"
"/* Option : zone du header o\303\271 se trouve le logo */\n"
"QFrame#header {\n"
"    background-color: white;\n"
"    border-bottom: 1px solid #e0e0e0;\n"
"}\n"
"\n"
"/* Style du titre \303\240 c\303\264t\303\251 du logo */\n"
"QLabel#appTitle {\n"
"    color: #556B2F;      /* vert olive */\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    font-family: Poppins;\n"
"}\n"
""));
        label_26->setPixmap(QPixmap(QString::fromUtf8("../../../Downloads/golden-removebg-preview.png")));
        label_26->setScaledContents(true);
        frame_8 = new QFrame(page_3);
        frame_8->setObjectName("frame_8");
        frame_8->setGeometry(QRect(150, 0, 1521, 91));
        frame_8->setStyleSheet(QString::fromUtf8("/* Couleur de fond du frame */\n"
"background-color: olive;\n"
"\n"
"/* Bordure fine et arrondie pour un effet moderne */\n"
"border: 1px solid #556B2F; \n"
"border-radius: 8px;\n"
"\n"
"/* Optionnel : ajoute un peu d'ombre pour plus de relief */\n"
"box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2);\n"
"\n"
"/* Style pour les labels et champs \303\240 l'int\303\251rieur */\n"
"label {\n"
"    font-family: Arial, sans-serif;\n"
"    font-size: 12pt;\n"
"    color: #fff; /* texte en blanc pour contraste */\n"
"}\n"
"\n"
"input, select, textarea {\n"
"    border-radius: 4px;\n"
"    border: 1px solid #ccc;\n"
"    padding: 3px 5px;\n"
"}\n"
"/* Couleur de fond du frame */\n"
"background-color: olive;\n"
"\n"
"/* Bordure fine et arrondie pour un effet moderne */\n"
"border: 1px solid #556B2F; \n"
"border-radius: 8px;\n"
"\n"
"/* Optionnel : ajoute un peu d'ombre pour plus de relief */\n"
"box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2);\n"
"\n"
"/* Style pour les labels et champs \303\240 l'int\303\251rieur */\n"
"label {\n"
""
                        "    font-family: Arial, sans-serif;\n"
"    font-size: 12pt;\n"
"    color: #fff; /* texte en blanc pour contraste */\n"
"}\n"
"\n"
"input, select, textarea {\n"
"    border-radius: 4px;\n"
"    border: 1px solid #ccc;\n"
"    padding: 3px 5px;\n"
"}\n"
""));
        frame_8->setFrameShape(QFrame::Shape::StyledPanel);
        frame_8->setFrameShadow(QFrame::Shadow::Raised);
        label_27 = new QLabel(frame_8);
        label_27->setObjectName("label_27");
        label_27->setGeometry(QRect(0, 0, 1511, 101));
        label_27->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";"));
        label_29 = new QLabel(page_3);
        label_29->setObjectName("label_29");
        label_29->setGeometry(QRect(320, 160, 701, 231));
        label_29->setStyleSheet(QString::fromUtf8("font: 14pt \"Segoe UI\";\n"
"font: 20pt \"Segoe UI\";"));
        message_2 = new QTextEdit(page_3);
        message_2->setObjectName("message_2");
        message_2->setGeometry(QRect(340, 630, 681, 91));
        message_2->setStyleSheet(QString::fromUtf8("/* Container g\303\251n\303\251ral du message IA */\n"
".chat-bubble.ia {\n"
"    background-color: #F3F6ED;   /* couleur claire, fond de l'IA */\n"
"    color: #2E3B1F;             /* texte vert fonc\303\251 */\n"
"    font-family: 'Segoe UI', sans-serif;\n"
"    font-size: 12pt;\n"
"    padding: 10px 15px;         /* espace int\303\251rieur */\n"
"    border-radius: 15px;        /* arrondi des coins */\n"
"    margin: 5px 0;              /* espace entre messages */\n"
"    max-width: 70%;             /* largeur maximale */\n"
"    word-wrap: break-word;      /* retour \303\240 la ligne automatique */\n"
"    box-shadow: 1px 1px 5px rgba(0,0,0,0.1); /* l\303\251g\303\250re ombre */\n"
"}\n"
"\n"
"/* Option pour aligner le message IA \303\240 gauche */\n"
".chat-bubble.ia.left {\n"
"    margin-left: 10px;\n"
"    margin-right: auto;\n"
"}\n"
"\n"
"/* Option pour aligner le message IA \303\240 droite (ex: messages utilisateur) */\n"
".chat-bubble.ia.right {\n"
"    margin-left: auto;\n"
"    margin-right: 10px;"
                        "\n"
"    background-color: #D1E8FF;   /* couleur diff\303\251rente pour utilisateur */\n"
"    color: #003366;\n"
"}\n"
""));
        envoyer = new QPushButton(page_3);
        envoyer->setObjectName("envoyer");
        envoyer->setGeometry(QRect(1050, 660, 93, 29));
        stacked->addWidget(page_3);
        page = new QWidget();
        page->setObjectName("page");
        frame_3 = new QFrame(page);
        frame_3->setObjectName("frame_3");
        frame_3->setGeometry(QRect(180, 150, 1001, 361));
        frame_3->setStyleSheet(QString::fromUtf8("/* ================= BACKGROUND GLOBAL ================= */\n"
"QMainWindow {\n"
"    background-color: #F4F6EF;\n"
"    font-family: \"Segoe UI\";\n"
"    font-size: 10pt;\n"
"}\n"
"\n"
"/* ================= GROUPBOX / PANELS ================= */\n"
"QGroupBox {\n"
"    background-color: #FBFBF8;\n"
"    border: 2px solid #6B8E23;\n"
"    border-radius: 14px;\n"
"    margin-top: 12px;\n"
"    padding: 15px;\n"
"}\n"
"\n"
"QGroupBox:title {\n"
"    subcontrol-origin: margin;\n"
"    left: 15px;\n"
"    padding: 2px 10px;\n"
"    background-color: #6B8E23;\n"
"    color: white;\n"
"    border-radius: 8px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"/* ================= LABELS ================= */\n"
"QLabel {\n"
"    color: #2F3E1E;\n"
"    font-weight: 500;\n"
"}\n"
"\n"
"/* ================= INPUT FIELDS (ULTRA COMPACT) ================= */\n"
"QLineEdit, QTextEdit, QDateEdit, QSpinBox, QComboBox {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"    border-bottom: 2px solid #B7C79A;\n"
"    pad"
                        "ding: 3px 4px;\n"
"    min-height: 22px;\n"
"}\n"
"\n"
"QLineEdit:focus, QTextEdit:focus, QDateEdit:focus, QSpinBox:focus, QComboBox:focus {\n"
"    border-bottom: 2px solid #6B8E23;\n"
"}\n"
"\n"
"/* ================= TABLE / ZONES RESULTATS ================= */\n"
"QTableWidget, QTableView, QListWidget, QTextBrowser {\n"
"    background-color: white;\n"
"    border: 2px solid #C9D4B6;\n"
"    border-radius: 12px;\n"
"    gridline-color: #E2E8D5;\n"
"}\n"
"\n"
"/* Header table */\n"
"QHeaderView::section {\n"
"    background-color: #6B8E23;\n"
"    color: white;\n"
"    padding: 4px;\n"
"    border: none;\n"
"}\n"
"\n"
"/* ================= BUTTONS PRINCIPAUX ================= */\n"
"QPushButton {\n"
"    background-color: #556B2F;\n"
"    color: white;\n"
"    border-radius: 12px;\n"
"    padding: 6px 18px;\n"
"    font-weight: bold;\n"
"    min-height: 28px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #6B8E23;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #4C5F28;\n"
"}\n"
""
                        "\n"
"/* ================= BOUTONS SECONDAIRES (Supprimer / Modifier) ================= */\n"
"QPushButton#secondaryBtn {\n"
"    background-color: #8FAF3C;\n"
"    border-radius: 10px;\n"
"}\n"
"\n"
"QPushButton#secondaryBtn:hover {\n"
"    background-color: #6B8E23;\n"
"}\n"
"\n"
"/* ================= BOUTON EXPORTER ================= */\n"
"QPushButton#exportBtn {\n"
"    background-color: #3F5122;\n"
"    border-radius: 10px;\n"
"}\n"
"\n"
"QPushButton#exportBtn:hover {\n"
"    background-color: #556B2F;\n"
"}\n"
"\n"
"/* ================= PANELS BAS (Assistant / V\303\251rification) ================= */\n"
"QFrame#bottomPanel {\n"
"    background-color: #6B8E23;\n"
"    border-radius: 18px;\n"
"    padding: 12px;\n"
"}\n"
"\n"
"QFrame#bottomPanel QLabel {\n"
"    color: white;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"/* zone texte assistant */\n"
"QTextEdit#assistantBox {\n"
"    background-color: #F8FAF3;\n"
"    border-radius: 12px;\n"
"    border: 2px solid #556B2F;\n"
"}\n"
"\n"
"/* ================= "
                        "SCROLLBAR MODERNE ================= */\n"
"QScrollBar:vertical {\n"
"    border: none;\n"
"    background: #EEF3E3;\n"
"    width: 10px;\n"
"    margin: 0px;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical {\n"
"    background: #6B8E23;\n"
"    border-radius: 5px;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical:hover {\n"
"    background: #556B2F;\n"
"}\n"
"\n"
""));
        frame_3->setFrameShape(QFrame::Shape::StyledPanel);
        frame_3->setFrameShadow(QFrame::Shadow::Raised);
        label = new QLabel(frame_3);
        label->setObjectName("label");
        label->setGeometry(QRect(0, 10, 63, 20));
        label_3 = new QLabel(frame_3);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(300, 0, 141, 31));
        label_5 = new QLabel(frame_3);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(0, 80, 121, 31));
        label_6 = new QLabel(frame_3);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(310, 80, 111, 20));
        label_7 = new QLabel(frame_3);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(600, 80, 121, 21));
        nom_client = new QLineEdit(frame_3);
        nom_client->setObjectName("nom_client");
        nom_client->setGeometry(QRect(270, 20, 281, 38));
        id_client = new QLineEdit(frame_3);
        id_client->setObjectName("id_client");
        id_client->setGeometry(QRect(-10, 20, 281, 38));
        prenom_client = new QLineEdit(frame_3);
        prenom_client->setObjectName("prenom_client");
        prenom_client->setGeometry(QRect(570, 20, 281, 38));
        tel = new QLineEdit(frame_3);
        tel->setObjectName("tel");
        tel->setGeometry(QRect(-10, 120, 281, 38));
        email = new QLineEdit(frame_3);
        email->setObjectName("email");
        email->setGeometry(QRect(290, 120, 281, 38));
        adresse = new QLineEdit(frame_3);
        adresse->setObjectName("adresse");
        adresse->setGeometry(QRect(590, 120, 281, 38));
        label_4 = new QLabel(frame_3);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(560, -10, 141, 41));
        label_2 = new QLabel(frame_3);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(280, 160, 51, 41));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Segoe UI")});
        font1.setPointSize(10);
        font1.setWeight(QFont::Medium);
        label_2->setFont(font1);
        particulier = new QRadioButton(frame_3);
        particulier->setObjectName("particulier");
        particulier->setGeometry(QRect(260, 210, 181, 41));
        profisionnel = new QRadioButton(frame_3);
        profisionnel->setObjectName("profisionnel");
        profisionnel->setGeometry(QRect(380, 210, 161, 41));
        ajouter = new QPushButton(frame_3);
        ajouter->setObjectName("ajouter");
        ajouter->setGeometry(QRect(500, 320, 121, 40));
        ajouter->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #D4AF37;\n"
"    color: black;\n"
"    border-radius: 6px;\n"
"    padding: 6px 12px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #c49b2f;\n"
"}"));
        label_8 = new QLabel(frame_3);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(0, 160, 191, 41));
        date_inscription = new QDateEdit(frame_3);
        date_inscription->setObjectName("date_inscription");
        date_inscription->setGeometry(QRect(10, 210, 161, 42));
        label_14 = new QLabel(frame_3);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(580, 170, 151, 20));
        checkBox = new QCheckBox(frame_3);
        checkBox->setObjectName("checkBox");
        checkBox->setGeometry(QRect(620, 220, 91, 24));
        checkBox_3 = new QCheckBox(frame_3);
        checkBox_3->setObjectName("checkBox_3");
        checkBox_3->setGeometry(QRect(740, 220, 91, 24));
        label_15 = new QLabel(frame_3);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(10, 260, 221, 20));
        emballage_prefere = new QComboBox(frame_3);
        emballage_prefere->addItem(QString());
        emballage_prefere->addItem(QString());
        emballage_prefere->addItem(QString());
        emballage_prefere->addItem(QString());
        emballage_prefere->setObjectName("emballage_prefere");
        emballage_prefere->setGeometry(QRect(30, 300, 71, 42));
        label_9 = new QLabel(frame_3);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(600, 260, 171, 21));
        label_16 = new QLabel(frame_3);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(680, 290, 63, 20));
        modifier = new QPushButton(frame_3);
        modifier->setObjectName("modifier");
        modifier->setGeometry(QRect(620, 320, 121, 40));
        modifier->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #D4AF37;\n"
"    color: black;\n"
"    border-radius: 6px;\n"
"    padding: 6px 12px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #c49b2f;\n"
"}"));
        annuler = new QPushButton(frame_3);
        annuler->setObjectName("annuler");
        annuler->setGeometry(QRect(740, 320, 121, 40));
        annuler->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #D4AF37;\n"
"    color: black;\n"
"    border-radius: 6px;\n"
"    padding: 6px 12px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #c49b2f;\n"
"}"));
        label_20 = new QLabel(frame_3);
        label_20->setObjectName("label_20");
        label_20->setGeometry(QRect(260, 270, 111, 20));
        label_21 = new QLabel(frame_3);
        label_21->setObjectName("label_21");
        label_21->setGeometry(QRect(340, 310, 63, 20));
        total_achat = new QSpinBox(frame_3);
        total_achat->setObjectName("total_achat");
        total_achat->setGeometry(QRect(280, 310, 42, 30));
        points_fidelite = new QSpinBox(frame_3);
        points_fidelite->setObjectName("points_fidelite");
        points_fidelite->setGeometry(QRect(630, 290, 42, 30));
        afficher = new QPushButton(frame_3);
        afficher->setObjectName("afficher");
        afficher->setGeometry(QRect(860, 320, 141, 40));
        afficher->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #D4AF37;\n"
"    color: black;\n"
"    border-radius: 6px;\n"
"    padding: 6px 12px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #c49b2f;\n"
"}"));
        frame_2 = new QFrame(page);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(0, 0, 151, 901));
        frame_2->setStyleSheet(QString::fromUtf8("QFrame {\n"
"    background-color: #556B2F;\n"
"} QFrame#sidebar_5 {\n"
"    background-color: #556B2F;\n"
"}\n"
"QPushButton {\n"
"    background-color: transparent;\n"
"    color: white;\n"
"    border: 2px solid #D4AF37;\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    margin: 8px 12px;\n"
"    text-align: left;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #D4AF37;\n"
"    color: black;\n"
"}"));
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        agriculteurs = new QPushButton(frame_2);
        agriculteurs->setObjectName("agriculteurs");
        agriculteurs->setGeometry(QRect(-10, 290, 171, 71));
        ventes = new QPushButton(frame_2);
        ventes->setObjectName("ventes");
        ventes->setGeometry(QRect(-10, 350, 171, 71));
        production = new QPushButton(frame_2);
        production->setObjectName("production");
        production->setGeometry(QRect(-10, 410, 171, 71));
        employes = new QPushButton(frame_2);
        employes->setObjectName("employes");
        employes->setGeometry(QRect(-10, 230, 171, 71));
        parametre = new QPushButton(frame_2);
        parametre->setObjectName("parametre");
        parametre->setGeometry(QRect(-10, 670, 171, 61));
        deconnexion = new QPushButton(frame_2);
        deconnexion->setObjectName("deconnexion");
        deconnexion->setGeometry(QRect(-10, 770, 171, 61));
        aide = new QPushButton(frame_2);
        aide->setObjectName("aide");
        aide->setGeometry(QRect(-10, 720, 171, 61));
        label_12 = new QLabel(frame_2);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(0, -10, 181, 131));
        label_12->setStyleSheet(QString::fromUtf8("QLabel#label_logo {\n"
"    background-color: transparent;\n"
"    padding: 10px;\n"
"}\n"
"\n"
"/* Option : zone du header o\303\271 se trouve le logo */\n"
"QFrame#header {\n"
"    background-color: white;\n"
"    border-bottom: 1px solid #e0e0e0;\n"
"}\n"
"\n"
"/* Style du titre \303\240 c\303\264t\303\251 du logo */\n"
"QLabel#appTitle {\n"
"    color: #556B2F;      /* vert olive */\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    font-family: Poppins;\n"
"}\n"
""));
        label_12->setPixmap(QPixmap(QString::fromUtf8("../../../Downloads/golden-removebg-preview.png")));
        label_12->setScaledContents(true);
        pushButton_2 = new QPushButton(frame_2);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(-10, 180, 171, 61));
        retour = new QPushButton(frame_2);
        retour->setObjectName("retour");
        retour->setGeometry(QRect(-10, 120, 171, 71));
        table = new QTableWidget(page);
        if (table->columnCount() < 13)
            table->setColumnCount(13);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        table->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(9, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(10, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(11, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(12, __qtablewidgetitem12);
        table->setObjectName("table");
        table->setGeometry(QRect(170, 580, 1221, 221));
        table->setStyleSheet(QString::fromUtf8("QTableView {\n"
"    background-color: white;\n"
"    border: 2px solid #D4AF37;   /* jaune dor\303\251 */\n"
"    gridline-color: #D4AF37;\n"
"    border-radius: 6px;\n"
"    selection-background-color: #FFF3C4;\n"
"    selection-color: black;\n"
"}\n"
""));
        frame_5 = new QFrame(page);
        frame_5->setObjectName("frame_5");
        frame_5->setGeometry(QRect(1200, 360, 301, 141));
        frame_5->setFrameShape(QFrame::Shape::StyledPanel);
        frame_5->setFrameShadow(QFrame::Shadow::Raised);
        label_28 = new QLabel(frame_5);
        label_28->setObjectName("label_28");
        label_28->setGeometry(QRect(10, 30, 63, 20));
        supprimer = new QPushButton(frame_5);
        supprimer->setObjectName("supprimer");
        supprimer->setGeometry(QRect(0, 100, 161, 42));
        supprimer->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #D4AF37;\n"
"    color: black;\n"
"    border-radius: 6px;\n"
"    padding: 6px 12px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #c49b2f;\n"
"}"));
        annuler1 = new QPushButton(frame_5);
        annuler1->setObjectName("annuler1");
        annuler1->setGeometry(QRect(160, 100, 151, 42));
        annuler1->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #D4AF37;\n"
"    color: black;\n"
"    border-radius: 6px;\n"
"    padding: 6px 12px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #c49b2f;\n"
"}"));
        id = new QLineEdit(frame_5);
        id->setObjectName("id");
        id->setGeometry(QRect(90, 30, 161, 42));
        recherche1 = new QLineEdit(page);
        recherche1->setObjectName("recherche1");
        recherche1->setGeometry(QRect(170, 550, 241, 26));
        recherche1->setStyleSheet(QString::fromUtf8("/* ===== SEARCH BAR ===== */\n"
"QLineEdit#searchEdit {\n"
"    background-color: #F1F5EA;\n"
" \n"
"    border-radius: 16px;\n"
"    padding: 6px 32px 6px 12px; /* espace \303\240 droite pour ic\303\264ne */\n"
"    font-size: 10pt;\n"
"    color: #2F3E1E;\n"
"  border: 2px solid #D4AF37;   /* jaune dor\303\251 */\n"
"}\n"
"\n"
"QLineEdit#searchEdit:focus {\n"
"    border: 2px solid #6B8E23;\n"
"    background-color: white;\n"
"}\n"
"\n"
"QLineEdit#searchEdit:hover {\n"
"    border: 2px solid #8FAF3C;\n"
"}\n"
""));
        recherche1_2 = new QPushButton(page);
        recherche1_2->setObjectName("recherche1_2");
        recherche1_2->setGeometry(QRect(410, 550, 93, 29));
        tri = new QLineEdit(page);
        tri->setObjectName("tri");
        tri->setGeometry(QRect(590, 550, 191, 21));
        tri_2 = new QPushButton(page);
        tri_2->setObjectName("tri_2");
        tri_2->setGeometry(QRect(790, 550, 93, 29));
        frame = new QFrame(page);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(0, 0, 1851, 91));
        frame->setStyleSheet(QString::fromUtf8("QFrame {\n"
"    background-color: #556B2F;\n"
"}"));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        analyse = new QPushButton(page);
        analyse->setObjectName("analyse");
        analyse->setGeometry(QRect(1220, 140, 161, 51));
        analyse->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"   \n"
"color: #556B2F;\n"
"}\n"
"\n"
""));
        chatbot = new QPushButton(page);
        chatbot->setObjectName("chatbot");
        chatbot->setGeometry(QRect(1220, 190, 161, 61));
        chatbot->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #6B4F3F\n"
"    color: black;\n"
"    border-radius: 6px;\n"
"    padding: 6px 12px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #c49b2f;\n"
"}\n"
""));
        pdf = new QPushButton(page);
        pdf->setObjectName("pdf");
        pdf->setGeometry(QRect(1220, 250, 161, 51));
        label_18 = new QLabel(page);
        label_18->setObjectName("label_18");
        label_18->setGeometry(QRect(170, 70, 361, 91));
        label_18->setStyleSheet(QString::fromUtf8("font: 350 18pt \"Segoe UI\";"));
        stacked->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        frame_4 = new QFrame(page_2);
        frame_4->setObjectName("frame_4");
        frame_4->setGeometry(QRect(-20, 0, 201, 1191));
        frame_4->setStyleSheet(QString::fromUtf8("/* Couleur de fond du frame */\n"
"background-color: olive;\n"
"\n"
"/* Bordure fine et arrondie pour un effet moderne */\n"
"border: 1px solid #556B2F; \n"
"border-radius: 8px;\n"
"\n"
"/* Optionnel : ajoute un peu d'ombre pour plus de relief */\n"
"box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2);\n"
"\n"
"/* Style pour les labels et champs \303\240 l'int\303\251rieur */\n"
"label {\n"
"    font-family: Arial, sans-serif;\n"
"    font-size: 12pt;\n"
"    color: #fff; /* texte en blanc pour contraste */\n"
"}\n"
"\n"
"input, select, textarea {\n"
"    border-radius: 4px;\n"
"    border: 1px solid #ccc;\n"
"    padding: 3px 5px;\n"
"}\n"
"/* Couleur de fond du frame */\n"
"background-color: olive;\n"
"\n"
"/* Bordure fine et arrondie pour un effet moderne */\n"
"border: 1px solid #556B2F; \n"
"border-radius: 8px;\n"
"\n"
"/* Optionnel : ajoute un peu d'ombre pour plus de relief */\n"
"box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2);\n"
"\n"
"/* Style pour les labels et champs \303\240 l'int\303\251rieur */\n"
"label {\n"
""
                        "    font-family: Arial, sans-serif;\n"
"    font-size: 12pt;\n"
"    color: #fff; /* texte en blanc pour contraste */\n"
"}\n"
"\n"
"input, select, textarea {\n"
"    border-radius: 4px;\n"
"    border: 1px solid #ccc;\n"
"    padding: 3px 5px;\n"
"}\n"
""));
        frame_4->setFrameShape(QFrame::Shape::StyledPanel);
        frame_4->setFrameShadow(QFrame::Shadow::Raised);
        production_2 = new QPushButton(frame_4);
        production_2->setObjectName("production_2");
        production_2->setGeometry(QRect(0, 380, 181, 41));
        employes_2 = new QPushButton(frame_4);
        employes_2->setObjectName("employes_2");
        employes_2->setGeometry(QRect(0, 260, 181, 41));
        ventes_2 = new QPushButton(frame_4);
        ventes_2->setObjectName("ventes_2");
        ventes_2->setGeometry(QRect(0, 300, 181, 41));
        agriculteurs_2 = new QPushButton(frame_4);
        agriculteurs_2->setObjectName("agriculteurs_2");
        agriculteurs_2->setGeometry(QRect(0, 340, 181, 41));
        deconnexion_2 = new QPushButton(frame_4);
        deconnexion_2->setObjectName("deconnexion_2");
        deconnexion_2->setGeometry(QRect(10, 1130, 201, 61));
        pushButton_3 = new QPushButton(frame_4);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(0, 120, 181, 41));
        label_10 = new QLabel(frame_4);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(180, -30, 1181, 101));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Segoe UI")});
        font2.setPointSize(13);
        font2.setWeight(QFont::DemiBold);
        label_10->setFont(font2);
        parametre_2 = new QPushButton(frame_4);
        parametre_2->setObjectName("parametre_2");
        parametre_2->setGeometry(QRect(0, 500, 181, 42));
        aide_2 = new QPushButton(frame_4);
        aide_2->setObjectName("aide_2");
        aide_2->setGeometry(QRect(0, 540, 181, 51));
        label_17 = new QLabel(frame_4);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(0, -20, 201, 141));
        label_17->setStyleSheet(QString::fromUtf8("QLabel#label_logo {\n"
"    background-color: transparent;\n"
"    padding: 10px;\n"
"}\n"
"\n"
"/* Option : zone du header o\303\271 se trouve le logo */\n"
"QFrame#header {\n"
"    background-color: white;\n"
"    border-bottom: 1px solid #e0e0e0;\n"
"}\n"
"\n"
"/* Style du titre \303\240 c\303\264t\303\251 du logo */\n"
"QLabel#appTitle {\n"
"    color: #556B2F;      /* vert olive */\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    font-family: Poppins;\n"
"}\n"
""));
        label_17->setPixmap(QPixmap(QString::fromUtf8("../../../Downloads/golden-removebg-preview.png")));
        label_17->setScaledContents(true);
        frame_6 = new QFrame(page_2);
        frame_6->setObjectName("frame_6");
        frame_6->setGeometry(QRect(160, 0, 1691, 91));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush);
        QBrush brush1(QColor(128, 128, 0, 255));
        brush1.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush1);
        QBrush brush2(QColor(0, 127, 0, 255));
        brush2.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Light, brush2);
        QBrush brush3(QColor(0, 106, 0, 255));
        brush3.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Midlight, brush3);
        QBrush brush4(QColor(0, 42, 0, 255));
        brush4.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Dark, brush4);
        QBrush brush5(QColor(0, 57, 0, 255));
        brush5.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Mid, brush5);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Text, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::BrightText, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ButtonText, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush1);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush1);
        QBrush brush6(QColor(0, 0, 0, 255));
        brush6.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Shadow, brush6);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::AlternateBase, brush4);
        QBrush brush7(QColor(255, 255, 220, 255));
        brush7.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ToolTipBase, brush7);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ToolTipText, brush6);
        QBrush brush8(QColor(255, 255, 255, 127));
        brush8.setStyle(Qt::BrushStyle::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::PlaceholderText, brush8);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Accent, brush6);
#endif
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush1);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Light, brush2);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Midlight, brush3);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Dark, brush4);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Mid, brush5);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Text, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::BrightText, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ButtonText, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush1);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush1);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Shadow, brush6);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::AlternateBase, brush4);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ToolTipBase, brush7);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ToolTipText, brush6);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::PlaceholderText, brush8);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Accent, brush6);
#endif
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::WindowText, brush4);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Light, brush2);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Midlight, brush3);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Dark, brush4);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Mid, brush5);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Text, brush4);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::BrightText, brush);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ButtonText, brush4);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Shadow, brush6);
        QBrush brush9(QColor(0, 85, 0, 255));
        brush9.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::AlternateBase, brush9);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ToolTipBase, brush7);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ToolTipText, brush6);
        QBrush brush10(QColor(0, 42, 0, 127));
        brush10.setStyle(Qt::BrushStyle::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::PlaceholderText, brush10);
#endif
        QBrush brush11(QColor(0, 60, 0, 255));
        brush11.setStyle(Qt::BrushStyle::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Accent, brush11);
#endif
        frame_6->setPalette(palette);
        frame_6->setStyleSheet(QString::fromUtf8("/* Couleur de fond du frame */\n"
"background-color: olive;\n"
"\n"
"/* Bordure fine et arrondie pour un effet moderne */\n"
"border: 1px solid #556B2F; \n"
"border-radius: 8px;\n"
"\n"
"/* Optionnel : ajoute un peu d'ombre pour plus de relief */\n"
"box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2);\n"
"\n"
"/* Style pour les labels et champs \303\240 l'int\303\251rieur */\n"
"label {\n"
"    font-family: Arial, sans-serif;\n"
"    font-size: 12pt;\n"
"    color: #fff; /* texte en blanc pour contraste */\n"
"}\n"
"\n"
"input, select, textarea {\n"
"    border-radius: 4px;\n"
"    border: 1px solid #ccc;\n"
"    padding: 3px 5px;\n"
"}\n"
""));
        frame_6->setFrameShape(QFrame::Shape::StyledPanel);
        frame_6->setFrameShadow(QFrame::Shadow::Raised);
        label_19 = new QLabel(frame_6);
        label_19->setObjectName("label_19");
        label_19->setGeometry(QRect(180, 0, 421, 101));
        label_11 = new QLabel(frame_6);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(0, 0, 721, 91));
        table1 = new QTableWidget(page_2);
        if (table1->columnCount() < 6)
            table1->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        table1->setHorizontalHeaderItem(0, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        table1->setHorizontalHeaderItem(1, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        table1->setHorizontalHeaderItem(2, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        table1->setHorizontalHeaderItem(3, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        table1->setHorizontalHeaderItem(4, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        table1->setHorizontalHeaderItem(5, __qtablewidgetitem18);
        table1->setObjectName("table1");
        table1->setGeometry(QRect(330, 250, 741, 291));
        recherche2 = new QLineEdit(page_2);
        recherche2->setObjectName("recherche2");
        recherche2->setGeometry(QRect(250, 140, 211, 42));
        label_22 = new QLabel(page_2);
        label_22->setObjectName("label_22");
        label_22->setGeometry(QRect(180, 150, 141, 20));
        recherche2_2 = new QPushButton(page_2);
        recherche2_2->setObjectName("recherche2_2");
        recherche2_2->setGeometry(QRect(470, 150, 93, 29));
        label_23 = new QLabel(page_2);
        label_23->setObjectName("label_23");
        label_23->setGeometry(QRect(790, 150, 63, 20));
        date_2 = new QDateEdit(page_2);
        date_2->setObjectName("date_2");
        date_2->setGeometry(QRect(820, 140, 161, 42));
        appliquer = new QPushButton(page_2);
        appliquer->setObjectName("appliquer");
        appliquer->setGeometry(QRect(990, 140, 131, 41));
        label_24 = new QLabel(page_2);
        label_24->setObjectName("label_24");
        label_24->setGeometry(QRect(180, 200, 63, 31));
        particulier_2 = new QRadioButton(page_2);
        particulier_2->setObjectName("particulier_2");
        particulier_2->setGeometry(QRect(250, 200, 110, 24));
        profisionnel_2 = new QRadioButton(page_2);
        profisionnel_2->setObjectName("profisionnel_2");
        profisionnel_2->setGeometry(QRect(360, 200, 131, 24));
        label_25 = new QLabel(page_2);
        label_25->setObjectName("label_25");
        label_25->setGeometry(QRect(770, 200, 241, 31));
        vierge = new QCheckBox(page_2);
        vierge->setObjectName("vierge");
        vierge->setGeometry(QRect(940, 200, 91, 24));
        bio = new QCheckBox(page_2);
        bio->setObjectName("bio");
        bio->setGeometry(QRect(1030, 200, 91, 24));
        table2 = new QTableWidget(page_2);
        if (table2->columnCount() < 3)
            table2->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        table2->setHorizontalHeaderItem(0, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        table2->setHorizontalHeaderItem(1, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        table2->setHorizontalHeaderItem(2, __qtablewidgetitem21);
        table2->setObjectName("table2");
        table2->setGeometry(QRect(250, 560, 391, 192));
        pushButton = new QPushButton(page_2);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(230, 820, 191, 29));
        classement = new QPushButton(page_2);
        classement->setObjectName("classement");
        classement->setGeometry(QRect(1250, 220, 191, 71));
        historique = new QPushButton(page_2);
        historique->setObjectName("historique");
        historique->setGeometry(QRect(1250, 150, 191, 71));
        stacked->addWidget(page_2);
        page_4 = new QWidget();
        page_4->setObjectName("page_4");
        frame_9 = new QFrame(page_4);
        frame_9->setObjectName("frame_9");
        frame_9->setGeometry(QRect(0, 0, 171, 851));
        frame_9->setStyleSheet(QString::fromUtf8("/* Couleur de fond du frame */\n"
"background-color: olive;\n"
"\n"
"/* Bordure fine et arrondie pour un effet moderne */\n"
"border: 1px solid #556B2F; \n"
"border-radius: 8px;\n"
"\n"
"/* Optionnel : ajoute un peu d'ombre pour plus de relief */\n"
"box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2);\n"
"\n"
"/* Style pour les labels et champs \303\240 l'int\303\251rieur */\n"
"label {\n"
"    font-family: Arial, sans-serif;\n"
"    font-size: 12pt;\n"
"    color: #fff; /* texte en blanc pour contraste */\n"
"}\n"
"\n"
"input, select, textarea {\n"
"    border-radius: 4px;\n"
"    border: 1px solid #ccc;\n"
"    padding: 3px 5px;\n"
"}\n"
"/* Couleur de fond du frame */\n"
"background-color: olive;\n"
"\n"
"/* Bordure fine et arrondie pour un effet moderne */\n"
"border: 1px solid #556B2F; \n"
"border-radius: 8px;\n"
"\n"
"/* Optionnel : ajoute un peu d'ombre pour plus de relief */\n"
"box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2);\n"
"\n"
"/* Style pour les labels et champs \303\240 l'int\303\251rieur */\n"
"label {\n"
""
                        "    font-family: Arial, sans-serif;\n"
"    font-size: 12pt;\n"
"    color: #fff; /* texte en blanc pour contraste */\n"
"}\n"
"\n"
"input, select, textarea {\n"
"    border-radius: 4px;\n"
"    border: 1px solid #ccc;\n"
"    padding: 3px 5px;\n"
"}\n"
""));
        frame_9->setFrameShape(QFrame::Shape::StyledPanel);
        frame_9->setFrameShadow(QFrame::Shadow::Raised);
        agriculteurs_4 = new QPushButton(frame_9);
        agriculteurs_4->setObjectName("agriculteurs_4");
        agriculteurs_4->setGeometry(QRect(0, 300, 181, 42));
        ventes_4 = new QPushButton(frame_9);
        ventes_4->setObjectName("ventes_4");
        ventes_4->setGeometry(QRect(0, 340, 181, 42));
        production_4 = new QPushButton(frame_9);
        production_4->setObjectName("production_4");
        production_4->setGeometry(QRect(-10, 380, 181, 42));
        employes_4 = new QPushButton(frame_9);
        employes_4->setObjectName("employes_4");
        employes_4->setGeometry(QRect(-10, 260, 181, 42));
        pushButton_5 = new QPushButton(frame_9);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(0, 120, 181, 42));
        parametre_4 = new QPushButton(frame_9);
        parametre_4->setObjectName("parametre_4");
        parametre_4->setGeometry(QRect(-10, 690, 181, 42));
        deconnexion_4 = new QPushButton(frame_9);
        deconnexion_4->setObjectName("deconnexion_4");
        deconnexion_4->setGeometry(QRect(0, 780, 181, 41));
        aide_4 = new QPushButton(frame_9);
        aide_4->setObjectName("aide_4");
        aide_4->setGeometry(QRect(0, 730, 181, 51));
        label_30 = new QLabel(frame_9);
        label_30->setObjectName("label_30");
        label_30->setGeometry(QRect(0, -20, 181, 141));
        label_30->setStyleSheet(QString::fromUtf8("QLabel#label_logo {\n"
"    background-color: transparent;\n"
"    padding: 10px;\n"
"}\n"
"\n"
"/* Option : zone du header o\303\271 se trouve le logo */\n"
"QFrame#header {\n"
"    background-color: white;\n"
"    border-bottom: 1px solid #e0e0e0;\n"
"}\n"
"\n"
"/* Style du titre \303\240 c\303\264t\303\251 du logo */\n"
"QLabel#appTitle {\n"
"    color: #556B2F;      /* vert olive */\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    font-family: Poppins;\n"
"}\n"
""));
        label_30->setPixmap(QPixmap(QString::fromUtf8("../../../Downloads/golden-removebg-preview.png")));
        label_30->setScaledContents(true);
        frame_10 = new QFrame(page_4);
        frame_10->setObjectName("frame_10");
        frame_10->setGeometry(QRect(170, 0, 1671, 80));
        frame_10->setStyleSheet(QString::fromUtf8("/* Couleur de fond du frame */\n"
"background-color: olive;\n"
"\n"
"/* Bordure fine et arrondie pour un effet moderne */\n"
"border: 1px solid #556B2F; \n"
"border-radius: 8px;\n"
"\n"
"/* Optionnel : ajoute un peu d'ombre pour plus de relief */\n"
"box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2);\n"
"\n"
"/* Style pour les labels et champs \303\240 l'int\303\251rieur */\n"
"label {\n"
"    font-family: Arial, sans-serif;\n"
"    font-size: 12pt;\n"
"    color: #fff; /* texte en blanc pour contraste */\n"
"}\n"
"\n"
"input, select, textarea {\n"
"    border-radius: 4px;\n"
"    border: 1px solid #ccc;\n"
"    padding: 3px 5px;\n"
"}\n"
"/* Couleur de fond du frame */\n"
"background-color: olive;\n"
"\n"
"/* Bordure fine et arrondie pour un effet moderne */\n"
"border: 1px solid #556B2F; \n"
"border-radius: 8px;\n"
"\n"
"/* Optionnel : ajoute un peu d'ombre pour plus de relief */\n"
"box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2);\n"
"\n"
"/* Style pour les labels et champs \303\240 l'int\303\251rieur */\n"
"label {\n"
""
                        "    font-family: Arial, sans-serif;\n"
"    font-size: 12pt;\n"
"    color: #fff; /* texte en blanc pour contraste */\n"
"}\n"
"\n"
"input, select, textarea {\n"
"    border-radius: 4px;\n"
"    border: 1px solid #ccc;\n"
"    padding: 3px 5px;\n"
"}"));
        frame_10->setFrameShape(QFrame::Shape::StyledPanel);
        frame_10->setFrameShadow(QFrame::Shadow::Raised);
        label_31 = new QLabel(frame_10);
        label_31->setObjectName("label_31");
        label_31->setGeometry(QRect(0, 0, 1171, 81));
        label_31->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";"));
        table3 = new QTableWidget(page_4);
        if (table3->columnCount() < 4)
            table3->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        table3->setHorizontalHeaderItem(0, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        table3->setHorizontalHeaderItem(1, __qtablewidgetitem23);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        table3->setHorizontalHeaderItem(2, __qtablewidgetitem24);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        table3->setHorizontalHeaderItem(3, __qtablewidgetitem25);
        table3->setObjectName("table3");
        table3->setGeometry(QRect(480, 130, 501, 591));
        exporter1 = new QPushButton(page_4);
        exporter1->setObjectName("exporter1");
        exporter1->setGeometry(QRect(420, 740, 271, 31));
        stacked->addWidget(page_4);
        page_5 = new QWidget();
        page_5->setObjectName("page_5");
        frame_11 = new QFrame(page_5);
        frame_11->setObjectName("frame_11");
        frame_11->setGeometry(QRect(0, 0, 171, 851));
        frame_11->setStyleSheet(QString::fromUtf8("/* Couleur de fond du frame */\n"
"background-color: olive;\n"
"\n"
"/* Bordure fine et arrondie pour un effet moderne */\n"
"border: 1px solid #556B2F; \n"
"border-radius: 8px;\n"
"\n"
"/* Optionnel : ajoute un peu d'ombre pour plus de relief */\n"
"box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2);\n"
"\n"
"/* Style pour les labels et champs \303\240 l'int\303\251rieur */\n"
"label {\n"
"    font-family: Arial, sans-serif;\n"
"    font-size: 12pt;\n"
"    color: #fff; /* texte en blanc pour contraste */\n"
"}\n"
"\n"
"input, select, textarea {\n"
"    border-radius: 4px;\n"
"    border: 1px solid #ccc;\n"
"    padding: 3px 5px;\n"
"}\n"
"/* Couleur de fond du frame */\n"
"background-color: olive;\n"
"\n"
"/* Bordure fine et arrondie pour un effet moderne */\n"
"border: 1px solid #556B2F; \n"
"border-radius: 8px;\n"
"\n"
"/* Optionnel : ajoute un peu d'ombre pour plus de relief */\n"
"box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2);\n"
"\n"
"/* Style pour les labels et champs \303\240 l'int\303\251rieur */\n"
"label {\n"
""
                        "    font-family: Arial, sans-serif;\n"
"    font-size: 12pt;\n"
"    color: #fff; /* texte en blanc pour contraste */\n"
"}\n"
"\n"
"input, select, textarea {\n"
"    border-radius: 4px;\n"
"    border: 1px solid #ccc;\n"
"    padding: 3px 5px;\n"
"}\n"
""));
        frame_11->setFrameShape(QFrame::Shape::StyledPanel);
        frame_11->setFrameShadow(QFrame::Shadow::Raised);
        agriculteurs_5 = new QPushButton(frame_11);
        agriculteurs_5->setObjectName("agriculteurs_5");
        agriculteurs_5->setGeometry(QRect(0, 300, 181, 42));
        ventes_5 = new QPushButton(frame_11);
        ventes_5->setObjectName("ventes_5");
        ventes_5->setGeometry(QRect(0, 340, 181, 42));
        production_5 = new QPushButton(frame_11);
        production_5->setObjectName("production_5");
        production_5->setGeometry(QRect(-10, 380, 181, 42));
        employes_5 = new QPushButton(frame_11);
        employes_5->setObjectName("employes_5");
        employes_5->setGeometry(QRect(-10, 260, 181, 42));
        pushButton_6 = new QPushButton(frame_11);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(-10, 140, 181, 42));
        parametre_5 = new QPushButton(frame_11);
        parametre_5->setObjectName("parametre_5");
        parametre_5->setGeometry(QRect(-10, 690, 181, 42));
        deconnexion_5 = new QPushButton(frame_11);
        deconnexion_5->setObjectName("deconnexion_5");
        deconnexion_5->setGeometry(QRect(0, 780, 181, 41));
        aide_5 = new QPushButton(frame_11);
        aide_5->setObjectName("aide_5");
        aide_5->setGeometry(QRect(0, 730, 181, 51));
        label_32 = new QLabel(frame_11);
        label_32->setObjectName("label_32");
        label_32->setGeometry(QRect(0, 0, 181, 141));
        label_32->setStyleSheet(QString::fromUtf8("QLabel#label_logo {\n"
"    background-color: transparent;\n"
"    padding: 10px;\n"
"}\n"
"\n"
"/* Option : zone du header o\303\271 se trouve le logo */\n"
"QFrame#header {\n"
"    background-color: white;\n"
"    border-bottom: 1px solid #e0e0e0;\n"
"}\n"
"\n"
"/* Style du titre \303\240 c\303\264t\303\251 du logo */\n"
"QLabel#appTitle {\n"
"    color: #556B2F;      /* vert olive */\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    font-family: Poppins;\n"
"}\n"
""));
        label_32->setPixmap(QPixmap(QString::fromUtf8("../../../Downloads/golden-removebg-preview.png")));
        label_32->setScaledContents(true);
        table4 = new QTableWidget(page_5);
        if (table4->columnCount() < 4)
            table4->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
        table4->setHorizontalHeaderItem(0, __qtablewidgetitem26);
        QTableWidgetItem *__qtablewidgetitem27 = new QTableWidgetItem();
        table4->setHorizontalHeaderItem(1, __qtablewidgetitem27);
        QTableWidgetItem *__qtablewidgetitem28 = new QTableWidgetItem();
        table4->setHorizontalHeaderItem(2, __qtablewidgetitem28);
        QTableWidgetItem *__qtablewidgetitem29 = new QTableWidgetItem();
        table4->setHorizontalHeaderItem(3, __qtablewidgetitem29);
        table4->setObjectName("table4");
        table4->setGeometry(QRect(260, 290, 501, 491));
        table5 = new QTableWidget(page_5);
        if (table5->columnCount() < 3)
            table5->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem30 = new QTableWidgetItem();
        table5->setHorizontalHeaderItem(0, __qtablewidgetitem30);
        QTableWidgetItem *__qtablewidgetitem31 = new QTableWidgetItem();
        table5->setHorizontalHeaderItem(1, __qtablewidgetitem31);
        QTableWidgetItem *__qtablewidgetitem32 = new QTableWidgetItem();
        table5->setHorizontalHeaderItem(2, __qtablewidgetitem32);
        table5->setObjectName("table5");
        table5->setGeometry(QRect(970, 290, 401, 491));
        label_33 = new QLabel(page_5);
        label_33->setObjectName("label_33");
        label_33->setGeometry(QRect(170, 90, 241, 20));
        label_33->setStyleSheet(QString::fromUtf8("font: 14pt \"Segoe UI\";"));
        label_34 = new QLabel(page_5);
        label_34->setObjectName("label_34");
        label_34->setGeometry(QRect(1020, 100, 251, 31));
        label_34->setStyleSheet(QString::fromUtf8("font: 14pt \"Segoe UI\";"));
        frame_12 = new QFrame(page_5);
        frame_12->setObjectName("frame_12");
        frame_12->setGeometry(QRect(170, 0, 1651, 80));
        frame_12->setStyleSheet(QString::fromUtf8("/* Couleur de fond du frame */\n"
"background-color: olive;\n"
"\n"
"/* Bordure fine et arrondie pour un effet moderne */\n"
"border: 1px solid #556B2F; \n"
"border-radius: 8px;\n"
"\n"
"/* Optionnel : ajoute un peu d'ombre pour plus de relief */\n"
"box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2);\n"
"\n"
"/* Style pour les labels et champs \303\240 l'int\303\251rieur */\n"
"label {\n"
"    font-family: Arial, sans-serif;\n"
"    font-size: 12pt;\n"
"    color: #fff; /* texte en blanc pour contraste */\n"
"}\n"
"\n"
"input, select, textarea {\n"
"    border-radius: 4px;\n"
"    border: 1px solid #ccc;\n"
"    padding: 3px 5px;\n"
"}\n"
"/* Couleur de fond du frame */\n"
"background-color: olive;\n"
"\n"
"/* Bordure fine et arrondie pour un effet moderne */\n"
"border: 1px solid #556B2F; \n"
"border-radius: 8px;\n"
"\n"
"/* Optionnel : ajoute un peu d'ombre pour plus de relief */\n"
"box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2);\n"
"\n"
"/* Style pour les labels et champs \303\240 l'int\303\251rieur */\n"
"label {\n"
""
                        "    font-family: Arial, sans-serif;\n"
"    font-size: 12pt;\n"
"    color: #fff; /* texte en blanc pour contraste */\n"
"}\n"
"\n"
"input, select, textarea {\n"
"    border-radius: 4px;\n"
"    border: 1px solid #ccc;\n"
"    padding: 3px 5px;\n"
"}\n"
""));
        frame_12->setFrameShape(QFrame::Shape::StyledPanel);
        frame_12->setFrameShadow(QFrame::Shadow::Raised);
        label_35 = new QLabel(frame_12);
        label_35->setObjectName("label_35");
        label_35->setGeometry(QRect(0, 0, 1481, 81));
        label_35->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";"));
        label_36 = new QLabel(page_5);
        label_36->setObjectName("label_36");
        label_36->setGeometry(QRect(200, 150, 63, 20));
        particulier_3 = new QRadioButton(page_5);
        particulier_3->setObjectName("particulier_3");
        particulier_3->setGeometry(QRect(240, 150, 110, 24));
        profisionnel_4 = new QRadioButton(page_5);
        profisionnel_4->setObjectName("profisionnel_4");
        profisionnel_4->setGeometry(QRect(350, 150, 110, 24));
        label_37 = new QLabel(page_5);
        label_37->setObjectName("label_37");
        label_37->setGeometry(QRect(190, 200, 141, 20));
        points_2 = new QLCDNumber(page_5);
        points_2->setObjectName("points_2");
        points_2->setGeometry(QRect(340, 200, 64, 23));
        label_38 = new QLabel(page_5);
        label_38->setObjectName("label_38");
        label_38->setGeometry(QRect(410, 200, 63, 20));
        appliquer2 = new QPushButton(page_5);
        appliquer2->setObjectName("appliquer2");
        appliquer2->setGeometry(QRect(450, 190, 111, 29));
        label_39 = new QLabel(page_5);
        label_39->setObjectName("label_39");
        label_39->setGeometry(QRect(1010, 190, 63, 20));
        particulier_4 = new QRadioButton(page_5);
        particulier_4->setObjectName("particulier_4");
        particulier_4->setGeometry(QRect(1070, 190, 110, 24));
        profisionnel_5 = new QRadioButton(page_5);
        profisionnel_5->setObjectName("profisionnel_5");
        profisionnel_5->setGeometry(QRect(1180, 190, 110, 24));
        appliquer3 = new QPushButton(page_5);
        appliquer3->setObjectName("appliquer3");
        appliquer3->setGeometry(QRect(1300, 190, 111, 29));
        exporter2 = new QPushButton(page_5);
        exporter2->setObjectName("exporter2");
        exporter2->setGeometry(QRect(260, 790, 221, 29));
        stacked->addWidget(page_5);
        client->setCentralWidget(centralwidget);
        menubar = new QMenuBar(client);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1592, 26));
        client->setMenuBar(menubar);
        statusbar = new QStatusBar(client);
        statusbar->setObjectName("statusbar");
        client->setStatusBar(statusbar);

        retranslateUi(client);

        stacked->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(client);
    } // setupUi

    void retranslateUi(QMainWindow *client)
    {
        client->setWindowTitle(QCoreApplication::translate("client", "client", nullptr));
        agriculteurs_3->setText(QCoreApplication::translate("client", "\342\200\213\360\237\221\250\360\237\217\273\342\200\215\360\237\214\276\342\200\213 Agriculteurs", nullptr));
        ventes_3->setText(QCoreApplication::translate("client", "\360\237\221\250\342\200\215\360\237\222\273Ventes", nullptr));
        production_3->setText(QCoreApplication::translate("client", "\360\237\217\255 Production", nullptr));
        employes_3->setText(QCoreApplication::translate("client", "\360\237\221\224\342\200\213 Employ\303\251s", nullptr));
        pushButton_4->setText(QCoreApplication::translate("client", "\342\200\213\342\200\213\342\227\200\357\270\217\342\200\213 retour", nullptr));
        parametre_3->setText(QCoreApplication::translate("client", " \342\232\231\357\270\217param\303\251tre", nullptr));
        deconnexion_3->setText(QCoreApplication::translate("client", " \342\200\213\342\200\213\342\227\200deconnexion", nullptr));
        aide_3->setText(QCoreApplication::translate("client", "centre d' aide", nullptr));
        label_26->setText(QString());
        label_27->setText(QCoreApplication::translate("client", " \360\237\244\226Ai golden oil press chat", nullptr));
        label_29->setText(QCoreApplication::translate("client", " \n"
"Bonjour cher client\n"
"Par o\303\271 commencer ?", nullptr));
        envoyer->setText(QCoreApplication::translate("client", " envoyer", nullptr));
        label->setText(QCoreApplication::translate("client", "id", nullptr));
        label_3->setText(QCoreApplication::translate("client", " nom", nullptr));
        label_5->setText(QCoreApplication::translate("client", " telephone", nullptr));
        label_6->setText(QCoreApplication::translate("client", " email", nullptr));
        label_7->setText(QCoreApplication::translate("client", " adresse", nullptr));
        label_4->setText(QCoreApplication::translate("client", " prenom", nullptr));
        label_2->setText(QCoreApplication::translate("client", "type", nullptr));
        particulier->setText(QCoreApplication::translate("client", " particulier", nullptr));
        profisionnel->setText(QCoreApplication::translate("client", "profisionnel", nullptr));
        ajouter->setText(QCoreApplication::translate("client", "\342\236\225 ajouter", nullptr));
        label_8->setText(QCoreApplication::translate("client", " date d'inscription", nullptr));
        label_14->setText(QCoreApplication::translate("client", " type Huile Prefere", nullptr));
        checkBox->setText(QCoreApplication::translate("client", " vierge", nullptr));
        checkBox_3->setText(QCoreApplication::translate("client", " bio", nullptr));
        label_15->setText(QCoreApplication::translate("client", " emballage Prefere", nullptr));
        emballage_prefere->setItemText(0, QCoreApplication::translate("client", "5L", nullptr));
        emballage_prefere->setItemText(1, QCoreApplication::translate("client", " 3L", nullptr));
        emballage_prefere->setItemText(2, QCoreApplication::translate("client", "1L", nullptr));
        emballage_prefere->setItemText(3, QCoreApplication::translate("client", " autre", nullptr));

        label_9->setText(QCoreApplication::translate("client", "points de fid\303\251lit\303\251s", nullptr));
        label_16->setText(QCoreApplication::translate("client", " pts", nullptr));
        modifier->setText(QCoreApplication::translate("client", "\360\237\224\204 modifier", nullptr));
        annuler->setText(QCoreApplication::translate("client", "\342\235\214\342\200\213 annuler", nullptr));
        label_20->setText(QCoreApplication::translate("client", "totale achat", nullptr));
        label_21->setText(QCoreApplication::translate("client", " dt", nullptr));
        afficher->setText(QCoreApplication::translate("client", "\342\234\205 afficher", nullptr));
        agriculteurs->setText(QCoreApplication::translate("client", "\342\200\213\360\237\221\250\360\237\217\273\342\200\215\360\237\214\276\342\200\213 Agriculteurs", nullptr));
        ventes->setText(QCoreApplication::translate("client", "\360\237\221\250\342\200\215\360\237\222\273Ventes", nullptr));
        production->setText(QCoreApplication::translate("client", "\360\237\217\255 Production", nullptr));
        employes->setText(QCoreApplication::translate("client", "\360\237\221\224\342\200\213 Employ\303\251s", nullptr));
        parametre->setText(QCoreApplication::translate("client", " \342\232\231\357\270\217param\303\251tre", nullptr));
        deconnexion->setText(QCoreApplication::translate("client", " \342\200\213\342\200\213\342\227\200deconnexion", nullptr));
        aide->setText(QCoreApplication::translate("client", "centre d' aide", nullptr));
        label_12->setText(QString());
        pushButton_2->setText(QCoreApplication::translate("client", " tableau de Bord", nullptr));
        retour->setText(QCoreApplication::translate("client", "\342\200\213\342\200\213\342\227\200\357\270\217\342\200\213 retour", nullptr));
        QTableWidgetItem *___qtablewidgetitem = table->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("client", "id", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = table->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("client", "nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = table->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("client", "prenom", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = table->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("client", "telephone", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = table->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("client", "email", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = table->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("client", "adresse", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = table->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("client", "type", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = table->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("client", " date d'inscription", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = table->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("client", " type huile prefere", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = table->horizontalHeaderItem(9);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("client", " emballage prefere", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = table->horizontalHeaderItem(10);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("client", " points de fid\303\251lit\303\251s", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = table->horizontalHeaderItem(11);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("client", " totale d achat", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = table->horizontalHeaderItem(12);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("client", " points de fidelit\303\251s", nullptr));
        label_28->setText(QCoreApplication::translate("client", " id", nullptr));
        supprimer->setText(QCoreApplication::translate("client", " \360\237\227\221\357\270\217\342\200\213 supprimer", nullptr));
        annuler1->setText(QCoreApplication::translate("client", "\342\235\214\342\200\213 annuler", nullptr));
        recherche1->setText(QString());
        recherche1_2->setText(QCoreApplication::translate("client", "\360\237\224\215\342\200\213", nullptr));
        tri_2->setText(QCoreApplication::translate("client", " trier par", nullptr));
        analyse->setText(QCoreApplication::translate("client", " \360\237\223\210analyse", nullptr));
        chatbot->setText(QCoreApplication::translate("client", "\360\237\244\226 chatbot", nullptr));
        pdf->setText(QCoreApplication::translate("client", "pdf\360\237\223\204", nullptr));
        label_18->setText(QCoreApplication::translate("client", " gestion des clients", nullptr));
        production_2->setText(QCoreApplication::translate("client", "\360\237\217\255 Production", nullptr));
        employes_2->setText(QCoreApplication::translate("client", "\360\237\221\224\342\200\213 Employ\303\251s", nullptr));
        ventes_2->setText(QCoreApplication::translate("client", "\360\237\221\250\342\200\215\360\237\222\273Ventes", nullptr));
        agriculteurs_2->setText(QCoreApplication::translate("client", "\342\200\213\360\237\221\250\360\237\217\273\342\200\215\360\237\214\276\342\200\213 Agriculteurs", nullptr));
        deconnexion_2->setText(QCoreApplication::translate("client", " \342\200\213\342\200\213\342\227\200deconnexion", nullptr));
        pushButton_3->setText(QCoreApplication::translate("client", "\342\200\213\342\200\213\342\227\200\357\270\217\342\200\213 retour", nullptr));
        label_10->setText(QCoreApplication::translate("client", " gestion des clients", nullptr));
        parametre_2->setText(QCoreApplication::translate("client", " \342\232\231\357\270\217param\303\251tre", nullptr));
        aide_2->setText(QCoreApplication::translate("client", "centre d' aide", nullptr));
        label_17->setText(QString());
#if QT_CONFIG(tooltip)
        frame_6->setToolTip(QCoreApplication::translate("client", "<html><head/><body><p><span style=\" color:#005500;\">#005500</span></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        label_19->setText(QString());
        label_11->setText(QCoreApplication::translate("client", " \360\237\233\222analyse de comportement d'achat", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = table1->horizontalHeaderItem(0);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("client", " nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = table1->horizontalHeaderItem(1);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("client", "prenom", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = table1->horizontalHeaderItem(2);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("client", "totale d achat", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = table1->horizontalHeaderItem(3);
        ___qtablewidgetitem16->setText(QCoreApplication::translate("client", " points fid\303\251lit\303\251s", nullptr));
        QTableWidgetItem *___qtablewidgetitem17 = table1->horizontalHeaderItem(4);
        ___qtablewidgetitem17->setText(QCoreApplication::translate("client", " type de huile prefere", nullptr));
        QTableWidgetItem *___qtablewidgetitem18 = table1->horizontalHeaderItem(5);
        ___qtablewidgetitem18->setText(QCoreApplication::translate("client", "type", nullptr));
        label_22->setText(QCoreApplication::translate("client", " recherche", nullptr));
        recherche2_2->setText(QCoreApplication::translate("client", "\360\237\224\215\342\200\213", nullptr));
        label_23->setText(QCoreApplication::translate("client", " au", nullptr));
        appliquer->setText(QCoreApplication::translate("client", " appliquer", nullptr));
        label_24->setText(QCoreApplication::translate("client", " type", nullptr));
        particulier_2->setText(QCoreApplication::translate("client", " particulier", nullptr));
        profisionnel_2->setText(QCoreApplication::translate("client", " profisionnel", nullptr));
        label_25->setText(QCoreApplication::translate("client", " type de huile prefere", nullptr));
        vierge->setText(QCoreApplication::translate("client", " vierge", nullptr));
        bio->setText(QCoreApplication::translate("client", " bio", nullptr));
        QTableWidgetItem *___qtablewidgetitem19 = table2->horizontalHeaderItem(0);
        ___qtablewidgetitem19->setText(QCoreApplication::translate("client", " volume d'achat", nullptr));
        QTableWidgetItem *___qtablewidgetitem20 = table2->horizontalHeaderItem(1);
        ___qtablewidgetitem20->setText(QCoreApplication::translate("client", "achat cumul\303\251s dans le temps", nullptr));
        QTableWidgetItem *___qtablewidgetitem21 = table2->horizontalHeaderItem(2);
        ___qtablewidgetitem21->setText(QCoreApplication::translate("client", " type de huile prefere", nullptr));
        pushButton->setText(QCoreApplication::translate("client", " exporter comme pdf", nullptr));
        classement->setText(QCoreApplication::translate("client", " classement de client", nullptr));
        historique->setText(QCoreApplication::translate("client", " historique d'achat", nullptr));
        agriculteurs_4->setText(QCoreApplication::translate("client", "\342\200\213\360\237\221\250\360\237\217\273\342\200\215\360\237\214\276\342\200\213 Agriculteurs", nullptr));
        ventes_4->setText(QCoreApplication::translate("client", "\360\237\221\250\342\200\215\360\237\222\273Ventes", nullptr));
        production_4->setText(QCoreApplication::translate("client", "\360\237\217\255 Production", nullptr));
        employes_4->setText(QCoreApplication::translate("client", "\360\237\221\224\342\200\213 Employ\303\251s", nullptr));
        pushButton_5->setText(QCoreApplication::translate("client", "\342\200\213\342\200\213\342\227\200\357\270\217\342\200\213 retour", nullptr));
        parametre_4->setText(QCoreApplication::translate("client", " \342\232\231\357\270\217param\303\251tre", nullptr));
        deconnexion_4->setText(QCoreApplication::translate("client", " \342\200\213\342\200\213\342\227\200deconnexion", nullptr));
        aide_4->setText(QCoreApplication::translate("client", "centre d' aide", nullptr));
        label_30->setText(QString());
        label_31->setText(QCoreApplication::translate("client", " \360\237\223\234historique d'achat", nullptr));
        QTableWidgetItem *___qtablewidgetitem22 = table3->horizontalHeaderItem(0);
        ___qtablewidgetitem22->setText(QCoreApplication::translate("client", " date d'inscription", nullptr));
        QTableWidgetItem *___qtablewidgetitem23 = table3->horizontalHeaderItem(1);
        ___qtablewidgetitem23->setText(QCoreApplication::translate("client", " type de huile prefere", nullptr));
        QTableWidgetItem *___qtablewidgetitem24 = table3->horizontalHeaderItem(2);
        ___qtablewidgetitem24->setText(QCoreApplication::translate("client", " emballage prefere", nullptr));
        QTableWidgetItem *___qtablewidgetitem25 = table3->horizontalHeaderItem(3);
        ___qtablewidgetitem25->setText(QCoreApplication::translate("client", " achat totale", nullptr));
        exporter1->setText(QCoreApplication::translate("client", " expoter comme pdf\360\237\223\204", nullptr));
        agriculteurs_5->setText(QCoreApplication::translate("client", "\342\200\213\360\237\221\250\360\237\217\273\342\200\215\360\237\214\276\342\200\213 Agriculteurs", nullptr));
        ventes_5->setText(QCoreApplication::translate("client", "\360\237\221\250\342\200\215\360\237\222\273Ventes", nullptr));
        production_5->setText(QCoreApplication::translate("client", "\360\237\217\255 Production", nullptr));
        employes_5->setText(QCoreApplication::translate("client", "\360\237\221\224\342\200\213 Employ\303\251s", nullptr));
        pushButton_6->setText(QCoreApplication::translate("client", "\342\200\213\342\200\213\342\227\200\357\270\217\342\200\213 retour", nullptr));
        parametre_5->setText(QCoreApplication::translate("client", " \342\232\231\357\270\217param\303\251tre", nullptr));
        deconnexion_5->setText(QCoreApplication::translate("client", " \342\200\213\342\200\213\342\227\200deconnexion", nullptr));
        aide_5->setText(QCoreApplication::translate("client", "centre d' aide", nullptr));
        label_32->setText(QString());
        QTableWidgetItem *___qtablewidgetitem26 = table4->horizontalHeaderItem(0);
        ___qtablewidgetitem26->setText(QCoreApplication::translate("client", " id", nullptr));
        QTableWidgetItem *___qtablewidgetitem27 = table4->horizontalHeaderItem(1);
        ___qtablewidgetitem27->setText(QCoreApplication::translate("client", " nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem28 = table4->horizontalHeaderItem(2);
        ___qtablewidgetitem28->setText(QCoreApplication::translate("client", "prenom", nullptr));
        QTableWidgetItem *___qtablewidgetitem29 = table4->horizontalHeaderItem(3);
        ___qtablewidgetitem29->setText(QCoreApplication::translate("client", " points de fid\303\251lit\303\251s", nullptr));
        QTableWidgetItem *___qtablewidgetitem30 = table5->horizontalHeaderItem(0);
        ___qtablewidgetitem30->setText(QCoreApplication::translate("client", " id", nullptr));
        QTableWidgetItem *___qtablewidgetitem31 = table5->horizontalHeaderItem(1);
        ___qtablewidgetitem31->setText(QCoreApplication::translate("client", "nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem32 = table5->horizontalHeaderItem(2);
        ___qtablewidgetitem32->setText(QCoreApplication::translate("client", "prenom", nullptr));
        label_33->setText(QCoreApplication::translate("client", " \360\237\244\235\360\237\217\274les clients fid\303\251les", nullptr));
        label_34->setText(QCoreApplication::translate("client", " \360\237\224\235les meilleurs clients", nullptr));
        label_35->setText(QCoreApplication::translate("client", " \342\200\213\360\237\227\202\357\270\217\342\200\213classement des clients ", nullptr));
        label_36->setText(QCoreApplication::translate("client", " type", nullptr));
        particulier_3->setText(QCoreApplication::translate("client", " particulier", nullptr));
        profisionnel_4->setText(QCoreApplication::translate("client", "profisionnel", nullptr));
        label_37->setText(QCoreApplication::translate("client", " points de fid\303\251lit\303\251s", nullptr));
        label_38->setText(QCoreApplication::translate("client", "pts", nullptr));
        appliquer2->setText(QCoreApplication::translate("client", " \360\237\252\247appliquer", nullptr));
        label_39->setText(QCoreApplication::translate("client", " type", nullptr));
        particulier_4->setText(QCoreApplication::translate("client", " particulier", nullptr));
        profisionnel_5->setText(QCoreApplication::translate("client", "profisionnel", nullptr));
        appliquer3->setText(QCoreApplication::translate("client", "\360\237\252\247 appliquer", nullptr));
        exporter2->setText(QCoreApplication::translate("client", " expoter comme pdf\360\237\223\204", nullptr));
    } // retranslateUi

};

namespace Ui {
    class client: public Ui_client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENT_H
