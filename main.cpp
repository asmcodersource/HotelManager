#include "mainwindow.h"
#include "calendarwidget.h"
#include <QApplication>
#include <QScrollArea>
#include <QScrollBar>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto* calendar = new CalendarWidget;
    calendar->show();
    return a.exec();
}
