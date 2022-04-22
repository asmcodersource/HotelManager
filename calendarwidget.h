#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <memory>
#include <QWidget>
#include "datewidget.h"
#include "roomstabwidget.h"

class CalendarWidget : public QWidget
{
    Q_OBJECT
    std::unique_ptr<QVBoxLayout> main_layout;
    std::unique_ptr<DateWidget> date_widget;
    int32_t left_offset = 230;

public:
    explicit CalendarWidget(QWidget *parent = nullptr);
    ~CalendarWidget() = default;
signals:

};

#endif // CALENDARWIDGET_H
