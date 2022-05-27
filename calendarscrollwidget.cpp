#include "calendarscrollwidget.h"

CalendarScrollWidget::CalendarScrollWidget(QWidget *parent) : QWidget{parent}{
    // Створення об'єктів даного вікна
    calendar_widget = std::unique_ptr<CalendarWidget>(new CalendarWidget(this));
    main_layout = std::unique_ptr<QVBoxLayout>(new QVBoxLayout(this));
    scroll_area = std::unique_ptr<QScrollArea>(new QScrollArea(this));
    horizontal_scroll_bar = std::unique_ptr<ScrollBar>(new ScrollBar);
    vertical_scroll_bar   = std::unique_ptr<ScrollBar>(new ScrollBar);

    // Ініціалізація вікна прокрутки
    scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scroll_area->horizontalScrollBar()->setEnabled(true);
    scroll_area->setWidget(calendar_widget.get());
    scroll_area->setAutoFillBackground(true);
    scroll_area->setWidgetResizable(true);
    scroll_area->horizontalScrollBar()->setSingleStep(10);

    // Ініціалізація головного макету віджета
    main_layout->addWidget(scroll_area.get());
    main_layout->setContentsMargins(0,0,0,0);
    setLayout(main_layout.get());

    scroll_area->setVerticalScrollBar(vertical_scroll_bar.get());
    scroll_area->setHorizontalScrollBar(horizontal_scroll_bar.get());

    // Підключення сигналів\слотів
    connect(scroll_area->verticalScrollBar(), &QScrollBar::valueChanged, this, &CalendarScrollWidget::widgetScrolledSlot);
    connect(scroll_area->horizontalScrollBar(), &QScrollBar::valueChanged, this, &CalendarScrollWidget::widgetScrolledSlot);
    connect(this, &CalendarScrollWidget::signal_widgetScrolled, calendar_widget.get(), &CalendarWidget::widgetScrolledSlot);
}


void CalendarScrollWidget::widgetScrolledSlot(){
    emit signal_widgetScrolled(scroll_area->horizontalScrollBar()->value(), scroll_area->verticalScrollBar()->value());
}
