#include "mainwindow.h"
#include "calendarwidget.h"
#include <QApplication>
#include <QDebug>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Загрузка та встановлення файлю стилів
    QFile css_file(":/style/style.css");
    css_file.open(QFile::ReadOnly);
    auto stylesheet = QString(css_file.readAll());
    css_file.close();
    qDebug() << stylesheet;
    a.setStyleSheet(stylesheet);


    auto* calendar = new CalendarWidget;
    calendar->show();
    return a.exec();
}
