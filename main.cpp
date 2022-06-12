
#include <QApplication>
#include <QDebug>
#include <QIcon>
#include <QFile>
#include "startupwidget.h"
#include "CentralWidget.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QIcon program_icon(":/icons/icons/program_icon.png");
    a.setWindowIcon(program_icon);

    StartupWidget w;
    w.show();
    w.setWindowIcon(program_icon);


    // Загрузка та встановлення файлю стилів
    QFile css_file("style.css");
    if ( !css_file.open(QFile::ReadOnly) ){
        css_file.setFileName(":/style/style.css");
        css_file.open(QFile::ReadOnly);
    }
    auto stylesheet = QString(css_file.readAll());
    css_file.close();
    a.setStyleSheet(stylesheet);


    return a.exec();
}
