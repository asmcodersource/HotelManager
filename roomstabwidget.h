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
#include <QSpacerItem>
#include <QLayout>
#include <QLabel>
#include <QDebug>
#include "calendarwidget.h"

class RoomsTabWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(quint16 day_cell_width READ dayCellWidth    WRITE setDayCellWidth  )
    Q_PROPERTY(quint16 day_cell_height READ dayCellHeight  WRITE setDayCellHeight )

    std::unique_ptr<QVBoxLayout> main_layout;
    std::unique_ptr<QHBoxLayout> rooms_header_layout;
    std::unique_ptr<QHBoxLayout> rooms_layout;
    std::unique_ptr<QLabel> rooms_type_name;
    std::unique_ptr<QSpacerItem> spacer_for_header_layout;

    std::unique_ptr<QTableView> rooms_table;
    std::unique_ptr<QStandardItemModel> rooms_model;
    std::unique_ptr<QTableView> rooms_organize_table;
    std::unique_ptr<QStandardItemModel> rooms_organize_model;

    int32_t year_begin, year_end;
    int32_t room_table_width = 230;
    QStringList rooms_list;
    int32_t cell_day_width = 25, cell_day_height = 25;

    std::vector<QStandardItem*> day_items_arrays;

public:
    explicit RoomsTabWidget(int32_t year_begin = 2022, int32_t year_end = 2023, QWidget *parent = nullptr);
    void exampleData();

    void addRoom(QString room_name);
    // метод для встановлення ширини клінки з числом дня у місяці.
    void setDayCellWidth(qint16 new_value);
    // метод для встановлення лівого відступу від батьківського віджету
    void setLeftOffset(int32_t offset /* в пікселях */ );

    qint16 dayCellWidth();
    qint16 dayCellHeight();
    void setDayCellHeight(qint16 new_value);

public slots:
    void widgetScrolledSlot(int32_t horizontal_scroll, int32_t vertical_scroll);

signals:


private:
    void addDayCells();

};

#endif // ROOMSTABWIDGET_H
