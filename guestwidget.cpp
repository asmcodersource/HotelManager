#include "guestwidget.h"

GuestWidget::GuestWidget(QWidget *parent) : QWidget{parent}{
    layout = new QHBoxLayout;
    name_label = new QLabel;
    layout->addWidget(name_label);
    layout->setContentsMargins(0,0,0,0);
    setAutoFillBackground(true);
    setLayout(layout);
}

void GuestWidget::paintEvent(QPaintEvent* e){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(e);
}

void GuestWidget::setNameText(QString text){
    name_label->setText(text);
    name_label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter );
}

void GuestWidget::setBackgroundColor(QColor color){
    QPalette pal(palette());
    pal.setColor(QPalette::Background, color);
    setPalette(pal);
    repaint();
}
