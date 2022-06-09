#include "calendarwidget.h"

CalendarWidget::CalendarWidget(QWidget *parent) : QWidget{parent}{
    // Створення об'єктів даного вікна
    date_widget = std::unique_ptr<DateWidget>(new DateWidget(this));
    main_layout = std::unique_ptr<QVBoxLayout>(new QVBoxLayout());
    date_user_inform_widget = std::unique_ptr<DateUserInformerWidget>( new DateUserInformerWidget(this));
    calendar_first_day = QDate(QDateTime::currentDateTime().date().year(), 1, 1);

    // Ініціалізація віджета дат
    date_widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    date_widget->setYearRange(calendar_first_day.year(), calendar_first_day.year()+2);
    date_widget->setLeftOffset(left_offset);

    // Ініціалізація головного макету віджета
    date_user_inform_widget->raise();
    date_user_inform_widget->setFixedWidth(left_offset);
    date_user_inform_widget->setFixedHeight(date_widget->height());
    widgetScrolledSlot(0, 0);

    // Отримування записів житла із бази даних
    main_layout->setAlignment(Qt::AlignTop);
    QSqlQuery query_categories("SELECT * FROM RoomsCategory WHERE state != 'inactive'");
    while(query_categories.next()){
        auto new_tab = new RoomsTabWidget(calendar_first_day.year(), calendar_first_day.year()+2);
        new_tab->setLeftOffset(left_offset);
        new_tab->setHeaderText(query_categories.value("name").toString());
        int32_t class_id = query_categories.value("id").toInt();
        connect(this, &CalendarWidget::signal_widgetScrolled, new_tab, &RoomsTabWidget::widgetScrolledSlot);
        tabs_widgets.push_back(new_tab);
        QSqlQuery query_rooms(QString("SELECT * FROM Rooms WHERE state != 'inactive' AND type = %1").arg(class_id));
        while(query_rooms.next()){
            new_tab->addRoom(query_rooms.value("name").toString());
        }
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        main_layout->addWidget(new_tab);
        connect(new_tab, &RoomsTabWidget::newSelection, this, [&, this](RoomsTabWidget* ptr){
            for( auto* tab: tabs_widgets )
                if( tab != ptr )
                    tab->resetSelection();
            last_selection_tab = ptr;
        });
        connect(new_tab, &RoomsTabWidget::newSelectionRange, this, &CalendarWidget::newSelectedRange);
    }


    for( int i = 0; i < 10; i++ )
        main_layout->addStretch(999999);
    main_layout->setContentsMargins(0, date_widget->height(),0,0);
    main_layout->setSpacing(0);
    setLayout(main_layout.get());

    widgetScrolledSlot(1, 1);


    // Підключення сигналів\слотів
    connect(date_widget.get(), &DateWidget::signal_widgetResize, this, [this](){
            main_layout->setContentsMargins(0, date_widget->height(),0,0);

        });
}


void CalendarWidget::widgetScrolledSlot(int32_t horizontal_scroll, int32_t vertical_scroll){
    date_widget->raise();
    date_widget->move(0, vertical_scroll);
    date_user_inform_widget->move(horizontal_scroll, vertical_scroll);
    date_user_inform_widget->raise();
    QString date = date_widget->getValueFromVisualCenter(static_cast<QWidget*>(parent())->width() - date_user_inform_widget->width(), horizontal_scroll);
    first_line_date = "<center><h2>" + date + "</h2></center>\n";
    date = first_line_date + "<center>" + first_day.toString(Qt::ISODate) + " - " + last_day.toString(Qt::ISODate) + "</center>";
    date_user_inform_widget->setLabelValue(date);
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
    date_user_inform_widget->raise();
    date_user_inform_widget->setFixedWidth(left_offset);
    date_user_inform_widget->setFixedHeight(date_widget->height());
}

void CalendarWidget::paintEvent(QPaintEvent* event){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}

void CalendarWidget::newSelectedRange(int32_t room_row, int32_t first_day, int32_t last_day){
    CalendarWidget::room_row = room_row;
    CalendarWidget::first_day = calendar_first_day.addDays(first_day);
    CalendarWidget::last_day = calendar_first_day.addDays(last_day);
    QString date = first_line_date + "<center>" + CalendarWidget::first_day.toString(Qt::ISODate) + " - " + CalendarWidget::last_day.toString(Qt::ISODate) + "</center>";
    date_user_inform_widget->setLabelValue(date);
}

void CalendarWidget::updateData(){
    for( auto* guest_widget : guest_widgets )
        guest_widget->deleteLater();
    guest_widgets.clear();

    QStringList colors;
    QSqlQuery query_colors("SELECT * FROM GuestStates");
    while(query_colors.next()){
        colors.push_back(query_colors.value("color").toString());
    }

    QSqlQuery query_categories("SELECT * FROM RoomsCategory WHERE state != 'inactive'");
    int32_t tab_id = 0;
    while(query_categories.next()){
        int32_t class_id = query_categories.value(0).toInt();
        QSqlQuery query_rooms(QString("SELECT * FROM Rooms WHERE state != 'inactive' AND type = %1").arg(class_id));
        int32_t row = 0;
        while(query_rooms.next()){
            QSqlQuery request_guests(QString("SELECT * FROM Guests WHERE state != 'inactive' AND room_id = %1").arg(query_rooms.value("id").toInt()));
            while(request_guests.next()){
                QString name = request_guests.value("name").toString();
                QString first_day = request_guests.value("begin_date").toString();
                QString last_day = request_guests.value("end_date").toString();
                QDate first_day_date = QDate::fromString(first_day, Qt::ISODate);
                QDate last_day_date = QDate::fromString(last_day, Qt::ISODate);
                int32_t first_day_int = calendar_first_day.daysTo(first_day_date);
                int32_t last_day_int  = calendar_first_day.daysTo(last_day_date);
                QColor color;
                color.setNamedColor(colors[request_guests.value("state_id").toInt() - 1]);
                guest_widgets.push_back(tabs_widgets[tab_id]->addGuestWidget(row,
                                                                             first_day_int,
                                                                             last_day_int,
                                                                             request_guests.value("name").toString(),
                                                                             color));
            }
            row++;
        }
        tab_id++;
    }
}

QDate CalendarWidget::firstDayRange(){
    return first_day;
}

QDate CalendarWidget::lastDayRange(){
    return last_day;
}

int32_t CalendarWidget::selectedRoomIndex(){
    if( last_selection_tab == nullptr )
        return 0;
    int32_t tab_id = 0;
    for(; last_selection_tab != tabs_widgets[tab_id]; tab_id++ );
    QSqlQuery query_categories(QString("SELECT * FROM RoomsCategory WHERE state != 'inactive' LIMIT %1, 1").arg(tab_id));
    query_categories.next();
    int32_t class_id = query_categories.value("id").toInt();
    QSqlQuery query_rooms(QString("SELECT * FROM Rooms WHERE state != 'inactive' AND type = %1 LIMIT %2, 1").arg(class_id).arg(room_row));
    query_rooms.next();
    return query_rooms.value("id").toInt();
}
