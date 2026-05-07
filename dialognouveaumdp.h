#ifndef DIALOGNOUVEAUMDP_H
#define DIALOGNOUVEAUMDP_H

#include <QDialog>

namespace Ui {
class DialogNouveauMdp;
}

class DialogNouveauMdp : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNouveauMdp(QWidget *parent = nullptr);
    ~DialogNouveauMdp();
    QString getNouveauMdp();

private slots:
    void on_btn_changer_clicked();
    void on_btn_annuler_clicked();

private:
    Ui::DialogNouveauMdp *ui;
};

#endif
