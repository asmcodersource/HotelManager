#include "startupwidget.h"

StartupWidget::StartupWidget(QWidget *parent) : QWidget{parent}{
    data_base = std::unique_ptr<QSqlDatabase>(new QSqlDatabase());
    load = new QPushButton("Обрати файл");
    create = new QPushButton("Створити");
    ok = new QPushButton("Відкрити");
    cancel = new QPushButton("Закрити");
    file_path = new QLineEdit();

    file_path->setEnabled(false);
    file_path->setPlaceholderText("Шлях до файлу");
    ok->setEnabled(false);

    main_layout = new QVBoxLayout();
    file_path_layout = new QHBoxLayout();
    buttons_layout = new QHBoxLayout;
    file_path_layout->setSpacing(5);
    file_path_layout->addWidget(file_path);
    file_path_layout->addWidget(load);
    buttons_layout->setSpacing(5);
    buttons_layout->addWidget(cancel);
    buttons_layout->addWidget(create);
    buttons_layout->addWidget(ok);
    buttons_layout->setContentsMargins(15,0,15,0);
    main_layout->addItem(file_path_layout);
    main_layout->addItem(buttons_layout);
    auto* label = new QLabel("У програмі використані графічні зображення з сайту icons8.ru");
    label->setAlignment(Qt::AlignCenter);
    main_layout->addStretch(100);
    main_layout->addWidget(label);
    setLayout(main_layout);
    setWindowTitle("Оберіть базу даних");

    connect(cancel, &QPushButton::clicked, this, [&, this](){
       QApplication::exit();
    });
    connect(ok, &QPushButton::clicked, this, &StartupWidget::openFile);
    connect(load, &QPushButton::clicked, this, [this](){
       static QFileDialog* file_dialog = new QFileDialog();
       file_dialog->setWindowTitle("Оберіть файл бази даних, з яким бажаєте працювати");
       file_dialog->setFileMode(QFileDialog::ExistingFile);
       file_dialog->setLabelText(QFileDialog::DialogLabel::Accept, "Відкрити");
       file_dialog->setLabelText(QFileDialog::DialogLabel::Reject, "Відміна");
       file_dialog->setLabelText(QFileDialog::DialogLabel::FileType, "Файли формату");
       file_dialog->setLabelText(QFileDialog::DialogLabel::FileName, "Ім'я файлу");
       file_dialog->setNameFilter("Файли формату баз даних (*.db)");
       file_dialog->show();
       connect(file_dialog, &QFileDialog::fileSelected, this, [this](QString file_name){
           newPathSelected(file_name);
       });
    });

    connect(create, &QPushButton::clicked, this, [this](){
       static QFileDialog* file_dialog = new QFileDialog();
       file_dialog->setWindowTitle("Вкажіть назву файлу що буде створюватись");
       file_dialog->setFileMode(QFileDialog::AnyFile);
       file_dialog->setLabelText(QFileDialog::DialogLabel::Accept, "Створити");
       file_dialog->setLabelText(QFileDialog::DialogLabel::Reject, "Відміна");
       file_dialog->setLabelText(QFileDialog::DialogLabel::FileType, "Файли формату");
       file_dialog->setLabelText(QFileDialog::DialogLabel::FileName, "Ім'я файлу");
       file_dialog->setNameFilter("Файли формату баз даних (*.db)");
       file_dialog->show();
       connect(file_dialog, &QFileDialog::fileSelected, this, [this](QString file_name){
           createNewFile(file_name);
       });
    });
}

void StartupWidget::newPathSelected(QString new_path){
    file_path->setText(new_path);
    ok->setEnabled(true);
}

void StartupWidget::createNewFile(QString new_path){
    new_path.append(".db");
    auto data_base = QSqlDatabase::addDatabase("QSQLITE");
    data_base.setDatabaseName(new_path);
    if (!data_base.open()) {
             qDebug()<<data_base.lastError().text();
        }
    qDebug() << "start to creating bullshit";
    QSqlQuery create_tables;
    create_tables.prepare(R"(CREATE TABLE IF NOT EXISTS "GuestStates" (
                       "id"	INTEGER NOT NULL DEFAULT 1 UNIQUE,
                       "desc"	TEXT,
                       "color"	TEXT,
                       PRIMARY KEY("id" AUTOINCREMENT)
                        ))");
    create_tables.exec();
    create_tables.prepare(R"(CREATE TABLE IF NOT EXISTS "Guests" (
                        "id"	INTEGER NOT NULL DEFAULT 0 UNIQUE,
                        "name"	TEXT DEFAULT 'Ім''я гостя',
                        "state_id"	INTEGER DEFAULT 4,
                        "description"	TEXT DEFAULT 'Інформація',
                        "begin_date"	TEXT DEFAULT 'Перший день',
                        "end_date"	TEXT DEFAULT 'Останній день',
                        "debt"	INTEGER DEFAULT 0,
                        "room_id"	INTEGER DEFAULT 0,
                        "contacts"	TEXT DEFAULT 'Номер гостя',
                        "address"	TEXT DEFAULT 'Адреса гостя',
                        "state"	TEXT DEFAULT 'active',
                        PRIMARY KEY("id" AUTOINCREMENT)
                    ))");
    create_tables.exec();
    create_tables.prepare(R"(CREATE TABLE IF NOT EXISTS "Rooms" (
                        "id"	INTEGER NOT NULL DEFAULT 0 UNIQUE,
                        "name"	TEXT DEFAULT 'Нова кімната',
                        "desc"	TEXT DEFAULT 'Опис кімнати',
                        "type"	INTEGER NOT NULL DEFAULT 0,
                        "state"	TEXT DEFAULT 'active',
                        PRIMARY KEY("id")
                    ))");
    create_tables.exec();
    create_tables.prepare(R"(CREATE TABLE IF NOT EXISTS "RoomsCategory" (
                        "id"	INTEGER NOT NULL UNIQUE,
                        "name"	TEXT DEFAULT 'Назва класу',
                        "description"	TEXT DEFAULT 'Опис класу',
                        "places"	INTEGER DEFAULT 0,
                        "price"	INTEGER DEFAULT 0,
                        "state"	TEXT DEFAULT 'active',
                        PRIMARY KEY("id")
                    ))");
    create_tables.exec();
    QSqlQuery create_guest_states;
    create_guest_states.exec(R"(INSERT INTO GuestStates (
                id,
                desc,
                color
            )
            VALUES
            (
                1,
                'заселення',
                '#237a32'
            ),
            (
                2,
                'виселення',
                '#631921'
            ),
            (
                3,
                'проживання',
                '#66641a'
            ),
            (
                4,
                'резервування',
                '#2b3478'
            ),
            (
                5,
                'закрито',
                '#523063'
            );)");
    newPathSelected(new_path);
    data_base.close();
    }

void StartupWidget::openFile(){
    *data_base = QSqlDatabase::addDatabase("QSQLITE");
    data_base->setDatabaseName(file_path->text());
    data_base->open();
    hide();
    central_widget = std::unique_ptr<CentralWidget>(new CentralWidget);
    main_window = std::unique_ptr<MainWindow>(new MainWindow);
    main_window->setCentralWidget(central_widget.get());
    main_window->show();
}
