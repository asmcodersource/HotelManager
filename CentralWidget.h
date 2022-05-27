#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QResizeEvent>
#include <QDebug>
#include <QStackedWidget>
#include <QLayout>
#include <memory>
#include <QVector>
#include "sidebar.h"
#include "calendarwidget.h"
#include "calendarscrollwidget.h"


class CentralWidget : public QWidget
{
    Q_OBJECT
    std::unique_ptr<QHBoxLayout> main_layout;
    std::unique_ptr<SideBarWidget> side_bar_widget;
    std::unique_ptr<QStackedWidget> stacked_widget;
    std::unique_ptr<CalendarScrollWidget> calendar_widget;
    QVector<QWidget*> display_widgets;


public:
    CentralWidget(QWidget *parent = nullptr);
    ~CentralWidget();

    void addDisplayWidget(QWidget* widget);

    SideBarWidget* sideBarWidget();
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void displayWidgetByIndex(int32_t index);
};
#endif // CENTRALWIDGET_H
