#include "CentralWidget.h"

CentralWidget::CentralWidget(QWidget *parent )  : QWidget(parent){
    // Створення об'єктів віджету
    main_layout = std::unique_ptr<QHBoxLayout>(new QHBoxLayout);
    stacked_widget = std::unique_ptr<QStackedWidget>(new QStackedWidget);
    side_bar_widget = std::unique_ptr<SideBarWidget>(new SideBarWidget);
    calendar_widget = std::unique_ptr<CalendarScrollWidget>(new CalendarScrollWidget);

    // Початкова ініціалізація віджету меню
    side_bar_widget->setParent(this);
    side_bar_widget->setGeometry(0,0, side_bar_widget->minimumWidth(), height());

    // Ініціалізація віджета, та його макету
    main_layout->setContentsMargins(side_bar_widget->minimumWidth(),0,0,0);
    main_layout->setSpacing(0);
    main_layout->addWidget(stacked_widget.get());
    setLayout(main_layout.get());
    setAutoFillBackground(true);
    setMinimumSize(640, 480);

    side_bar_widget->raise();

    // Формування головного меню
    sideBarWidget()->addButton(":/icons/icons/home.png", "Головна", true);
    addDisplayWidget(new QWidget);
    sideBarWidget()->addButton(":/icons/icons/calendar.png", "Календар", true);
    addDisplayWidget(calendar_widget.get());
    sideBarWidget()->addButton(":/icons/icons/list.png", "Список", true);
    addDisplayWidget(new QWidget);
    sideBarWidget()->addButton(":/icons/icons/editor.png", "Редактор", true);
    addDisplayWidget(new QWidget);
    sideBarWidget()->addStretch(99999);
    sideBarWidget()->addButton(":/icons/icons/settings.png", "Налаштування", false);



    //!!
    stacked_widget->addWidget(calendar_widget.get());
    stacked_widget->setCurrentWidget(calendar_widget.get());
}

CentralWidget::~CentralWidget(){

}

void CentralWidget::addDisplayWidget(QWidget* widget){
    display_widgets.push_back(widget);
    stacked_widget->addWidget(widget);
}

void CentralWidget::displayWidgetByIndex(int32_t index){
    if( display_widgets.size() > index )
        stacked_widget->setCurrentIndex(index);
}

SideBarWidget* CentralWidget::sideBarWidget(){
    return side_bar_widget.get();
}

void CentralWidget::resizeEvent(QResizeEvent *event){
   side_bar_widget->setGeometry(0,0, side_bar_widget->width(), height());
   main_layout->setContentsMargins(side_bar_widget->minimumWidth(),0,0,0);
   main_layout->update();
}

