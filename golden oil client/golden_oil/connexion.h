#ifndef CONNEXION_H
#define CONNEXION_H

#include <QMainWindow>
#include <QObject>
#include <QQuickItem>
#include <QSharedDataPointer>
#include <QWidget>

class connexionData;

class connexion
{
    Q_OBJECT
    QML_ELEMENT
public:
    connexion();
    connexion(const connexion &);
    connexion(connexion &&);
    connexion &operator=(const connexion &);
    connexion &operator=(connexion &&);
    ~connexion();

private:
    QSharedDataPointer<connexionData> data;
};

#endif // CONNEXION_H
