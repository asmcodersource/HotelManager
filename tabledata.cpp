#include "tabledata.h"


TableData::TableData(QWidget *parent): QWidget{parent}{ 
    table_view = std::unique_ptr<QTableView>(new QTableView);
    main_layout = std::unique_ptr<QVBoxLayout>(new QVBoxLayout);
    data_base = std::unique_ptr<QSqlDatabase>(new QSqlDatabase);
    header = std::unique_ptr<QLabel>(new QLabel);
    data_model = std::unique_ptr<QStandardItemModel>(new QStandardItemModel(this));
    buttons_layout = std::unique_ptr<QHBoxLayout>(new QHBoxLayout);
    column_resize_filter = std::unique_ptr<ListWidgetColumnResizeFilter>(new ListWidgetColumnResizeFilter(this));
    row_resize_filter = std::unique_ptr<ListWidgetRowResizeFilter>(new ListWidgetRowResizeFilter(this));
    vertical_scroll_bar = std::unique_ptr<ScrollBar>(new ScrollBar(this));
    horizontal_scroll_bar = std::unique_ptr<ScrollBar>(new ScrollBar(this));

    table_view->setVerticalScrollBar(vertical_scroll_bar.get());
    table_view->setHorizontalScrollBar(horizontal_scroll_bar.get());
    setMouseTracking(true);

    buttons.push_back(new QPushButton("Додати"));
    buttons.push_back(new QPushButton("Видалити"));
    buttons.push_back(new QPushButton("Змінити"));
    buttons_layout->setSpacing(3);
    for( auto& button: buttons )
        buttons_layout->addWidget(button);
    buttons_layout->addStretch(100);
    buttons_layout->setContentsMargins(5,5,5,5);


    table_view->setModel(data_model.get());
    table_view->horizontalHeader()->setStretchLastSection(true);
    table_view->verticalHeader()->hide();
    table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_view->setSelectionMode(QAbstractItemView::SingleSelection);
    table_view->horizontalHeader()->setStretchLastSection(true);
    table_view->setFocusPolicy(Qt::NoFocus);
    table_view->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    table_view->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    column_resize_filter->setTableView(table_view.get());
    row_resize_filter->setTableView(table_view.get());
    table_view->setMouseTracking(true);
    QApplication::instance()->installEventFilter(column_resize_filter.get());
    QApplication::instance()->installEventFilter(row_resize_filter.get());

    main_layout->setContentsMargins(0,0,0,0);
    main_layout->setSpacing(0);
    main_layout->addWidget(header.get());
    main_layout->addItem(buttons_layout.get());
    main_layout->addWidget(table_view.get());
    setLayout(main_layout.get());
    setAutoFillBackground(true);

    connect(buttons[0], &QPushButton::clicked, this, &TableData::slot_createNewRow);
    connect(buttons[1], &QPushButton::clicked, this, &TableData::slot_deleteSelectedRow);
    connect(buttons[2], &QPushButton::clicked, this, &TableData::slot_editSelectedRow);
}


void TableData::setQueryInfo( QString table_name, QString select_query, QStringList header_titles, int32_t columns){
    TableData::headers = header_titles;
    TableData::table_name = table_name;
    TableData::select_query = select_query;
    TableData::columns = columns;
    updateData();
}

void TableData::updateData(){
    data_model->clear();
    objects.clear();
    data_model->setHorizontalHeaderLabels(headers);

    QSqlQuery query(select_query);
    uint32_t row = 0;
    while(query.next()){
        for( uint32_t col = 0; col < query.record().count(); col++ ){
            auto* item = new QStandardItem;
            item->setText(query.value(col).toString());
            item->setEditable(false);
            data_model->setItem(row, col, item);
            table_view->setColumnWidth(1, 250);
            table_view->resizeColumnToContents(col);
            table_view->resizeRowToContents(row);
        }
        row++;
    }

    for( int32_t col = 0; col < data_model->columnCount(); col++ )
        if( col > columns || col == 0 )
            table_view->setColumnHidden(col, true);
}

void TableData::setHeaderText(QString text){
    header->setText(text);
}

void TableData::setEditor(DataEditor* editor_widget){
    TableData::edit_widget = std::unique_ptr<DataEditor>(editor_widget);
}

void TableData::resizeEvent(QResizeEvent* event){
    // Отримування минулого, та нового значення розмірів по ширині
    int32_t old_width = event->oldSize().width();
    int32_t new_width = width();

    // Пропорційна зміна розмірів колонок
    double scale_width = static_cast<double>(new_width) / old_width;
    for( int i = 0; i < columns; i++ )
        table_view->setColumnWidth(i, table_view->columnWidth(i) * scale_width);
}

void TableData::slot_createNewRow(){
    QSqlQuery query(QString("INSERT INTO ") + table_name + " DEFAULT VALUES");
    this->updateData();
    edit_widget->setData(data_model.get(), data_model->rowCount() - 1);
    edit_widget->show();
    emit dataUpdated();
}

