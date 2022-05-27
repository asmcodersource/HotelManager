#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStackedWidget>
#include <QLayout>

#include <calendarscrollwidget.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    std::unique_ptr<QVBoxLayout> layout_main;
    std::unique_ptr<QStackedWidget> stacked_widget;
    std::unique_ptr<QMenuBar> menu_bar;
    std::vector<std::unique_ptr<QAction> > menu_bar_items;

    std::unique_ptr<CalendarScrollWidget> calendar_widget;


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:

};
#endif // MAINWINDOW_H
