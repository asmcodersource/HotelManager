#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <memory>
#include <QWidget>
#include <QColor>
#include <QPainter>
#include <vector>
#include <QDateTime>
#include <QDate>
#include "datewidget.h"
#include "roomstabwidget.h"
#include "dateuserinformerwidget.h"
#include "guestwidget.h"
#include <QItemSelectionModel>
#include <QSqlDatabase>
#include <QSqlQuery>

class RoomsTabWidget;

class CalendarWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(quint16 width_left_side READ leftSideWidth WRITE setLeftSideWidth NOTIFY signal_leftSideWidthChange )
    std::unique_ptr<QVBoxLayout> main_layout;
    std::unique_ptr<DateWidget> date_widget;
    std::unique_ptr<DateUserInformerWidget> date_user_inform_widget;
    std::vector<RoomsTabWidget*> tabs_widgets;
    std::vector<GuestWidget*> guest_widgets;
    quint16 left_offset = 0;
    quint16 dateline_height = 0;
    QString first_line_date;
    RoomsTabWidget* last_selection_tab = nullptr;
    int32_t room_row;
    QDate first_day, last_day;
    QDate calendar_first_day;

public:
    explicit CalendarWidget(QWidget *parent = nullptr);
    ~CalendarWidget() = default;

    quint16 leftSideWidth();
    void setLeftSideWidth(quint16  new_width);
    void paintEvent(QPaintEvent* e) override;
    QDate firstDayRange();
    QDate lastDayRange();
    int32_t selectedRoomIndex();

public slots:
    void widgetScrolledSlot(int32_t horizontal_scroll, int32_t vertical_scroll);
    void newSelectedRange(int32_t room_row, int32_t first_day, int32_t last_day);
    void updateData();

signals:
    void signal_widgetScrolled(int32_t horizontal_scroll, int32_t vertical_scroll);
    void signal_leftSideWidthChange(QColor value);
};

#endif // CALENDARWIDGET_H
