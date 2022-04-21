#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <QWidget>
#include <memory>
#include <QHeaderView>
#include <QSizePolicy>
#include <QTableView>
#include <QStandardItem>
#include <QDate>
#include <QDebug>
#include <QLayout>
#include <QScrollArea>
#include <QScrollBar>
#include "datewidget.h"


/*
 Клас що надає віджет календаря, із таблицею кімнат, якими можна керувати.
*/
class CalendarWidget : public QWidget
{
    Q_OBJECT
    std::unique_ptr<DateWidget> date_widget;
    std::unique_ptr<QVBoxLayout> main_layout;
    std::unique_ptr<QScrollArea> scroll_area;
public:
    explicit CalendarWidget(QWidget *parent = nullptr);
    ~CalendarWidget() = default;

signals:

};

#endif // CALENDARWIDGET_H
