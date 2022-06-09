#ifndef STARTUPWIDGET_H
#define STARTUPWIDGET_H

#include <memory>
#include <QWidget>
#include <QLineEdit>
#include <QApplication>
#include <QLayout>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QFileDialog>
#include <QFile>
#include "CentralWidget.h"
#include <QDebug>
#include "mainwindow.h"
#include <QPushButton>

class StartupWidget : public QWidget{
    Q_OBJECT
    std::unique_ptr<QSqlDatabase> data_base;
    std::unique_ptr<CentralWidget> central_widget;
    std::unique_ptr<MainWindow> main_window;
    QLayout* main_layout;
    QLayout* file_path_layout;
    QLayout* buttons_layout;
    QLineEdit* file_path;
    QPushButton *load, *create, *ok, *cancel;

public:
    explicit StartupWidget(QWidget *parent = nullptr);

public slots:
    void newPathSelected(QString new_path);
    void createNewFile(QString new_path);
    void openFile();
};

#endif // STARTUPWIDGET_H
