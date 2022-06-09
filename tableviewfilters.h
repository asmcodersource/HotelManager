#ifndef TABLEVIEWFILTERS_H
#define TABLEVIEWFILTERS_H
#include <QWidget>
#include <QTableView>
#include <QMouseEvent>
#include <QStandardItemModel>
#include <QApplication>
#include <QDebug>


class ListWidgetBaseFilter: public QObject{

protected:
    QTableView* table_view;

public:
    ListWidgetBaseFilter(QObject* parent = nullptr);
    virtual ~ListWidgetBaseFilter();

    virtual void setTableView(QTableView* table_view);
    virtual QTableView* getTableView();
    virtual bool eventFilter(QObject* object, QEvent* event ) = 0;

protected:
    bool isParent(QObject* child, QObject* parent);


};

class ListWidgetColumnResizeFilter: public ListWidgetBaseFilter{
    int sizing_start_size;
    QPoint sizing_mouse_position;
    QModelIndex sizing_cell_index;
    bool sizing = false;

public:
    ListWidgetColumnResizeFilter(QObject* parent = nullptr);
    bool eventFilter(QObject* object, QEvent* event ) override;
};

class ListWidgetRowResizeFilter: public ListWidgetBaseFilter{
    int sizing_start_size;
    QPoint sizing_mouse_position;
    QModelIndex sizing_cell_index;
    bool sizing = false;

public:
    ListWidgetRowResizeFilter(QObject* parent = nullptr);
    bool eventFilter(QObject* object, QEvent* event ) override;
};
#endif // TABLEVIEWFILTERS_H
