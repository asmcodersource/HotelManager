#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <memory>
#include <QWidget>
#include <QColor>
#include <vector>
#include "datewidget.h"
#include "roomstabwidget.h"
#include "dateuserinformerwidget.h"

class RoomsTabWidget;

class CalendarWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(quint16 width_left_side READ leftSideWidth WRITE setLeftSideWidth NOTIFY signal_leftSideWidthChange )
    std::unique_ptr<QVBoxLayout> main_layout;
    std::unique_ptr<DateWidget> date_widget;
    std::unique_ptr<DateUserInformerWidget> date_user_inform_widget;
    std::vector<RoomsTabWidget*> tabs_widgets;
    quint16 left_offset = 0;
    quint16 dateline_height = 0;

public:
    explicit CalendarWidget(QWidget *parent = nullptr);
    ~CalendarWidget() = default;

    quint16 leftSideWidth();
    void setLeftSideWidth(quint16  new_width);

public slots:
    void widgetScrolledSlot(int32_t horizontal_scroll, int32_t vertical_scroll);

signals:
    void signal_widgetScrolled(int32_t horizontal_scroll, int32_t vertical_scroll);
    void signal_leftSideWidthChange(QColor value);
};

#endif // CALENDARWIDGET_H
