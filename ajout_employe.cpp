#include "ajout_employe.h"
#include "ui_ajout_employe.h"

ajout_employe::ajout_employe(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ajout_employe)
{
    ui->setupUi(this);
}

ajout_employe::~ajout_employe()
{
    delete ui;
}
