// arduino.h
#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#ifndef NO_SERIALPORT
#include <QSerialPort>
#include <QSerialPortInfo>
#endif

class Arduino : public QObject
{
    Q_OBJECT

public:
    explicit Arduino(QObject *parent = nullptr);
    ~Arduino();

    bool connecter(const QString &portName, qint32 baudRate = 9600);
    void deconnecter();
    bool estConnecte() const;
    void envoyerDonnees(const QString &data);
    void envoyerReset();
    bool enregistrerPresence(const QString &cin);

signals:
    void carteDetectee(const QString &cin);
    void messageRecu(const QString &message);
    void employeIdentifie(const QString &cin, const QString &nom, const QString &prenom);
    void employeInconnu(const QString &cin);

private slots:
    void lireDonneesSerial();

private:
#ifndef NO_SERIALPORT
    QSerialPort *m_serial;
#endif
    QString m_buffer;
    void traiterMessage(const QString &message);
};

#endif // ARDUINO_H
