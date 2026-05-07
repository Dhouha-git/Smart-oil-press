#ifndef DIALOGCODE_H
#define DIALOGCODE_H

#include <QDialog>

namespace Ui {
class DialogCode;
}

class DialogCode : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCode(QString codeAttendu,
                        QWidget *parent = nullptr);
    ~DialogCode();

private slots:
    void on_btn_verifier_clicked();
    void on_btn_annuler_clicked();

private:
    Ui::DialogCode *ui;
    QString codeAttendu;
};

#endif
