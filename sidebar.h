#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QDebug>
#include <QTimer>
#include <QMainWindow>
#include <QSpacerItem>
#include <QPushButton>
#include <QAbstractButton>
#include <QVector>
#include <QImage>
#include <QIcon>
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QStyleOption>
#include <QLayout>
#include <QPropertyAnimation>
#include <memory>

class SideBarButtonWidget: public QPushButton{
    Q_OBJECT
    Q_PROPERTY(quint16 minimized_width READ minimizedWidth WRITE setMinimizedWidth )
    Q_PROPERTY(double icon_scale READ iconScale WRITE setIconScale )

    std::unique_ptr<QHBoxLayout> main_layout;
    std::unique_ptr<QSpacerItem> spacer_item;
    std::unique_ptr<QLabel> title;
    bool radio = true;
    QIcon icon;

    double icon_scale_value = 0.1;
    uint16_t minimized_width = 60;
    int32_t button_index;

public:
    SideBarButtonWidget(bool radio = true, QWidget* parent = nullptr );
    ~SideBarButtonWidget() = default;

    void setIndex(int32_t index);
    quint16 minimizedWidth();
    void setMinimizedWidth( quint16 value );
    double iconScale();
    void setIconScale(double value);
    int32_t index();

    void setButtonTitle(QString title);
    void setButtonIcon(QString image_path);
    void paintEvent(QPaintEvent* event) override;

signals:
    void signal_checkedChange(QAbstractButton* button);
    void signal_buttonPressed( int32_t index, QAbstractButton* button);
};



class SideBarWidget : public QWidget{
    Q_OBJECT
    Q_PROPERTY(quint16 minimized_width READ minimizedWidth WRITE setMinimizedWidth )

    QVector<SideBarButtonWidget*> buttons;
    std::unique_ptr<QVBoxLayout> main_layout;
    std::unique_ptr<QPropertyAnimation> side_animation;

    uint16_t minimized_width = 60;
    bool mouse_enter = false;
    bool slide_maximized = false;
    QRect default_rect;
    uint16_t delay_to_maximize = 200;
    uint16_t delay_to_minimize = 200;
    uint16_t animation_duration = 300;
    int32_t current_checked = -1;

public:
    SideBarWidget(QWidget* parent = nullptr);
    ~SideBarWidget();

    quint16 minimizedWidth();
    void setMinimizedWidth( quint16 value );

    SideBarButtonWidget* getButton(int32_t button_index );
    const QVector<SideBarButtonWidget*>& getButtons();

    void addStretch(int stretch);
    void addButton(QString title, bool radio = true );
    void addButton(QString image_path, QString title, bool radio = true);
    void maximizeMenu();
    void minimizeMenu();
    void paintEvent(QPaintEvent* e) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

public slots:
    void checkedHandler(QAbstractButton* button);
signals:
    void signal_checkedChange(int32_t index);
};

#endif // SIDEBAR_H
