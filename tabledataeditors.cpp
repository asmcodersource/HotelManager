#include "tabledataeditors.h"

//-----------------------------------------------------------------------------------------------

DataEditor::DataEditor( QWidget* parent ): QWidget(parent){}

//-----------------------------------------------------------------------------------------------

RoomsCategoriesEditor::RoomsCategoriesEditor( QWidget* parent ): DataEditor(parent){
    main_layout = std::unique_ptr<QVBoxLayout>(new QVBoxLayout);
    buttons_layout = std::unique_ptr<QHBoxLayout>(new QHBoxLayout);
    category_name = std::unique_ptr<QLineEdit>(new QLineEdit);
    category_desc = std::unique_ptr<QTextEdit>(new QTextEdit);
    category_places = std::unique_ptr<QLineEdit>(new QLineEdit);
    category_price = std::unique_ptr<QLineEdit>(new QLineEdit);
    button_confirm = std::unique_ptr<QPushButton>(new QPushButton);
    button_cancel = std::unique_ptr<QPushButton>(new QPushButton);


    button_confirm->setText("Зберегти");
    button_cancel->setText("Відмінити");
    buttons_layout->setSpacing(5);
    buttons_layout->addWidget(button_cancel.get());
    buttons_layout->addWidget(button_confirm.get());
    buttons_layout->setContentsMargins(15,15,15,5);

    setWindowTitle("Редагування класу кімнат");
    main_layout->addWidget(new QLabel("Назва класу"));
    main_layout->addWidget(category_name.get());
    main_layout->addWidget(new QLabel("Опис класу"));
    main_layout->addWidget(category_desc.get());
    main_layout->addWidget(new QLabel("Місця"));
    main_layout->addWidget(category_places.get());
    main_layout->addWidget(new QLabel("Ціна за день"));
    main_layout->addWidget(category_price.get());
    main_layout->addItem(buttons_layout.get());
    setLayout(main_layout.get());

    connect(button_confirm.get(), &QPushButton::pressed, this, &RoomsCategoriesEditor::slot_buttonConfirmPressed);
    connect(button_cancel.get(), &QPushButton::pressed, this, &RoomsCategoriesEditor::slot_buttonCancelPressed);
    connect(category_name.get(), &QLineEdit::textChanged, this, &RoomsCategoriesEditor::slot_valueChanged);
    connect(category_price.get(), &QLineEdit::textChanged, this, &RoomsCategoriesEditor::slot_valueChanged);
    connect(category_places.get(), &QLineEdit::textChanged, this, &RoomsCategoriesEditor::slot_valueChanged);
    connect(category_desc.get(), &QTextEdit::textChanged, this, &RoomsCategoriesEditor::slot_valueChanged);
}

void RoomsCategoriesEditor::setData(QStandardItemModel* model, int32_t row){
    RoomsCategoriesEditor::model = model;
    RoomsCategoriesEditor::row = row;
    category_name->setText(model->item(row, 1)->text());
    category_desc->setText(model->item(row, 2)->text());
    category_places->setText(model->item(row, 3)->text());
    category_price->setText(model->item(row, 4)->text());
}

void RoomsCategoriesEditor::slot_valueChanged(){
    bool bad_values = false;
    bad_values |= category_name->text().isEmpty();
    bad_values |= category_desc->toPlainText().isEmpty();
    bad_values |= category_price->text().isEmpty();
    bad_values |= category_places->text().isEmpty();
    if( bad_values ){
        button_confirm->setEnabled(false);
        return;
    }
    bool places_bad_value = false, price_bad_value = false;
    category_places->text().toInt(&places_bad_value);
    category_price->text().toDouble(&price_bad_value);
    if( !(places_bad_value && price_bad_value) ){
        button_confirm->setEnabled(false);
        return;
    }
    button_confirm->setEnabled(true);
}

