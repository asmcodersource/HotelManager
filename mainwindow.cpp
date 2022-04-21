#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    layout_main = std::unique_ptr<QVBoxLayout>(new QVBoxLayout(this));
    setLayout(layout_main.get());
}

MainWindow::~MainWindow(){
}

