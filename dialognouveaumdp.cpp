#include "dialognouveaumdp.h"
#include "ui_dialognouveaumdp.h"
#include <QMessageBox>

DialogNouveauMdp::DialogNouveauMdp(QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogNouveauMdp)
{
    ui->setupUi(this);
    setWindowTitle("Nouveau mot de passe");
    setModal(true);
}

DialogNouveauMdp::~DialogNouveauMdp()
{
    delete ui;
}

QString DialogNouveauMdp::getNouveauMdp()
{
    return ui->lineEdit_nouveau_mdp->text();
}

void DialogNouveauMdp::on_btn_changer_clicked()
{
    QString nouveau = ui->lineEdit_nouveau_mdp->text();
    QString confirm = ui->lineEdit_confirm_mdp->text();

    if (nouveau.length() < 6) {
        QMessageBox::warning(this, "Erreur",
                             "Minimum 6 caractères !"); return; }

    if (nouveau != confirm) {
        QMessageBox::warning(this, "Erreur",
                             "Les mots de passe ne correspondent pas !"); return; }

    accept();
}

void DialogNouveauMdp::on_btn_annuler_clicked()
{
    reject();
}
