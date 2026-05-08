// arduino.h
#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class Arduino : public QObject
{
    Q_OBJECT

public:
    explicit Arduino(QObject *parent = nullptr);
    ~Arduino();

    // Connexion au port série (ex: "COM3" sous Windows, "/dev/ttyUSB0" sous Linux)
    bool connecter(const QString &portName, qint32 baudRate = 9600);
    void deconnecter();
    bool estConnecte() const;

    // Envoyer une commande à l'Arduino
    // Ex: envoyer le CIN d'un employé à écrire sur la carte
    void envoyerDonnees(const QString &data);

    // Envoyer "RESET" pour effacer une carte
    void envoyerReset();

    // Enregistrer la présence d'un employé dans la BD
    // (appelé automatiquement quand une carte valide est scannée)
     bool enregistrerPresence(const QString &cin);

signals:
    // Émis quand une carte valide est lue (avec le CIN)
    void carteDetectee(const QString &cin);

    // Émis pour afficher les logs dans l'UI
    void messageRecu(const QString &message);

    // Émis quand l'employé est reconnu (après vérification BD)
    void employeIdentifie(const QString &cin, const QString &nom, const QString &prenom);

    // Émis si le CIN n'existe pas en BD
    void employeInconnu(const QString &cin);

private slots:
    void lireDonneesSerial();

private:
    QSerialPort *m_serial;
    QString      m_buffer;

    void traiterMessage(const QString &message);
};

#endif // ARDUINO_H
