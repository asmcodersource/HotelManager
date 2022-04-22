#ifndef DATEWIDGET_H
#define DATEWIDGET_H

#include <QWidget>
#include <memory>
#include <QHeaderView>
#include <QSizePolicy>
#include <QTableView>
#include <QStandardItem>
#include <QDate>
#include <QDebug>
#include <QLayout>
#include <QWidget>


/*
  Клас що надає віджет стрічки дати.
*/
class DateWidget : public QWidget
{
    Q_OBJECT
    std::unique_ptr<QStandardItemModel> model_items_date_day;
    std::unique_ptr<QStandardItemModel> model_items_date_month;
    std::unique_ptr<QStandardItemModel> model_items_date_year;
    std::shared_ptr<QTableView> table_date_day;
    std::shared_ptr<QTableView> table_date_month;
    std::shared_ptr<QTableView> table_date_year;
    std::unique_ptr<QVBoxLayout> layout_main;

    QStandardItem* items_days;

    QStringList months = {"Січень", "Лютий", "Березень", "Квітень", "Травень", "Червень", "Липень", "Серпень", "Вересень", "Жовтень", "Листопад", "Грудень"};
    int32_t cell_day_size = 25;
    int32_t year_begin, year_end;
    int32_t left_offset;

public:
    explicit DateWidget(QWidget *parent = nullptr);
    ~DateWidget() = default;

    // метод для встановлення початкової і кінцевої дати у стрічці дати.
    void setYearRange(int32_t begin_year, int32_t end_year);
    // метод для встановлення ширини клінки з числом дня у місяці.
    void setDayCellWidth(int32_t new_width);
    // метод для встановлення лівого відступу від батьківського віджету
    void setLeftOffset(int32_t offset /* в пікселях */ );

signals:

private:
    void initializeDateModel();
};

#endif // DATEWIDGET_H
