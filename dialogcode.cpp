#include "dialogcode.h"
#include "ui_dialogcode.h"
#include <QMessageBox>

DialogCode::DialogCode(QString codeAttendu, QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogCode),
    codeAttendu(codeAttendu)
{
    ui->setupUi(this);
    setWindowTitle("Vérification du code");
    setModal(true);
}

DialogCode::~DialogCode()
{
    delete ui;
}

void DialogCode::on_btn_verifier_clicked()
{
    QString codeSaisi = ui->lineEdit_code->text().trimmed();

    if (codeSaisi.isEmpty()) {
        QMessageBox::warning(this, "Erreur",
                             "Entrez le code reçu !"); return; }

    if (codeSaisi != codeAttendu) {
        QMessageBox::warning(this, "Erreur",
                             "Code incorrect !"); return; }

    // ✅ Code correct
    accept();
}

void DialogCode::on_btn_annuler_clicked()
{
    reject();
}
