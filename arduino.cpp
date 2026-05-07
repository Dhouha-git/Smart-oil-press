// arduino.cpp
#include "arduino.h"
#include "connection.h"

Arduino::Arduino(QObject *parent)
    : QObject(parent)
#ifndef NO_SERIALPORT
    , m_serial(new QSerialPort(this))
#endif
{
#ifndef NO_SERIALPORT
    connect(m_serial, &QSerialPort::readyRead,
            this,     &Arduino::lireDonneesSerial);
#endif
}

Arduino::~Arduino()
{
    deconnecter();
}

// ─── Connexion au port série ──────────────────────────────────────────────────
bool Arduino::connecter(const QString &portName, qint32 baudRate)
{
#ifndef NO_SERIALPORT
    if (m_serial->isOpen())
        m_serial->close();

    m_serial->setPortName(portName);
    m_serial->setBaudRate(baudRate);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);

    if (!m_serial->open(QIODevice::ReadWrite)) {
        qDebug() << "[ARDUINO] Erreur ouverture port:" << m_serial->errorString();
        return false;
    }

    qDebug() << "[ARDUINO] Connecté sur" << portName << "à" << baudRate << "baud";
    return true;
#else
    Q_UNUSED(portName) Q_UNUSED(baudRate)
    qDebug() << "[ARDUINO] Module SerialPort non disponible";
    return false;
#endif
}

void Arduino::deconnecter()
{
#ifndef NO_SERIALPORT
    if (m_serial->isOpen())
        m_serial->close();
#endif
}

bool Arduino::estConnecte() const
{
#ifndef NO_SERIALPORT
    return m_serial->isOpen();
#else
    return false;
#endif
}

// ─── Envoi de données vers l'Arduino ─────────────────────────────────────────
void Arduino::envoyerDonnees(const QString &data)
{
#ifndef NO_SERIALPORT
    if (!m_serial->isOpen()) return;
    QString cmd = data.trimmed() + "\n";
    m_serial->write(cmd.toUtf8());
    qDebug() << "[ARDUINO] Envoyé:" << cmd.trimmed();
#else
    Q_UNUSED(data)
#endif
}

void Arduino::envoyerReset()
{
    envoyerDonnees("RESET");
}


// ─── Interprétation du message reçu ──────────────────────────────────────────
void Arduino::traiterMessage(const QString &message)
{
    emit messageRecu(message);  // toujours logger

    if (message.startsWith("Card data:")) {
        QString cin = message.mid(QString("Card data:").length()).trimmed();
        if (cin.isEmpty()) return;

        emit carteDetectee(cin);

        QSqlQuery query;
        query.prepare(
            "SELECT NOM, PRENOM FROM SMART.EMPLOYE "
            "WHERE TRIM(CIN) = TRIM(:cin)"
            );
        query.bindValue(":cin", cin);

        if (!query.exec()) {
            qDebug() << "[ARDUINO] Erreur SQL:" << query.lastError().text();
            return;
        }

        if (query.next()) {
            QString nom    = query.value(0).toString();
            QString prenom = query.value(1).toString();
            enregistrerPresence(cin);
            emit employeIdentifie(cin, nom, prenom);
        } else {
            emit employeInconnu(cin);  // CIN existe mais pas en BD
        }

    } else if (message.startsWith("No data on tag")) {
        // ← carte blanche → popup accès refusé
        emit employeInconnu("Carte non enregistrée");
    }
}

void Arduino::lireDonneesSerial()
{
#ifndef NO_SERIALPORT
    m_buffer += QString::fromUtf8(m_serial->readAll());

    while (m_buffer.contains('\n')) {
        int idx       = m_buffer.indexOf('\n');
        QString ligne = m_buffer.left(idx);
        m_buffer      = m_buffer.mid(idx + 1);

        ligne.remove(QChar('\0'));
        ligne = ligne.trimmed();

        if (!ligne.isEmpty()) {
            emit messageRecu(ligne);
            traiterMessage(ligne);
        }
    }
#endif
}

// ─── Enregistrement en BD ─────────────────────────────────────────────────────
bool Arduino::enregistrerPresence(const QString &cin)
{
    QSqlQuery query;

    // Vérifier si HEURE_ENTREE existe et HEURE_SORTIE est NULL
    query.prepare(
        "SELECT HEURE_ENTREE, HEURE_SORTIE FROM SMART.EMPLOYE "
        "WHERE TRIM(CIN) = TRIM(:cin)"
        );
    query.bindValue(":cin", cin);

    if (!query.exec() || !query.next()) {
        qDebug() << "[ARDUINO] Employé introuvable";
        return false;
    }

    QString heureEntree = query.value(0).toString();
    QString heureSortie = query.value(1).toString();

    QSqlQuery updateQuery;

    if (heureEntree.isEmpty()) {
        // ── Pas encore entré → ENTREE ──
        updateQuery.prepare(
            "UPDATE SMART.EMPLOYE "
            "SET HEURE_ENTREE = TO_CHAR(SYSDATE, 'HH24:MI'), "
            "    HEURE_SORTIE = NULL "
            "WHERE TRIM(CIN) = TRIM(:cin)"
            );
        updateQuery.bindValue(":cin", cin);
        updateQuery.exec();
        emit messageRecu("ENTREE enregistrée : " + cin);
        qDebug() << "[ARDUINO] Entrée enregistrée pour CIN:" << cin;

    } else if (heureSortie.isEmpty()) {
        // ── Déjà entré, pas encore sorti → SORTIE ──
        updateQuery.prepare(
            "UPDATE SMART.EMPLOYE "
            "SET HEURE_SORTIE = TO_CHAR(SYSDATE, 'HH24:MI') "
            "WHERE TRIM(CIN) = TRIM(:cin)"
            );
        updateQuery.bindValue(":cin", cin);
        updateQuery.exec();
        emit messageRecu("SORTIE enregistrée : " + cin);
        qDebug() << "[ARDUINO] Sortie enregistrée pour CIN:" << cin;

    } else {
        // ── Les deux existent → reset pour nouveau jour ──
        updateQuery.prepare(
            "UPDATE SMART.EMPLOYE "
            "SET HEURE_ENTREE = TO_CHAR(SYSDATE, 'HH24:MI'), "
            "    HEURE_SORTIE = NULL "
            "WHERE TRIM(CIN) = TRIM(:cin)"
            );
        updateQuery.bindValue(":cin", cin);
        updateQuery.exec();
        emit messageRecu("ENTREE enregistrée : " + cin);
        qDebug() << "[ARDUINO] Nouveau jour — Entrée pour CIN:" << cin;
    }

    return true;
}
