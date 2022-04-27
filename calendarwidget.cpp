#include "calendarwidget.h"

CalendarWidget::CalendarWidget(QWidget *parent) : QWidget{parent}{
    // Створення об'єктів даного вікна
    date_widget = std::unique_ptr<DateWidget>(new DateWidget(this));
    main_layout = std::unique_ptr<QVBoxLayout>(new QVBoxLayout(this));
    // Ініціалізація віджета дат
    date_widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    date_widget->setYearRange(2022, 2025);
    date_widget->setLeftOffset(left_offset);

    // Ініціалізація головного макету віджета
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! DELETE THIS

    auto ptr1 = new RoomsTabWidget(2022,2023);
    ptr1->setLeftOffset(left_offset);
    ptr1->exampleData();
    connect(this, &CalendarWidget::signal_widgetScrolled, ptr1, &RoomsTabWidget::widgetScrolledSlot);
    tabs_widgets.push_back(ptr1);

    auto ptr0 = new RoomsTabWidget(2022,2023);
    ptr0->setLeftOffset(left_offset);
    ptr0->exampleData();
    connect(this, &CalendarWidget::signal_widgetScrolled, ptr0, &RoomsTabWidget::widgetScrolledSlot);
    tabs_widgets.push_back(ptr0);
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    main_layout->addWidget(ptr1);
    main_layout->addWidget(ptr0);
    main_layout->setContentsMargins(0, date_widget->height(),0,0);
    main_layout->addStretch(10000);
    main_layout->setSpacing(0);
    setLayout(main_layout.get());

    // Підключення сигналів\слотів
    connect(date_widget.get(), &DateWidget::signal_widgetResize, this, [this](){
            main_layout->setContentsMargins(0, date_widget->height(),0,0);
        });
}


void CalendarWidget::widgetScrolledSlot(int32_t horizontal_scroll, int32_t vertical_scroll){
    date_widget->raise();
    date_widget->move(0, vertical_scroll);
    emit signal_widgetScrolled(horizontal_scroll, vertical_scroll);
}

quint16 CalendarWidget::leftSideWidth(){
    return left_offset;
}

void CalendarWidget::setLeftSideWidth(quint16 new_width){
    left_offset = new_width;
    date_widget->setLeftOffset(left_offset);
    for( auto* tab : tabs_widgets )
        tab->setLeftOffset(left_offset);
}

