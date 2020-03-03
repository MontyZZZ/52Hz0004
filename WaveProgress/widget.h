#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget() override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    int m_size;
    qreal m_offset;
    qreal m_waterDensity;       // 水波的密度 同长度下水波的个数
    qreal m_waterHeightRate;    // 水波波动的高度比率
    qreal m_max;                // 进度的最大值
    qreal m_min;                // 进度的最小值
    qreal m_value;              // 当前进度值
    bool m_drag;                // 拖动窗口
    QPoint m_pos;               // 拖动窗口位置
    QPoint m_startPos;          // 窗口起始位置
};

#endif // WIDGET_H
