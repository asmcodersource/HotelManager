#include "CentralWidget.h"

CentralWidget::CentralWidget(QWidget *parent )  : QWidget(parent){
    // Створення об'єктів віджету
    main_layout = std::unique_ptr<QHBoxLayout>(new QHBoxLayout);
    stacked_widget = std::unique_ptr<QStackedWidget>(new QStackedWidget);
    side_bar_widget = std::unique_ptr<SideBarWidget>(new SideBarWidget);
    rooms_categories = std::unique_ptr<TableData>(new TableData);
    rooms_list = std::unique_ptr<TableDataRooms>(new TableDataRooms);
    calendar_widget = std::unique_ptr<CalendarScrollWidget>(new CalendarScrollWidget);
    guests_list = std::unique_ptr<TableDataGuests>(new TableDataGuests(calendar_widget->calendarWidget()));

    QStringList headers_categories;
    headers_categories << "id";
    headers_categories << "Назва";
    headers_categories << "Опис";
    headers_categories << "Місця";
    headers_categories << "Ціна";
    rooms_categories->setQueryInfo("RoomsCategory", "SELECT * FROM RoomsCategory WHERE state != 'inactive'", headers_categories, 4);
    rooms_categories->setEditor(new RoomsCategoriesEditor);
    rooms_categories->setHeaderText("Категорії житла");

    QStringList headers_rooms;
    headers_rooms << "id";
    headers_rooms << "Назва";
    headers_rooms << "Опис";
    headers_rooms << "Клас";
    rooms_list->setQueryInfo("Rooms", "SELECT * FROM Rooms WHERE state != 'inactive'", headers_rooms, 3);
    rooms_list->setEditor(new RoomsEditor);
    rooms_list->setHeaderText("Список житла");

    QStringList headers_guests;
    headers_guests << "id";
    headers_guests << "Ім'я";
    headers_guests << "Статус";
    headers_guests << "Інформація";
    headers_guests << "Перший день";
    headers_guests << "Останній день";
    headers_guests << "До оплати";
    headers_guests << "Кімната";
    headers_guests << "Контакти";
    headers_guests << "Адреса";
    guests_list->setQueryInfo("Guests", "SELECT * FROM Guests WHERE state_id != 5 AND state != 'inactive'", headers_guests, 9);
    guests_list->setEditor(new GuestsEditor);
    guests_list->setHeaderText("Список клієнтів");
    calendar_widget->calendarWidget()->updateData();


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
    sideBarWidget()->addButton(":/icons/icons/calendar.png", "Календар", true);
    addDisplayWidget(calendar_widget.get());
    sideBarWidget()->addButton(":/icons/icons/list.png", "Список клієнтів", true);
    addDisplayWidget(guests_list.get());
    sideBarWidget()->addButton(":/icons/icons/room_clases.png", "Класи житла", true);
    addDisplayWidget(rooms_categories.get());
    sideBarWidget()->addButton(":/icons/icons/editor.png", "Список житла", true);
    addDisplayWidget(rooms_list.get());


    stacked_widget->addWidget(calendar_widget.get());
    stacked_widget->setCurrentWidget(rooms_categories.get());

    connect(sideBarWidget(), &SideBarWidget::signal_checkedChange, this, &CentralWidget::displayWidgetByIndex);
    connect(rooms_list.get(), &TableData::dataUpdated, this, &CentralWidget::blockCalendar);
    connect(rooms_categories.get(), &TableData::dataUpdated, this, &CentralWidget::blockCalendar);
    connect(guests_list.get(), &TableData::dataUpdated, this, &CentralWidget::updateCalendar);
}

CentralWidget::~CentralWidget(){

}

void CentralWidget::addDisplayWidget(QWidget* widget){
    display_widgets.push_back(widget);
    stacked_widget->addWidget(widget);
}

void CentralWidget::displayWidgetByIndex(int32_t index){
    rooms_list->updateData();
    rooms_categories->updateData();
    guests_list->updateData();
    calendar_widget->calendarWidget()->updateData();
    if( display_widgets.size() > index )
        stacked_widget->setCurrentWidget(display_widgets[index]);
}

SideBarWidget* CentralWidget::sideBarWidget(){
    return side_bar_widget.get();
}

void CentralWidget::resizeEvent(QResizeEvent *event){
   side_bar_widget->setGeometry(0,0, side_bar_widget->width(), height());
   main_layout->setContentsMargins(side_bar_widget->minimumWidth(),0,0,0);
   main_layout->update();
}

void CentralWidget::blockCalendar(){
    auto* button = side_bar_widget->getButton(1);
    button->setEnabled(false);
}

void CentralWidget::updateCalendar(){

}

