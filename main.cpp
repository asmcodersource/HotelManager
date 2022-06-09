
#include <QApplication>
#include <QDebug>
#include <QFile>
#include "startupwidget.h"
#include "CentralWidget.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StartupWidget w;
    w.show();

    // Загрузка та встановлення файлю стилів
    QFile css_file(":/style/style.css");
    css_file.open(QFile::ReadOnly);
    auto stylesheet = QString(css_file.readAll());
    css_file.close();
    a.setStyleSheet(stylesheet);



    return a.exec();
}
