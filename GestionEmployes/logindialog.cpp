#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    QAction *toggleAction = new QAction(this);
    toggleAction->setIcon(QIcon(":/images/eye.png"));
    ui->lineEdit_password->addAction(toggleAction, QLineEdit::TrailingPosition);

    connect(toggleAction, &QAction::triggered, this, [=]() {
        if(ui->lineEdit_password->echoMode() == QLineEdit::Password)
            ui->lineEdit_password->setEchoMode(QLineEdit::Normal);
        else
            ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    });

}

LoginDialog::~LoginDialog()
{
    delete ui;
}
