#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include <QScrollBar>
#include <QPainter>
#include <QScrollArea>
#include <QAbstractSlider>
#include <QAbstractScrollArea>
#include <QAbstractButton>
#include <QSize>
#include <Qt>
#include <QEvent>
#include <QDebug>
#include <QApplication>
#include <QTimer>
#include <atomic>
#include <QMouseEvent>
#include <QPropertyAnimation>



class Slider:public QAbstractSlider{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)
    QColor slider_color{131, 238, 255}; // color of beginning

    Qt::Orientation orientation;
    double lenght = 0;
    std::atomic<uint64_t> last_hide;
    uint32_t scroll_start_value = 0;
    QPoint mouse_start_position;
    bool showed = false;
    QTimer showed_timer;

public:
    Slider(QWidget* parent = nullptr);
    Slider(Qt::Orientation type, QWidget* parent = nullptr );
    void paintEvent(QPaintEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

    void setOrientation(Qt::Orientation orientation);
    void sliderChange(SliderChange change) override;
    void updatePosition();
    void hover();

    void setColor (QColor color);
    QColor color();

public slots:
    void hoverTimerFinish();
signals:
    void colorChanged();
};


class ScrollBar : public QScrollBar
{
    Q_OBJECT
    Qt::Orientation orientation;
    Slider* slider;
    QWidget* scrollarea;

public:
    ScrollBar(QScrollBar* parent = nullptr);
    void sliderChange(SliderChange change) override;
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent( QShowEvent * event ) override;
    void hideEvent(QHideEvent *event) override;
    bool eventFilter(QObject* object, QEvent* event) override;
};


#endif // SCROLLBAR_H
