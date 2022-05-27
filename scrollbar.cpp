#include "scrollbar.h"

ScrollBar::ScrollBar(QScrollBar* parent) : QScrollBar(parent){
    setAutoFillBackground(false);
    slider = nullptr;
    setStyleSheet("width:0px");
}

void ScrollBar::sliderChange(SliderChange change){
    if( slider == nullptr ){
        slider = new Slider(this->QAbstractSlider::orientation(), this->parentWidget()->parentWidget());
        connect(this, &QScrollBar::rangeChanged, slider, &QAbstractSlider::setRange);
        connect(this, &QScrollBar::valueChanged, slider, &QAbstractSlider::setValue);
        connect(slider, &QAbstractSlider::valueChanged, this, &QScrollBar::setValue);
        slider->setSingleStep(1);
        this->parentWidget()->parentWidget()->setMouseTracking(true);
        QApplication* application = static_cast<QApplication *>(QApplication::instance());
        application->installEventFilter(this);
        scrollarea = this->parentWidget()->parentWidget();
    }
    if( change == SliderChange::SliderStepsChange )
        slider->setOrientation(this->QAbstractSlider::orientation());
}


void ScrollBar::paintEvent(QPaintEvent *event){
    slider->updatePosition();
    slider->repaint();
}

void ScrollBar::resizeEvent(QResizeEvent *event){
    slider->updatePosition();
    //slider->repaint();
}

void ScrollBar::hideEvent(QHideEvent *event){
    qDebug() << "hide";
    slider->hide();
}

void ScrollBar::showEvent( QShowEvent * event ){
    slider->show();
    qDebug() << "show";
}

bool ScrollBar::eventFilter(QObject* object, QEvent* event){
    static int i = 0;
    if( object->parent() == scrollarea && event->type() == QEvent::MouseMove ){
        if( this->orientation == Qt::Orientation::Vertical ){
            if( scrollarea->mapFromGlobal(QCursor::pos()).y() > scrollarea->height() - 10 )
                slider->hover();
        } else {
            if( scrollarea->mapFromGlobal(QCursor::pos()).x() > scrollarea->width() - 10)
                slider->hover();
        }
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////////

Slider::Slider(QWidget* parent ): QAbstractSlider(parent){
    setMouseTracking(true);
    slider_color = QColor(90, 150, 230, 0);
    connect(this, &Slider::valueChanged, [this](){
        hover();
    });
}

Slider::Slider(Qt::Orientation orientation, QWidget* parent ): Slider(parent){
    Slider::orientation = orientation;
}

void Slider::paintEvent(QPaintEvent *event){
    QPainter paint(this);
    updatePosition();
    paint.setPen(QColor(0,0,0,0));
    paint.setBrush(QColor(slider_color));
    paint.drawRect(0,0, width(), height());
}

void Slider::updatePosition(){
    int value;
    switch (orientation) {
        case Qt::Orientation::Horizontal:
            if( this->maximum() > 0 ){
                lenght = parentWidget()->width() *  parentWidget()->width() / (this->maximum() + parentWidget()->width());
                if( lenght < 5 )
                    lenght = 5;
                value = this->value() * (parentWidget()->width() - lenght) / this->maximum();
            } else {
                lenght = parentWidget()->width();
                value = 0;
            }
            resize(lenght, height()+10);
            move(value, (parentWidget()->height() - 10));
            break;
        case Qt::Orientation::Vertical:
            if( this->maximum() > 0 ){
                lenght = parentWidget()->height() *  parentWidget()->height() / (this->maximum() + parentWidget()->height());
                if( lenght < 5 )
                    lenght = 5;
                value = this->value() * (parentWidget()->height() - lenght) / this->maximum();
            } else {
                lenght = parentWidget()->height();
                value = 0;
            }
            resize(width(), lenght);
            move((parentWidget()->width() - 10), value);
            break;
    };
}

void Slider::sliderChange(SliderChange change){
    updatePosition();
}

void Slider::setOrientation(Qt::Orientation orientation){
    updatePosition();
    Slider::orientation = orientation;
}

void Slider::mousePressEvent(QMouseEvent* event){
    mouse_start_position = QCursor::pos();
    scroll_start_value = value();
}

void Slider::mouseMoveEvent(QMouseEvent* event){
    if( !(event->buttons() & Qt::LeftButton) )
        return;
    int offset = 0;
    if(orientation == Qt::Orientation::Horizontal){
        if( lenght == parentWidget()->width() )
            return;
        offset = QCursor::pos().x() - mouse_start_position.x();
        offset *= this->maximum() / (parentWidget()->width() - lenght);
    } else {
        if( lenght == parentWidget()->height() )
            return;
        offset = QCursor::pos().y() - mouse_start_position.y();
        offset *= this->maximum() / (parentWidget()->height() - lenght);
    }
    setValue(scroll_start_value + offset);
    emit QAbstractSlider::valueChanged(value());
}

void Slider::hover(){
    last_hide++;
    parentWidget()->unsetCursor();
    if( showed == false ){
        showed = true;
        QTimer::singleShot(5000, this, &Slider::hoverTimerFinish);
        QPropertyAnimation *animation = new QPropertyAnimation(this, "color");
        animation->setDuration(500);
        animation->setStartValue(QColor(25, 31, 41, 0));
        animation->setEndValue(QColor(25, 31, 41, 255));
        animation->start();
    }
}

void Slider::hoverTimerFinish(){
    last_hide--;
    if( last_hide != 0 ){
        last_hide = 1;
        QTimer::singleShot(500, this, &Slider::hoverTimerFinish);
    } else {
        QPropertyAnimation *animation = new QPropertyAnimation(this, "color");
        animation->setDuration(500);
        animation->setStartValue(QColor(25, 31, 41, 255));
        animation->setEndValue(QColor(25, 31, 41, 0));
        animation->start();
        showed = false;
    }
}

void Slider::setColor (QColor color){
    slider_color = color;
    repaint();
}

QColor Slider::color(){
   return Qt::black; // getter is not really needed for now
}
