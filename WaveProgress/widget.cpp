#include "widget.h"

#include <QPainter>
#include <QtMath>
#include <QDebug>
#include <QTimer>
#include <QMouseEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    m_size = 100;
    m_offset = 50;
    m_waterDensity = 5;
    m_waterHeightRate = 0.03;
    m_max = 100;
    m_min = 0;
    m_value = 0;

    setFixedSize(QSize(m_size, m_size));
    startTimer(100);
}

Widget::~Widget()
{
}

void Widget::paintEvent(QPaintEvent *event)
{
    //qDebug() << "paintEvent";
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 背景透明
    painter.save();
    QPen pen;
    pen.setColor(QColor(0, 0, 0, 1));
    painter.setPen(pen);
    painter.setBrush(QBrush(QColor(0, 0, 0, 1)));

    painter.drawRect(QRect(0, 0, m_size, m_size));
    painter.restore();

    // 绘制波纹
    QColor bgColor(Qt::white);
    QColor fillColor(Qt::blue);

    qreal percent = 1 - (m_value - m_min) / (m_max - m_min);
    //qDebug() << percent << m_offset;

    // 正弦曲线公式 y = A * sin(ωx + φ) + k
    // w 周期,水波的密度,同长度下水波的个数
    qreal w = m_waterDensity * M_PI / m_size;
    // A 振幅,水波波动的高度
    qreal A = m_size * m_waterHeightRate;
    // k y轴偏移,可以理解为进度宝粉币
    qreal k = m_size * percent;

    //第一条水波路径
    QPainterPath wavePath1;
    //第二条波水波路径
    QPainterPath wavePath2;

    //移动到左上角起始点
    wavePath1.moveTo(0, m_size);
    wavePath2.moveTo(0, m_size);

    m_offset += 0.6;
    if (m_offset > (m_size / 2)) {
        m_offset = 0;
    }

    for(int x = 0; x <= m_size; x++) {
        qreal waveY1 = (A * sin(w * x + m_offset)) + k;
        qreal waveY2 = (A * sin(w * x + m_offset + (m_size / 2 * w))) + k;

        //如果当前值为最小值则Y轴为高度
        if (m_value == m_min) {
            waveY1 = m_size;
            waveY2 = m_size;
        }

        //如果当前值为最大值则Y轴为0
        if (m_value == m_max) {
            waveY1 = 0;
            waveY2 = 0;
        }

        wavePath1.lineTo(x, waveY1);
        wavePath2.lineTo(x, waveY2);
    }

    // 方形加上水波纹
    wavePath1.lineTo(m_size, m_size);
    wavePath2.lineTo(m_size, m_size);

    // 圆形路径
    QPainterPath bigPath;
    bigPath.addEllipse(0, 0, m_size, m_size);
    painter.setBrush(bgColor);
    painter.save();

    QPainterPath path;
    painter.setPen(Qt::NoPen);
    QColor waterColor1 = fillColor;
    waterColor1.setAlpha(100);
    QColor waterColor2 = fillColor;
    waterColor2.setAlpha(180);

    // intersected 之后是波浪加上圆弧
    path = bigPath.intersected(wavePath1);
    painter.setBrush(waterColor1);
    painter.drawPath(path);
//    painter.drawPath(wavePath1);
//    painter.drawPath(wavePath2);

    path = bigPath.intersected(wavePath2);
    painter.setBrush(waterColor2);
    painter.drawPath(path);

    painter.restore();

    QFont font;
    font.setPixelSize(m_size / 5);
    painter.setPen(Qt::black);
    painter.setFont(font);
    painter.drawText(this->rect(), Qt::AlignCenter, QString("%0%").arg(QString::number(m_value)));
}

void Widget::timerEvent(QTimerEvent *event)
{
    if (m_value < 100) {
        m_value++;
    }
    this->update();

}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_drag = true;
        m_pos = event->globalPos();
        m_startPos = frameGeometry().topLeft();
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_drag)
    {
        QPoint distance = event->globalPos() - m_pos;
        move(m_startPos + distance);
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_drag = false;
    }
}