void RoomsCategoriesEditor::slot_buttonConfirmPressed(){
    int32_t id =  model->item(row, 0)->text().toInt();
    model->item(row, 1)->setText(category_name->text());
    model->item(row, 2)->setText(category_desc->toPlainText());
    model->item(row, 3)->setText(category_places->text());
    model->item(row, 4)->setText(category_price->text());
    QSqlQuery query_name(QString("UPDATE RoomsCategory\n SET name = '%1'\n WHERE id =  %2").arg(model->item(row, 1)->text()).arg(id));
    QSqlQuery query_desc(QString("UPDATE RoomsCategory\n SET description = '%1'\n WHERE id =  %2").arg(model->item(row, 2)->text()).arg(id));
    QSqlQuery query_places(QString("UPDATE RoomsCategory\n SET places = '%1'\n WHERE id =  %2").arg(model->item(row, 3)->text()).arg(id));
    QSqlQuery query_price(QString("UPDATE RoomsCategory\n SET price = '%1'\n WHERE id =  %2").arg(model->item(row, 4)->text()).arg(id));
    hide();
}

void RoomsCategoriesEditor::slot_buttonCancelPressed(){
    hide();
}


//-----------------------------------------------------------------------------------------------

RoomsEditor::RoomsEditor( QWidget* parent ): DataEditor(parent){
    main_layout = std::unique_ptr<QVBoxLayout>(new QVBoxLayout);
    buttons_layout = std::unique_ptr<QHBoxLayout>(new QHBoxLayout);
    room_name = std::unique_ptr<QLineEdit>(new QLineEdit);
    room_desc = std::unique_ptr<QTextEdit>(new QTextEdit);
    button_confirm = std::unique_ptr<QPushButton>(new QPushButton);
    button_cancel = std::unique_ptr<QPushButton>(new QPushButton);
    room_type = std::unique_ptr<QComboBox>(new QComboBox);


    button_confirm->setText("Зберегти");
    button_cancel->setText("Відмінити");
    buttons_layout->setSpacing(5);
    buttons_layout->addWidget(button_cancel.get());
    buttons_layout->addWidget(button_confirm.get());
    buttons_layout->setContentsMargins(15,15,15,5);

    setWindowTitle("Редагування даних кімнати");
    main_layout->addWidget(new QLabel("Назва кімнати"));
    main_layout->addWidget(room_name.get());
    main_layout->addWidget(new QLabel("Опис кімнати"));
    main_layout->addWidget(room_desc.get());
    main_layout->addWidget(new QLabel("Клас кімнати"));
    main_layout->addWidget(room_type.get());
    main_layout->addItem(buttons_layout.get());
    setLayout(main_layout.get());

    connect(button_confirm.get(), &QPushButton::pressed, this, &RoomsEditor::slot_buttonConfirmPressed);
    connect(button_cancel.get(), &QPushButton::pressed, this, &RoomsEditor::slot_buttonCancelPressed);
    connect(room_name.get(), &QLineEdit::textChanged, this, &RoomsEditor::slot_valueChanged);
    connect(room_desc.get(), &QTextEdit::textChanged, this, &RoomsEditor::slot_valueChanged);
}

void RoomsEditor::setData(QStandardItemModel* model, int32_t row){
    RoomsEditor::model = model;
    RoomsEditor::row = row;
    room_name->setText(model->item(row, 1)->text());
    room_desc->setText(model->item(row, 2)->text());
    room_type->clear();
    QSqlQuery query("SELECT * FROM RoomsCategory WHERE state != 'inactive'");
    while(query.next()){
        room_type->addItem(query.value("name").toString());
    }

}

void RoomsEditor::slot_valueChanged(){
    bool bad_values = false;
    bad_values |= room_name->text().isEmpty();
    bad_values |= room_desc->toPlainText().isEmpty();
    if( bad_values ){
        button_confirm->setEnabled(false);
        return;
    }
    button_confirm->setEnabled(true);
}

