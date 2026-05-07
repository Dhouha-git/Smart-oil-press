#include "dialogemail.h"
#include "ui_dialogemail.h"
#include <QMessageBox>
#include <QSqlQuery>

DialogEmail::DialogEmail(QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogEmail)
{
    ui->setupUi(this);
    setWindowTitle("Mot de passe oublié");
    // ✅ Popup au milieu
    setModal(true);
}

DialogEmail::~DialogEmail()
{
    delete ui;
}

QString DialogEmail::getEmail()
{
    return ui->lineEdit_email->text().trimmed();
}

void DialogEmail::on_btn_envoyer_clicked()
{
    QString email = ui->lineEdit_email->text().trimmed();

    if (email.isEmpty()) {
        QMessageBox::warning(this, "Erreur",
                             "Entrez votre email !"); return; }

    // Vérifier email dans BD
    QSqlQuery query;
    query.prepare(
        "SELECT ID_EMP FROM SMART.EMPLOYE "
        "WHERE TRIM(EMAIL) = :email"
        );
    query.bindValue(":email", email);
    query.exec();

    if (!query.next()) {
        QMessageBox::warning(this, "Erreur",
                             "Aucun compte avec cet email !"); return; }

    // ✅ Email trouvé → fermer ce dialog avec succès
    accept();
}

void DialogEmail::on_btn_annuler_clicked()
{
    reject();
}
