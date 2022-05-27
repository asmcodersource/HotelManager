#include "roomstabwidget.h"

RoomsTabWidget::RoomsTabWidget(int32_t year_begin, int32_t year_end, QWidget *parent) : QWidget{parent}{
    RoomsTabWidget::year_begin = year_begin;
    RoomsTabWidget::year_end = year_end;

    // Створення об'єктів даного вікна
    main_layout  = std::unique_ptr<QVBoxLayout>(new QVBoxLayout());
    rooms_layout = std::unique_ptr<QHBoxLayout>(new QHBoxLayout());
    rooms_header_layout = std::unique_ptr<QHBoxLayout>(new QHBoxLayout());
    rooms_type_name = std::unique_ptr<QLabel>(new QLabel());
    rooms_table = std::unique_ptr<QTableView>(new QTableView);
    rooms_model = std::unique_ptr<QStandardItemModel>(new QStandardItemModel(this));
    rooms_organize_table = std::unique_ptr<QTableView>(new QTableView());
    rooms_organize_model = std::unique_ptr<QStandardItemModel>(new QStandardItemModel(this));
    spacer_for_header_layout = std::unique_ptr<QSpacerItem>(new QSpacerItem(0,1));

    // Ініціалізація вікна вкладки, та її об'єктів
    rooms_type_name->setText("Кімната класу Example");
    setAutoFillBackground(true);
    rooms_header_layout->addItem(spacer_for_header_layout.get());
    rooms_header_layout->addWidget(rooms_type_name.get());
    rooms_header_layout->setContentsMargins(0,0,0,0);

    // Ініціалізація таблиць
    rooms_table->setParent(this);
    rooms_table->move(0, rooms_type_name->height() + 2);
    rooms_table->setModel(rooms_model.get());
    rooms_table->setFixedWidth(room_table_width);
    rooms_table->horizontalHeader()->hide();
    rooms_table->verticalHeader()->hide();
    rooms_table->horizontalHeader()->setStretchLastSection(true);
    rooms_table->setFrameStyle(0);
    rooms_table->setFocusPolicy(Qt::NoFocus);

    rooms_organize_table->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    rooms_organize_table->setModel(rooms_organize_model.get());
    rooms_organize_table->horizontalHeader()->hide();
    rooms_organize_table->verticalHeader()->hide();
    rooms_organize_table->setFocusPolicy(Qt::NoFocus);
    rooms_organize_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    rooms_organize_table->setFrameStyle(0);

    // Ініціалізація макету таблиці кімнат
    // rooms_layout->addWidget(rooms_table.get());
    rooms_layout->addWidget(rooms_organize_table.get());
    rooms_layout->setSpacing(0);
    rooms_layout->setContentsMargins(0,0,0,0);

    // Ініціалізація головного макету віджета
    main_layout->addItem(rooms_header_layout.get());
    main_layout->addItem(rooms_layout.get());
    main_layout->setContentsMargins(0,0,0,0);
    main_layout->setSpacing(0);
    setLayout(main_layout.get());

}


void RoomsTabWidget::exampleData(){
    for( int i = 0; i < 23; i++ ){
        addRoom(QString::number(i + 100));
    }

}

void RoomsTabWidget::addRoom(QString room_name){
    auto* item = new QStandardItem;
    item->setText(room_name);
    item->setSelectable(false);
    item->setEditable(false);
    item->setBackground(QColor(172, 211, 193));
    item->setForeground(QColor(86, 131, 111));
    rooms_model->setItem(rooms_list.size(), 0, item);
    addDayCells();
    rooms_table->setRowHeight(rooms_list.size(), cell_day_height);
    rooms_table->setMaximumHeight((rooms_list.size() + 1) * cell_day_height );
    rooms_organize_table->setMinimumHeight((rooms_list.size() + 1) * cell_day_height);
    rooms_list.append(room_name);
    rooms_table->setMinimumHeight(rooms_organize_table->height());
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
       // Встановлення кольору представлення якщо день є суботою, чи неділею
       if( date.dayOfWeek() == 6 || date.dayOfWeek() == 7 ){
           day_item_ptr->setForeground(QColor(255,255,255));
           day_item_ptr->setBackground(QColor(89, 158, 127));
       } else {
           // В іншому випадку просто чорний текст
           if( rooms_list.size() % 2 )
                day_item_ptr->setBackground(QColor(237, 237, 237));
           day_item_ptr->setForeground(QColor(0,0,0));
       }
       rooms_organize_model->setItem(day_items_arrays.size() - 1,day_number, day_item_ptr);
       date = date.addDays(1);
       // Встановлення ширини клітинки
       rooms_organize_table->setColumnWidth(day_number, cell_day_width);
    }
    rooms_organize_table->setMinimumWidth(cell_day_width * number_of_days );
    rooms_organize_table->show();
}

void RoomsTabWidget::setLeftOffset(int32_t offset ){
    room_table_width = offset;
    rooms_table->setFixedWidth(room_table_width);
    rooms_layout->setContentsMargins(room_table_width,0,0,0);
}

void RoomsTabWidget::widgetScrolledSlot(int32_t horizontal_scroll, int32_t vertical_scroll){
    rooms_table->raise();
    rooms_table->move(horizontal_scroll, rooms_type_name->height() + 0);
    spacer_for_header_layout->changeSize(horizontal_scroll, 1);
    rooms_header_layout->invalidate();
    main_layout->invalidate();

}

qint16 RoomsTabWidget::dayCellWidth(){
    return cell_day_width;
}

void RoomsTabWidget::setDayCellWidth(qint16 new_value){
    cell_day_width =  new_value;
    QDate date = QDate(year_begin, 1, 1);
    int32_t number_of_days = date.daysTo(QDate(year_end, 1, 1));
    // встановлення ширини для клітонок з числом кожного дня
    for( int32_t day_number = 0; day_number < number_of_days; day_number++ ){
         rooms_organize_table->setColumnWidth(day_number, cell_day_width);
    }
    rooms_organize_table->setMinimumWidth(cell_day_width * number_of_days );
}

qint16 RoomsTabWidget::dayCellHeight(){
    return cell_day_height;
}

void RoomsTabWidget::setDayCellHeight(qint16 new_value){
    cell_day_height = new_value;
    // встановлення висоти для клітонок з числом кожного дня
    for( int32_t room_number = 0; room_number < rooms_list.size(); room_number++ ){
         rooms_organize_table->setRowHeight(room_number, cell_day_height);
         rooms_table->setRowHeight(room_number, cell_day_height);
    }
    rooms_organize_table->setMinimumHeight((rooms_list.size()) * cell_day_height);
    rooms_table->setMinimumHeight(rooms_organize_table->height());
}
