#ifndef GUESTWIDGET_H
#define GUESTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QLayout>
#include <QStyleOption>

class GuestWidget : public QWidget
{
    Q_OBJECT
    int32_t model_row;
    QLabel* name_label;
    QHBoxLayout* layout;


public:
    explicit GuestWidget(QWidget *parent = nullptr);
    void setNameText(QString text);
    void paintEvent(QPaintEvent* e) override;
    void setBackgroundColor(QColor color);

signals:

};

#endif // GUESTWIDGET_H
