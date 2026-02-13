#ifndef AJOUT_EMPLOYE_H
#define AJOUT_EMPLOYE_H

#include <QDialog>

namespace Ui {
class ajout_employe;
}

class ajout_employe : public QDialog
{
    Q_OBJECT

public:
    explicit ajout_employe(QWidget *parent = nullptr);
    ~ajout_employe();

private:
    Ui::ajout_employe *ui;
};

#endif // AJOUT_EMPLOYE_H
