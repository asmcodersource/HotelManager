#include "calendarwidget.h"

CalendarWidget::CalendarWidget(QWidget *parent) : QWidget{parent}{
    // Створення об'єктів даного вікна
    date_widget = std::unique_ptr<DateWidget>(new DateWidget(this));
    main_layout = std::unique_ptr<QVBoxLayout>(new QVBoxLayout(this));
    scroll_area = std::unique_ptr<QScrollArea>(new QScrollArea(this));

    // Ініціалізація вікна прокрутки
    scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scroll_area->horizontalScrollBar()->setEnabled(true);
    scroll_area->setWidget(date_widget.get());
    scroll_area->setWidgetResizable(true);

    // Ініціалізація віджета дат
    date_widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    date_widget->setYearRange(2022, 2025);

    // Ініціалізація головного макету віджета
    main_layout->addWidget(scroll_area.get());
    setLayout(main_layout.get());
}
