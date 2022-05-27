#include "CentralWidget.h"
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
    a.setStyleSheet(stylesheet);



    auto* ptr = new CentralWidget;
    ptr->show();
    return a.exec();
}
