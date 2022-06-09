#ifndef TABLEDATA_H
#define TABLEDATA_H

#include <memory>
#include <tableviewfilters.h>
#include <QVector>
#include <QPainter>
#include <QObject>
#include <QWidget>
#include <QLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QStandardItemModel>
#include <QResizeEvent>
#include <QAbstractItemDelegate>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QItemDelegate>
#include <QStandardItem>
#include <QTableView>
#include <QHeaderView>
#include <calendarwidget.h>
#include <QtSql>
#include <QDebug>
#include <scrollbar.h>
#include <tabledataeditors.h>

class RoomsCategoriesEditor;
class DataEditor;

class TableData : public QWidget
{
    Q_OBJECT
protected:
    std::unique_ptr<ScrollBar> vertical_scroll_bar;
    std::unique_ptr<ScrollBar> horizontal_scroll_bar;

    std::unique_ptr<QVBoxLayout> main_layout;
    std::unique_ptr<QHBoxLayout> buttons_layout;
    std::unique_ptr<QLabel> header;
    std::unique_ptr<QTableView> table_view;
    std::unique_ptr<QSqlDatabase> data_base;
    std::unique_ptr<QStandardItemModel> data_model;
    std::unique_ptr<DataEditor> edit_widget;
    std::unique_ptr<ListWidgetColumnResizeFilter> column_resize_filter;
    std::unique_ptr<ListWidgetRowResizeFilter> row_resize_filter;
    QStringList headers;
    QVector<QPushButton*> buttons;
    QVector<QStandardItem*> objects;
    QString table_name, select_query;
    int32_t columns;

public:
    explicit TableData(QWidget *parent = nullptr);
    void setHeaderText(QString text);
    void setEditor(DataEditor* editor_widget);
    virtual void setQueryInfo( QString table_name, QString select_query, QStringList header_titles, int32_t columns);
    virtual void updateData();

    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

public slots:
    virtual void slot_createNewRow();
    void slot_editSelectedRow();
    void slot_deleteSelectedRow();


signals:
    void dataUpdated();
};

//-----------------------------------------------------------------------------------------------
class TableDataRooms : public TableData{
    Q_OBJECT

public:
    TableDataRooms(QWidget *parent = nullptr);
    void updateData() override;

public slots:
};

//-----------------------------------------------------------------------------------------------
class TableDataGuests : public TableData{
    Q_OBJECT
    CalendarWidget* calendar;

public:
    TableDataGuests(CalendarWidget* calendar_ptr, QWidget *parent = nullptr);
    void slot_createNewRow() override;
    void updateData() override;

public slots:

};
//-----------------------------------------------------------------------------------------------


#endif // TABLEDATA_H