void RoomsEditor::slot_buttonConfirmPressed(){
    int32_t id =  model->item(row, 0)->text().toInt();
    model->item(row, 1)->setText(room_name->text());
    model->item(row, 2)->setText(room_desc->toPlainText());
    qDebug() << room_type->currentText();
    QSqlQuery query(QString("SELECT * FROM RoomsCategory WHERE name = '%1'").arg(room_type->currentText()));
    query.next();
    int32_t room_type_id = query.value("id").toInt();
    QString class_name = query.value("name").toString();
    model->item(row, 3)->setText(class_name);
    QSqlQuery query_name(QString("UPDATE Rooms\n SET name = '%1'\n WHERE id =  %2").arg(model->item(row, 1)->text()).arg(id));
    QSqlQuery query_desc(QString("UPDATE Rooms\n SET desc = '%1'\n WHERE id =  %2").arg(model->item(row, 2)->text()).arg(id));
    QSqlQuery query_type(QString("UPDATE Rooms\n SET type = '%1'\n WHERE id =  %2").arg(room_type_id).arg(id));
    hide();
}

void RoomsEditor::slot_buttonCancelPressed(){
    hide();
}

//-----------------------------------------------------------------------------------------------


GuestsEditor::GuestsEditor( QWidget* parent ): DataEditor(parent){
    main_layout = std::unique_ptr<QVBoxLayout>(new QVBoxLayout);
    buttons_layout = std::unique_ptr<QHBoxLayout>(new QHBoxLayout);
    guest_name = std::unique_ptr<QLineEdit>(new QLineEdit);
    guest_desc = std::unique_ptr<QTextEdit>(new QTextEdit);
    guest_room_id = std::unique_ptr<QComboBox>(new QComboBox);
    guest_state_id = std::unique_ptr<QComboBox>(new QComboBox);
    guest_contacts = std::unique_ptr<QTextEdit>(new QTextEdit);
    guest_adress = std::unique_ptr<QLineEdit>(new QLineEdit);
    guest_debt = std::unique_ptr<QLineEdit>(new QLineEdit);
    guest_first_day = std::unique_ptr<QLineEdit>(new QLineEdit);
    guest_last_day = std::unique_ptr<QLineEdit>(new QLineEdit);
    button_confirm = std::unique_ptr<QPushButton>(new QPushButton);
    button_cancel = std::unique_ptr<QPushButton>(new QPushButton);

    button_confirm->setText("Зберегти");
    button_cancel->setText("Відмінити");
    buttons_layout->setSpacing(5);
    buttons_layout->addWidget(button_cancel.get());
    buttons_layout->addWidget(button_confirm.get());
    buttons_layout->setContentsMargins(15,15,15,5);

    guest_first_day->setPlaceholderText("yyyy-MM-dd");
    guest_last_day->setPlaceholderText("yyyy-MM-dd");

    setWindowTitle("Редагування даних кімнати");
    main_layout->addWidget(new QLabel("Ім'я"));
    main_layout->addWidget(guest_name.get());
    main_layout->addWidget(new QLabel("Інформація"));
    main_layout->addWidget(guest_desc.get());
    main_layout->addWidget(new QLabel("Контакти"));
    main_layout->addWidget(guest_contacts.get());
    main_layout->addWidget(new QLabel("Адреса"));
    main_layout->addWidget(guest_adress.get());
    main_layout->addWidget(new QLabel("Перший день"));
    main_layout->addWidget(guest_first_day.get());
    main_layout->addWidget(new QLabel("Останній день"));
    main_layout->addWidget(guest_last_day.get());
    main_layout->addWidget(new QLabel("Статус"));
    main_layout->addWidget(guest_state_id.get());
    main_layout->addWidget(new QLabel("Кімната"));
    main_layout->addWidget(guest_room_id.get());
    main_layout->addWidget(new QLabel("До оплати"));
    main_layout->addWidget(guest_debt.get());


    main_layout->addItem(buttons_layout.get());
    setLayout(main_layout.get());
    date_validator.setRegularExpression(QRegularExpression("[0-9]{4}-[0-9]{2}-[0-9]{2}"));

    connect(button_confirm.get(), &QPushButton::pressed, this, &GuestsEditor::slot_buttonConfirmPressed);
    connect(button_cancel.get(), &QPushButton::pressed, this, &GuestsEditor::slot_buttonCancelPressed);
    connect(guest_debt.get(), &QLineEdit::textChanged, this, &GuestsEditor::slot_valueChanged);
}

