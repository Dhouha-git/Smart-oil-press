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

    // ── Connexion ──────────────────────────────────────────────
    bool connecter(const QString &portName, qint32 baudRate = 9600);
    void deconnecter();
    bool estConnecte() const;
    QStringList listePortsDisponibles() const;

    // ── RFID Employés ──────────────────────────────────────────
    void envoyerDonnees(const QString &data);
    void envoyerReset();
    bool enregistrerPresence(const QString &cin);

signals:
    // ── RFID ───────────────────────────────────────────────────
    void carteDetectee(const QString &cin);
    void employeIdentifie(const QString &cin, const QString &nom, const QString &prenom);
    void employeInconnu(const QString &cin);
    void messageRecu(const QString &message);

    // ── Production / capteurs olives ───────────────────────────
    void greenCountChanged(int count);
    void blackCountChanged(int count);
    void formulaireRecu(QString idOperation, QString idAgriculteur, QString idMachine);

    // ── Statut connexion ───────────────────────────────────────
    void statutChanged(const QString &message, bool connecte);

private slots:
    void lireDonneesSerial();

private:
    QSerialPort *m_serial;
    QByteArray   m_buffer;

    // État formulaire production
    QString m_idOperation;
    QString m_idAgriculteur;
    QString m_idMachine;

    void traiterMessage(const QString &ligne);
    void traiterRFID(const QString &ligne);
    void traiterProduction(const QString &ligne);
};

#endif // ARDUINO_H
