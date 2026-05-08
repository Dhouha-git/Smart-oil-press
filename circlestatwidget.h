#ifndef CIRCLESTATWIDGET_H
#define CIRCLESTATWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPainter>

class CircleStatWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CircleStatWidget(const QString &label,
                              const QString &value,
                              const QString &subtitle,
                              const QColor  &color,
                              int            percent,
                              QWidget       *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *) override;

private:
    QString m_label, m_value, m_subtitle;
    QColor  m_color;
    int     m_percent, m_animated;
    QTimer *m_timer;
};

#endif