void GuestsEditor::setData(QStandardItemModel* model, int32_t row){
    GuestsEditor::model = model;
    GuestsEditor::row = row;
    QSqlQuery query_states("SELECT * FROM GuestStates");
    guest_state_id->clear();
    while(query_states.next()){
        guest_state_id->addItem(query_states.value("desc").toString());
    }

    guest_room_id->clear();
    QSqlQuery query_rooms("SELECT * FROM Rooms WHERE state != 'inactive'");
    while(query_rooms.next()){
        guest_room_id->addItem(query_rooms.value("name").toString());
    }
    guest_name->setText(model->item(row, 1)->text());
    guest_room_id->setCurrentText(model->item(row, 7)->text());
    guest_state_id->setCurrentText(model->item(row, 2)->text());
    guest_first_day->setText(model->item(row, 4)->text());
    guest_last_day->setText(model->item(row, 5)->text());
    guest_desc->setText(model->item(row, 3)->text());
    guest_debt->setText(model->item(row, 6)->text());
    guest_contacts->setText(model->item(row, 8)->text());
    guest_adress->setText(model->item(row, 9)->text());
}

void GuestsEditor::slot_valueChanged(){
    QString debt = guest_debt->text();
    bool wrong_values = true;
    debt.toDouble(&wrong_values);
    if( wrong_values == false ){
        button_confirm->setEnabled(false);
        return;
    }

    button_confirm->setEnabled(true);
}

void GuestsEditor::slot_buttonConfirmPressed(){
    int32_t id =  model->item(row, 0)->text().toInt();
    QString date_begin = guest_first_day->text();
    QString date_end = guest_last_day->text();
    QString name = guest_name->text();
    QString state = guest_state_id->currentText();
    QString room = guest_room_id->currentText();
    QString desc = guest_desc->toPlainText();
    QString contacts = guest_contacts->toPlainText();
    QString debt = guest_debt->text();
    QString address = guest_adress->text();

    model->item(row, 1)->setText(name);
    model->item(row, 2)->setText(state);
    QSqlQuery query_states(QString("SELECT * FROM GuestStates WHERE id = %1").arg(guest_state_id->currentIndex() + 1));
    if( query_states.next() ){
        QColor color;
        color.setNamedColor(query_states.value("color").toString());
        model->item(row, 2)->setBackground(color);
    }
    model->item(row, 3)->setText(desc);
    model->item(row, 4)->setText(date_begin);
    model->item(row, 5)->setText(date_end);
    model->item(row, 6)->setText(debt);
    model->item(row, 7)->setText(room);
    model->item(row, 8)->setText(contacts);
    model->item(row, 9)->setText(address);

    QSqlQuery query_name(QString("UPDATE Guests\n SET name = '%1'\n WHERE id =  %2").arg(model->item(row, 1)->text()).arg(id));
    QSqlQuery query_state(QString("UPDATE Guests\n SET state_id = '%1'\n WHERE id =  %2").arg(guest_state_id->currentIndex() + 1).arg(id));
    QSqlQuery query_desc(QString("UPDATE Guests\n SET description = '%1'\n WHERE id =  %2").arg(model->item(row, 3)->text()).arg(id));
    QSqlQuery query_date_begin(QString("UPDATE Guests\n SET begin_date = '%1'\n WHERE id =  %2").arg(model->item(row, 4)->text()).arg(id));
    QSqlQuery query_date_end(QString("UPDATE Guests\n SET end_date = '%1'\n WHERE id =  %2").arg(model->item(row, 5)->text()).arg(id));
    QSqlQuery query_room(QString("UPDATE Guests\n SET room_id = '%1'\n WHERE id =  %2").arg(guest_room_id->currentIndex() + 1).arg(id));
    QSqlQuery query_debt(QString("UPDATE Guests\n SET debt = %1\n WHERE id =  %2").arg(model->item(row, 6)->text()).arg(id));
    QSqlQuery query_contacts(QString("UPDATE Guests\n SET contacts = '%1'\n WHERE id =  %2").arg(model->item(row, 8)->text()).arg(id));
    QSqlQuery query_address(QString("UPDATE Guests\n SET address = '%1'\n WHERE id =  %2").arg(model->item(row, 9)->text()).arg(id));
    hide();
}

void GuestsEditor::slot_buttonCancelPressed(){
    hide();
}
