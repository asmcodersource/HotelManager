#include "datewidget.h"

DateWidget::DateWidget(QWidget *parent) : QWidget{parent}{
    // Дінамічне створення необхідних компонентів вікна, та об'єктів цього класу
    layout_main = std::unique_ptr<QVBoxLayout>(new QVBoxLayout(this));
    table_date_day = std::shared_ptr<QTableView>(new QTableView(this));
    model_items_date_day = std::unique_ptr<QStandardItemModel>(new QStandardItemModel(this));
    table_date_month = std::shared_ptr<QTableView>(new QTableView(this));
    model_items_date_month = std::unique_ptr<QStandardItemModel>(new QStandardItemModel(this));

    // ініціалізація представлення таблиць дати
    table_date_day->setModel(model_items_date_day.get());
    table_date_month->setModel(model_items_date_month.get());

    // Ініціалізація головного макету віджета
    layout_main->setSpacing(0);
    layout_main->addWidget(table_date_month.get());
    layout_main->addWidget(table_date_day.get());
    layout_main->setContentsMargins(0,0,0,0);
    layout_main->addStretch(1000);
    setLayout(layout_main.get());

}

void DateWidget::initializeDateModel(){
    int32_t years_count = year_end - year_begin;

    // Заповнення таблиці місяців
    auto* model = model_items_date_month.get();
    QDate date = QDate(year_begin, 1, 1);
    for( int32_t month_number = 0; month_number < (12 * years_count);  month_number++ ){
        auto* month_item_ptr = new QStandardItem();
        month_item_ptr->setText(months[date.month() - 1] + ", " + QString::number(date.year()));
        month_item_ptr->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        // Встановлення кольору представлення в залежності від парності числа
        if( date.month() % 2 ){
             month_item_ptr->setForeground(QColor(255,255,255));
             month_item_ptr->setBackground(QColor(58, 118, 186));
        } else {
            month_item_ptr->setForeground(QColor(255,255,255));
            month_item_ptr->setBackground(QColor(67, 137, 217));
        }
        model->setItem(0,month_number, month_item_ptr);
        // Зміна ширини даної клітинки відносно кількості днів у місяці
        table_date_month->setColumnWidth(month_number, cell_day_size*date.daysInMonth());
        date = date.addMonths(1);
    }

    // Заповнення таблиці днів
    model = model_items_date_day.get();
    date = QDate(year_begin, 1, 1);
    table_date_day->horizontalHeader()->setMinimumSectionSize(0);
    table_date_day->hide();
    model->setColumnCount((365 * years_count));
    items_days = new QStandardItem[(365 * years_count)];
    for( int32_t day_number = 0; day_number < (365 * years_count); day_number++ ){
       auto* day_item_ptr = &(items_days[day_number]);
       day_item_ptr->setText(QString::number(date.day()));
       day_item_ptr->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
       // Встановлення кольору представлення якщо день є суботою, чи неділею
       if( date.dayOfWeek() == 6 || date.dayOfWeek() == 7 ){
            day_item_ptr->setForeground(QColor(255,255,255));
            day_item_ptr->setBackground(QColor(64, 154, 214));
       } else {
            // В іншому випадку просто чорний текст
            day_item_ptr->setForeground(QColor(0,0,0));
       }
       model->setItem(0,day_number, day_item_ptr);
       date = date.addDays(1);
       // Встановлення ширини клітинки
       table_date_day->setColumnWidth(day_number, cell_day_size);
    }
    table_date_day->show();
}

void DateWidget::setYearRange(int32_t begin_year, int32_t end_year){
    year_begin = begin_year;
    year_end = end_year;
    int32_t years_count = end_year - begin_year;

    // Очищення моделі від попередніх даних
    model_items_date_day->clear();
    model_items_date_month->clear();

    // Заповнення моделі новими данними
    initializeDateModel();

    // Ініціалізація графічного відображення елементів
    QTableView* views[2];
    views[0] = table_date_day.get();
    views[1] = table_date_month.get();
    for( QTableView* view: views){
        view->setContentsMargins(0,0,0,0);
        view->verticalHeader()->hide();
        view->horizontalHeader()->hide();
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        view->setFrameStyle(QFrame::NoFrame);
        view->setMaximumHeight(30);
    }

    // Ініціалізація ширини таблиці
    setMinimumWidth(cell_day_size * 365 * years_count );
}

void DateWidget::setDayCellWidth(int32_t new_width){
    cell_day_size = new_width;
    int32_t years_count = year_end - year_begin;
    QDate date = QDate(year_begin, 1, 1);
    // встановлення ширини для клітинок місяця відносно кількості днів у них
    for( int32_t month_number = 0; month_number < (12 * years_count);  month_number++ ){
        table_date_month->setColumnWidth(month_number, cell_day_size*date.daysInMonth());
        date = date.addMonths(1);
    }
    // встановлення ширини для клітонок з числом кожного дня
    for( int32_t day_number = 0; day_number < (365 * years_count); day_number++ ){
         table_date_day->setColumnWidth(day_number, cell_day_size);
    }
}
