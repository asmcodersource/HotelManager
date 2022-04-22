#include "roomstabwidget.h"

RoomsTabWidget::RoomsTabWidget(int32_t year_begin, int32_t year_end, QWidget *parent) : QWidget{parent}{
    RoomsTabWidget::year_begin = year_begin;
    RoomsTabWidget::year_end = year_end;

    // Створення об'єктів даного вікна
    main_layout  = std::unique_ptr<QVBoxLayout>(new QVBoxLayout());
    rooms_layout = std::unique_ptr<QHBoxLayout>(new QHBoxLayout());
    rooms_type_name = std::unique_ptr<QLabel>(new QLabel(this));
    rooms_table = std::unique_ptr<QTableView>(new QTableView);
    rooms_model = std::unique_ptr<QStandardItemModel>(new QStandardItemModel());
    rooms_organize_table = std::unique_ptr<QTableView>(new QTableView);
    rooms_organize_model = std::unique_ptr<QStandardItemModel>(new QStandardItemModel());

    // Ініціалізація вікна вкладки, та її об'єктів
    setAutoFillBackground(true);
    rooms_type_name->setText("Кімната класу Example");

    // Ініціалізація таблиць
    rooms_table->setModel(rooms_model.get());
    rooms_table->setFixedWidth(room_table_width);
    rooms_table->horizontalHeader()->hide();
    rooms_table->verticalHeader()->hide();
    rooms_table->horizontalHeader()->setStretchLastSection(true);
    rooms_table->setFrameStyle(0);

    rooms_organize_table->setModel(rooms_organize_model.get());
    rooms_organize_table->horizontalHeader()->hide();
    rooms_organize_table->verticalHeader()->hide();
    rooms_table->setFocusPolicy(Qt::NoFocus);
    rooms_organize_table->setFocusPolicy(Qt::NoFocus);
    rooms_organize_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    rooms_organize_table->setFrameStyle(0);

    // Ініціалізація макету таблиці кімнат
    rooms_layout->addWidget(rooms_table.get());
    rooms_layout->addWidget(rooms_organize_table.get());
    rooms_layout->setSpacing(0);
    rooms_layout->setContentsMargins(0,0,0,0);

    // Ініціалізація головного макету віджета
    main_layout->addWidget(rooms_type_name.get());
    main_layout->addItem(rooms_layout.get());
    main_layout->setContentsMargins(0,0,0,0);
    main_layout->setSpacing(0);
    setLayout(main_layout.get());
}


void RoomsTabWidget::exampleData(){
    for( int i = 0; i < 5; i++ ){
        addRoom(QString::number(i + 100));
    }
}

void RoomsTabWidget::addRoom(QString room_name){
    auto* item = new QStandardItem;
    item->setText(room_name);
    item->setSelectable(false);
    rooms_model->setItem(rooms_list.size(), 0, item);
    addDayCells();
    rooms_table->setRowHeight(rooms_list.size(), 30);
    rooms_table->setMaximumHeight((rooms_list.size() + 1) * 30 );
    rooms_organize_table->setMaximumHeight((rooms_list.size() + 1) * 30 );
    rooms_list.append(room_name);
}

void RoomsTabWidget::addDayCells(){
    // Заповнення таблиці днів
    QDate date = QDate(year_begin, 1, 1);
    int32_t number_of_days = date.daysTo(QDate(year_end, 1, 1));
    rooms_organize_table->horizontalHeader()->setMinimumSectionSize(0);
    rooms_organize_table->hide();
    rooms_organize_model->setColumnCount(number_of_days);
    auto* items_days = new QStandardItem[number_of_days];
    day_items_arrays.push_back(items_days);
    for( int32_t day_number = 0; day_number < number_of_days; day_number++ ){
       auto* day_item_ptr = &(items_days[day_number]);
       day_item_ptr->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
       day_item_ptr->setEditable(false);
       day_item_ptr->setCheckable(false);
       day_item_ptr->setSelectable(false);
       // Встановлення кольору представлення якщо день є суботою, чи неділею
       if( date.dayOfWeek() == 6 || date.dayOfWeek() == 7 ){
           day_item_ptr->setForeground(QColor(255,255,255));
           day_item_ptr->setBackground(QColor(64, 154, 214));
       } else {
           // В іншому випадку просто чорний текст
           day_item_ptr->setForeground(QColor(0,0,0));
       }
       rooms_organize_model->setItem(day_items_arrays.size() - 1,day_number, day_item_ptr);
       date = date.addDays(1);
       // Встановлення ширини клітинки
       rooms_organize_table->setColumnWidth(day_number, cell_day_size);
    }
    rooms_organize_table->show();
}


void RoomsTabWidget::setDayCellWidth(int32_t new_width){
    cell_day_size = new_width;
    QDate date = QDate(year_begin, 1, 1);
    int32_t number_of_days = date.daysTo(QDate(year_end, 1, 1));
    // встановлення ширини для клітонок з числом кожного дня
    for( int32_t day_number = 0; day_number < number_of_days; day_number++ ){
         rooms_organize_table->setColumnWidth(day_number, cell_day_size);
    }
}
