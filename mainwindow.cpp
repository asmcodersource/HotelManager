#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    // Створення об'єктів даного вікна
    layout_main = std::unique_ptr<QVBoxLayout>(new QVBoxLayout());
    stacked_widget = std::unique_ptr<QStackedWidget>(new QStackedWidget());

    // Ініціалізація головного вікна програми
    setWindowTitle("Hotel Manager");
    setAutoFillBackground(true);
    setMinimumSize(640,480);

    // Створення та ініціалізація віджета меню, та його дій
    menu_bar = std::unique_ptr<QMenuBar>(new QMenuBar());
    menu_bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    menu_bar_items.push_back(std::unique_ptr<QAction>(new QAction("Файл")));
    menu_bar_items.push_back(std::unique_ptr<QAction>(new QAction("Огляд")));
    menu_bar_items.push_back(std::unique_ptr<QAction>(new QAction("Календар")));
    menu_bar_items.push_back(std::unique_ptr<QAction>(new QAction("Таблиця даних")));
    menu_bar_items.push_back(std::unique_ptr<QAction>(new QAction("Редактор")));
    for( unsigned long long i = 0; i < menu_bar_items.size(); i++ )
        menu_bar->addAction(menu_bar_items[i].get());

    // Створення та ініціалізація вікна календаря
    calendar_widget = std::unique_ptr<CalendarScrollWidget>(new CalendarScrollWidget);

    //!!
    stacked_widget->addWidget(calendar_widget.get());
    stacked_widget->setCurrentWidget(calendar_widget.get());
    //!

    // Ініціалізація головного макету віджета
    //setMenuBar(menu_bar.get());
    setCentralWidget(stacked_widget.get());
}

MainWindow::~MainWindow(){
}

