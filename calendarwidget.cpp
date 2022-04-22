#include "calendarwidget.h"

CalendarWidget::CalendarWidget(QWidget *parent) : QWidget{parent}{
    // Створення об'єктів даного вікна
    date_widget = std::unique_ptr<DateWidget>(new DateWidget(this));
    main_layout = std::unique_ptr<QVBoxLayout>(new QVBoxLayout(this));

    // Ініціалізація віджета дат
    date_widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    date_widget->setYearRange(2022, 2025);
    date_widget->setLeftOffset(left_offset);

    // Ініціалізація головного макету віджета
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! DELETE THIS
    auto ptr = new RoomsTabWidget(2022,2025);
    ptr->exampleData();
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    main_layout->addWidget(date_widget.get());
    main_layout->addWidget(ptr);
    main_layout->addStretch(10000);
    main_layout->setContentsMargins(0,0,0,0);
    main_layout->setSpacing(0);
    setLayout(main_layout.get());
}