void TableData::slot_editSelectedRow(){
    QModelIndexList indexes = table_view->selectionModel()->selection().indexes();
    if( indexes.isEmpty() )
        return;
    int32_t row = indexes.at(0).row();
    edit_widget->setData(data_model.get(), row);
    edit_widget->show();
    emit dataUpdated();
}

void TableData::slot_deleteSelectedRow(){
    QModelIndexList indexes = table_view->selectionModel()->selection().indexes();
    if( indexes.isEmpty() )
        return;
    int32_t row = indexes.at(0).row();
    int32_t id = data_model->index(row, 0).data().toInt();
    QString qry = "UPDATE " + table_name + "\n SET state = 'inactive'\n WHERE id = " + QString::number(id);
    QSqlQuery query(qry);
    data_model->removeRow(row);
    emit dataUpdated();
}

void TableData::paintEvent(QPaintEvent* event){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}
//-----------------------------------------------------------------------------------------------


TableDataRooms::TableDataRooms(QWidget *parent ): TableData(parent){

}

void TableDataRooms::updateData(){
    data_model->clear();
    objects.clear();
    data_model->setHorizontalHeaderLabels(headers);

    QSqlQuery query(select_query);
    uint32_t row = 0;
    while(query.next()){
        for( uint32_t col = 0; col < query.record().count(); col++ ){
            auto* item = new QStandardItem;
            item->setText(query.value(col).toString());
            item->setEditable(false);
            data_model->setItem(row, col, item);
            table_view->setColumnWidth(1, 250);
            table_view->resizeRowToContents(row);
        }
        row++;
    }

    for( int32_t row = 0; row < data_model->rowCount(); row++ ){
        QString room_type = data_model->item(row, 3)->text();
        QSqlQuery query(QString("SELECT * FROM RoomsCategory WHERE id = %1").arg(room_type));
        if( !query.next() )
            continue;
        QString class_name = query.value("name").toString();
        data_model->item(row, 3)->setText(class_name);
    }

    for( int32_t row = 0; row < data_model->rowCount(); row++ ){
        QString room_type = data_model->item(row, 3)->text();
        QSqlQuery query(QString("SELECT * FROM RoomsCategory WHERE name = '%1'").arg(room_type));
        if( !query.next() )
            continue;
        if( query.value("state").toString() == "inactive" ){
            data_model->removeRow(row);
            row--;
        }
    }

    for( int32_t col = 0; col < data_model->columnCount(); col++ )
        if( col > columns || col == 0 )
            table_view->setColumnHidden(col, true);
}

//-----------------------------------------------------------------------------------------------


TableDataGuests::TableDataGuests(CalendarWidget* calendar_ptr, QWidget *parent ): TableData(parent){
    calendar = calendar_ptr;
}

void TableDataGuests::slot_createNewRow(){
    QSqlQuery query(QString("INSERT INTO ") + table_name + " DEFAULT VALUES");
    this->updateData();
    qDebug() << calendar->selectedRoomIndex();
    data_model->item(data_model->rowCount() - 1, 4)->setText(calendar->firstDayRange().toString(Qt::ISODate));
    data_model->item(data_model->rowCount() - 1, 5)->setText(calendar->lastDayRange().toString(Qt::ISODate));
    edit_widget->setData(data_model.get(), data_model->rowCount() - 1);
    edit_widget->show();
    emit dataUpdated();
}

void TableDataGuests::updateData(){
    data_model->clear();
    objects.clear();
    data_model->setHorizontalHeaderLabels(headers);

    QSqlQuery query(select_query);
    uint32_t row = 0;
    while(query.next()){
        for( uint32_t col = 0; col < query.record().count(); col++ ){
            auto* item = new QStandardItem;
            item->setText(query.value(col).toString());
            item->setEditable(false);
            data_model->setItem(row, col, item);
            table_view->resizeColumnToContents(col);
            table_view->resizeRowToContents(row);
        }
        row++;
    }

    for( int32_t row = 0; row < data_model->rowCount(); row++ ){
        QString state_id = data_model->item(row, 2)->text();
        QSqlQuery query_state(QString("SELECT * FROM GuestStates WHERE id = %1").arg(state_id));
        if( !query_state.next() )
            continue;
        QString state_name = query_state.value("desc").toString();
        data_model->item(row, 2)->setText(state_name);
        QColor color;
        color.setNamedColor(query_state.value("color").toString());
        data_model->item(row, 2)->setBackground(color);
        table_view->resizeColumnToContents(2);

        QString room_id = data_model->item(row, 7)->text();
        QSqlQuery query_rooom(QString("SELECT * FROM Rooms WHERE id = %1").arg(room_id));
        if( !query_rooom.next() )
            continue;
        if( query_rooom.value("state") == "inactive" ){
            data_model->removeRow(row);
            row--;
            continue;
        }
        QString room_name = query_rooom.value("name").toString();
        data_model->item(row, 7)->setText(room_name);
        table_view->resizeColumnToContents(7);
    }


    for( int32_t col = 0; col < data_model->columnCount(); col++ )
        if( col > columns || col == 0 )
            table_view->setColumnHidden(col, true);
}




