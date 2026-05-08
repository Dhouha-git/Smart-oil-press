#ifndef DIALOGEMAIL_H
#define DIALOGEMAIL_H

#include <QDialog>

namespace Ui {
class DialogEmail;
}

class DialogEmail : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEmail(QWidget *parent = nullptr);
    ~DialogEmail();
    QString getEmail();

private slots:
    void on_btn_envoyer_clicked();
    void on_btn_annuler_clicked();

private:
    Ui::DialogEmail *ui;
};

#endif
