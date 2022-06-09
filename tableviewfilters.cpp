#include "tableviewfilters.h"

ListWidgetBaseFilter::ListWidgetBaseFilter(QObject* parent ) : QObject(parent){}
ListWidgetBaseFilter::~ListWidgetBaseFilter() = default;

void ListWidgetBaseFilter::setTableView(QTableView* table_view){
    this->table_view = table_view;
}

QTableView* ListWidgetBaseFilter::getTableView(){
    return table_view;
}

bool ListWidgetBaseFilter::isParent(QObject* child, QObject* parent){
    while( child != parent ){
        child = child->parent();
        if( child == nullptr )
            return false;
    }
    return true;
}

//------------------------------------------------------------------------

ListWidgetColumnResizeFilter::ListWidgetColumnResizeFilter(QObject* parent ): ListWidgetBaseFilter(parent){

}

bool ListWidgetColumnResizeFilter::eventFilter(QObject* object, QEvent* event ){
    if( isParent(object, table_view) && ( event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonPress )){
        QMouseEvent *m = static_cast<QMouseEvent*>(event);
        QPoint p = m->pos();
        QRect r_left = table_view->visualRect(table_view->indexAt(p));
       if (abs(p.x() - r_left.right()) < 3 ){
           if( event->type() == QEvent::MouseButtonPress ){
               sizing_cell_index = table_view->indexAt(p);
               sizing_start_size = table_view->columnWidth(sizing_cell_index.column());
               sizing_mouse_position = p;
               sizing = true;
               return true;
           } else {
               table_view->setCursor(Qt::SizeHorCursor);
           }
       } else if( table_view->cursor() == Qt::SizeHorCursor && m->button() != Qt::LeftButton){
           table_view->unsetCursor();
       }
    }
    if( isParent(object, table_view) && ( event->type() == QEvent::MouseMove && sizing == true  )){
        QMouseEvent *m = static_cast<QMouseEvent*>(event);
        QPoint p = m->pos();
        int offset = sizing_mouse_position.x() - p.x();
        if( sizing_start_size - offset > 0 )
            table_view->setColumnWidth(sizing_cell_index.column(), sizing_start_size - offset);
        return true;
    }
    if( isParent(object, table_view) && ( event->type() == QEvent::MouseButtonRelease && sizing == true  )){
        sizing = false;
        table_view->unsetCursor();
    }
    return false;
}

//------------------------------------------------------------------------


ListWidgetRowResizeFilter::ListWidgetRowResizeFilter(QObject* parent ): ListWidgetBaseFilter(parent){

}

bool ListWidgetRowResizeFilter::eventFilter(QObject* object, QEvent* event ){
    if( isParent(object, table_view) && ( event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonPress )){
        QMouseEvent *m = static_cast<QMouseEvent*>(event);
        QPoint p = m->pos();
        QRect r_first = table_view->visualRect(table_view->indexAt(p));
       if (abs(p.y() - r_first.bottom()) < 3 ){
           if( event->type() == QEvent::MouseButtonPress ){
               sizing_cell_index = table_view->indexAt(p);
               sizing_start_size = table_view->rowHeight(table_view->indexAt(p).row());
               sizing_mouse_position = p;
               sizing = true;
               return true;
           } else {
                table_view->setCursor(Qt::SizeVerCursor);
           }
       } else if( table_view->cursor() == Qt::SizeVerCursor && m->button() != Qt::LeftButton ){
           table_view->unsetCursor();
       }
    }
    if( isParent(object, table_view) && ( event->type() == QEvent::MouseMove && sizing == true  )){
        QMouseEvent *m = static_cast<QMouseEvent*>(event);
        QPoint p = m->pos();
        int offset = sizing_mouse_position.y() - p.y();
        if( sizing_start_size - offset > 0 )
            table_view->setRowHeight(sizing_cell_index.row(), sizing_start_size - offset);
        return true;
    }
    if( isParent(object, table_view) && ( event->type() == QEvent::MouseButtonRelease && sizing == true  )){
        sizing = false;
        table_view->unsetCursor();
    }
    return false;
}
