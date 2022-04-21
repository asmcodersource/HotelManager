#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <QLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    std::unique_ptr<QVBoxLayout> layout_main;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:

};
#endif // MAINWINDOW_H
