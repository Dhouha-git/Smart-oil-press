#include "arduino.h"
#include "connection.h"

Arduino::Arduino(QObject *parent)
    : QObject(parent)
    , m_serial(new QSerialPort(this))
{
    connect(m_serial, &QSerialPort::readyRead,
            this,     &Arduino::lireDonneesSerial);
}

Arduino::~Arduino()
{
    deconnecter();
}

// ════════════════════════════════════════════════════════
// CONNEXION
// ════════════════════════════════════════════════════════
bool Arduino::connecter(const QString &portName, qint32 baudRate)
{
    if (m_serial->isOpen())
        m_serial->close();

    m_serial->setPortName(portName);
    m_serial->setBaudRate(baudRate);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);

    if (!m_serial->open(QIODevice::ReadWrite)) {
        emit statutChanged("● Erreur : " + m_serial->errorString(), false);
        qDebug() << "[ARDUINO] Erreur ouverture:" << m_serial->errorString();
        return false;
    }

    emit statutChanged("● Connecté sur " + portName, true);
    emit messageRecu("Connecté sur " + portName);
    qDebug() << "[ARDUINO] Connecté sur" << portName;
    return true;
}

void Arduino::deconnecter()
{
    if (m_serial && m_serial->isOpen()) {
        m_serial->close();
        emit statutChanged("● Déconnecté", false);
        emit messageRecu("Arduino déconnecté.");
    }
}

bool Arduino::estConnecte() const
{
    return m_serial && m_serial->isOpen();
}

QStringList Arduino::listePortsDisponibles() const
{
    QStringList liste;
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts())
        liste << info.portName();
    return liste;
}

// ════════════════════════════════════════════════════════
// ENVOI (RFID)
// ════════════════════════════════════════════════════════
void Arduino::envoyerDonnees(const QString &data)
{
    if (!m_serial->isOpen()) return;
    m_serial->write((data.trimmed() + "\n").toUtf8());
    qDebug() << "[ARDUINO] Envoyé:" << data.trimmed();
}

void Arduino::envoyerReset()
{
    envoyerDonnees("RESET");
}

// ════════════════════════════════════════════════════════
// LECTURE SÉRIE — slot unique
// ════════════════════════════════════════════════════════
void Arduino::lireDonneesSerial()
{
    m_buffer += m_serial->readAll();

    while (m_buffer.contains('\n')) {
        int idx = m_buffer.indexOf('\n');
        QString ligne = QString::fromUtf8(m_buffer.left(idx));
        m_buffer = m_buffer.mid(idx + 1);

        ligne.remove(QChar('\0'));
        ligne = ligne.trimmed();

        if (!ligne.isEmpty()) {
            emit messageRecu(ligne);
            traiterMessage(ligne);
        }
    }
}

// ════════════════════════════════════════════════════════
// DISPATCH — détecte RFID ou Production
// ════════════════════════════════════════════════════════
void Arduino::traiterMessage(const QString &ligne)
{
    // ── Production ──────────────────────────────────────
    if (ligne.startsWith("ID operation:") ||
        ligne.startsWith("Green count:")  ||
        ligne.startsWith("Black count:")  ||
        ligne.startsWith("================"))
    {
        traiterProduction(ligne);
    }
    // ── RFID ────────────────────────────────────────────
    else if (ligne.startsWith("Card data:") ||
             ligne.startsWith("No data on tag"))
    {
        traiterRFID(ligne);
    }
    else {
        qDebug() << "[ARDUINO]" << ligne;
    }
}

// ════════════════════════════════════════════════════════
// SCÉNARIO RFID — employés
// ════════════════════════════════════════════════════════
void Arduino::traiterRFID(const QString &ligne)
{
    if (ligne.startsWith("Card data:")) {
        QString cin = ligne.mid(QString("Card data:").length()).trimmed();
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
            emit employeInconnu(cin);
        }
    }
    else if (ligne.startsWith("No data on tag")) {
        emit employeInconnu("Carte non enregistrée");
    }
}

bool Arduino::enregistrerPresence(const QString &cin)
{
    QSqlQuery query;
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

    QSqlQuery upd;

    if (heureEntree.isEmpty()) {
        // Première scan → ENTRÉE
        upd.prepare(
            "UPDATE SMART.EMPLOYE "
            "SET HEURE_ENTREE = TO_CHAR(SYSDATE,'HH24:MI'), HEURE_SORTIE = NULL "
            "WHERE TRIM(CIN) = TRIM(:cin)"
            );
        emit messageRecu("ENTRÉE enregistrée : " + cin);

    } else if (heureSortie.isEmpty()) {
        // Deuxième scan → SORTIE
        upd.prepare(
            "UPDATE SMART.EMPLOYE "
            "SET HEURE_SORTIE = TO_CHAR(SYSDATE,'HH24:MI') "
            "WHERE TRIM(CIN) = TRIM(:cin)"
            );
        emit messageRecu("SORTIE enregistrée : " + cin);

    } else {
        // Les deux existent → reset nouveau jour
        upd.prepare(
            "UPDATE SMART.EMPLOYE "
            "SET HEURE_ENTREE = TO_CHAR(SYSDATE,'HH24:MI'), HEURE_SORTIE = NULL "
            "WHERE TRIM(CIN) = TRIM(:cin)"
            );
        emit messageRecu("ENTRÉE (nouveau jour) : " + cin);
    }

    upd.bindValue(":cin", cin);
    return upd.exec();
}

// ════════════════════════════════════════════════════════
// SCÉNARIO PRODUCTION — capteurs olives
// ════════════════════════════════════════════════════════
void Arduino::traiterProduction(const QString &ligne)
{
    if (ligne.startsWith("ID operation:")) {
        m_idOperation = ligne.mid(ligne.indexOf(':') + 1).trimmed();
        qDebug() << "[ARDUINO] ID Opération reçu:" << m_idOperation;
    }
    else if (ligne.startsWith("Green count:")) {
        int count = ligne.mid(ligne.indexOf(':') + 1).trimmed().toInt();
        emit greenCountChanged(count);
        qDebug() << "[ARDUINO] Green:" << count;
    }
    else if (ligne.startsWith("Black count:")) {
        int count = ligne.mid(ligne.indexOf(':') + 1).trimmed().toInt();
        emit blackCountChanged(count);
        qDebug() << "[ARDUINO] Black:" << count;
    }
    else if (ligne.startsWith("================")) {
        // Fin de trame — émettre formulaire complet
        if (!m_idOperation.isEmpty()) {
            emit formulaireRecu(m_idOperation,
                                m_idAgriculteur.isEmpty() ? "0" : m_idAgriculteur,
                                m_idMachine.isEmpty()     ? "0" : m_idMachine);
            // Reset pour prochain formulaire
            m_idOperation.clear();
            m_idAgriculteur.clear();
            m_idMachine.clear();
        }
    }
}
