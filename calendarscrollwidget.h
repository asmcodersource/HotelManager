#ifndef CALENDARSCROLLWIDGET_H
#define CALENDARSCROLLWIDGET_H

#include <memory>
#include <QWidget>
#include <QHeaderView>
#include <QSizePolicy>
#include <QTableView>
#include <QStandardItem>
#include <QDate>
#include <QDebug>
#include <QLayout>
#include <QScrollArea>
#include <QScrollBar>
#include "calendarwidget.h"


/*
 Клас що надає віджет календаря, із таблицею кімнат, якими можна керувати.
*/
class CalendarScrollWidget : public QWidget
{
    Q_OBJECT
    std::unique_ptr<QVBoxLayout> main_layout;
    std::unique_ptr<QScrollArea> scroll_area;
    std::unique_ptr<CalendarWidget> calendar_widget;

public:
    explicit CalendarScrollWidget(QWidget *parent = nullptr);
    ~CalendarScrollWidget() = default;

signals:

};

#endif // CALENDARSCROLLWIDGET_H
