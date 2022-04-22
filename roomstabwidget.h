#ifndef ROOMSTABWIDGET_H
#define ROOMSTABWIDGET_H

#include <memory>
#include <vector>
#include <QDate>
#include <QTableView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QDebug>

class RoomsTabWidget : public QWidget
{
    Q_OBJECT
    std::unique_ptr<QVBoxLayout> main_layout;
    std::unique_ptr<QHBoxLayout> rooms_layout;
    std::unique_ptr<QLabel> rooms_type_name;

    std::unique_ptr<QTableView> rooms_table;
    std::unique_ptr<QStandardItemModel> rooms_model;
    std::unique_ptr<QTableView> rooms_organize_table;
    std::unique_ptr<QStandardItemModel> rooms_organize_model;

    int32_t year_begin, year_end;
    int32_t room_table_width = 230;
    QStringList rooms_list;
    int32_t cell_day_size = 25;

    std::vector<QStandardItem*> day_items_arrays;

public:
    explicit RoomsTabWidget(int32_t year_begin = 2022, int32_t year_end = 2023, QWidget *parent = nullptr);
    void exampleData();

    void addRoom(QString room_name);
    // метод для встановлення ширини клінки з числом дня у місяці.
    void setDayCellWidth(int32_t new_width);
signals:

private:
    void addDayCells();

};

#endif // ROOMSTABWIDGET_H
