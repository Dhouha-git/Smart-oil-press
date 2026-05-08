#include "circlestatwidget.h"

CircleStatWidget::CircleStatWidget(const QString &label,
                                   const QString &value,
                                   const QString &subtitle,
                                   const QColor  &color,
                                   int            percent,
                                   QWidget       *parent)
    : QWidget(parent)
    , m_label(label), m_value(value)
    , m_subtitle(subtitle), m_color(color)
    , m_percent(percent), m_animated(0)
{
    setMinimumSize(160, 200);
    setMaximumSize(200, 220);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [this](){
        m_animated += 2;
        if (m_animated >= m_percent) {
            m_animated = m_percent;
            m_timer->stop();
        }
        update();
    });
    m_timer->start(15);
}

void CircleStatWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int cx = width() / 2;
    int cy = 80;
    int r  = 50;
    int pen = 10;

    p.setPen(QPen(m_color.lighter(200), pen, Qt::SolidLine, Qt::RoundCap));
    p.drawArc(cx-r, cy-r, r*2, r*2, 90*16, -360*16);

    int span = (int)(-360.0 * m_animated / 100.0 * 16);
    p.setPen(QPen(m_color, pen, Qt::SolidLine, Qt::RoundCap));
    p.drawArc(cx-r, cy-r, r*2, r*2, 90*16, span);

    QFont fVal("Arial", 14, QFont::Bold);
    p.setFont(fVal);
    p.setPen(QColor("#222222"));
    p.drawText(QRect(cx-r, cy-r, r*2, r*2), Qt::AlignCenter, m_value);

    int yBase = cy + r + 18;
    p.setFont(QFont("Arial", 9));
    p.setPen(QColor("#666666"));
    p.drawText(QRect(0, yBase, width(), 20), Qt::AlignCenter, m_label);

    p.setFont(QFont("Arial", 8));
    p.setPen(m_color.darker(120));
    p.drawText(QRect(0, yBase+18, width(), 18), Qt::AlignCenter, m_subtitle);
}
