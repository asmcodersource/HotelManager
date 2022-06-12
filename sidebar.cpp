#include "sidebar.h"

SideBarButtonWidget::SideBarButtonWidget(bool radio, QWidget* parent  ): QPushButton(parent){
    SideBarButtonWidget::radio = radio;
    spacer_item = std::unique_ptr<QSpacerItem>(new QSpacerItem(minimized_width,1));
    main_layout = std::unique_ptr<QHBoxLayout>(new QHBoxLayout);
    title = std::unique_ptr<QLabel>(new QLabel);

    main_layout->addItem(spacer_item.get());
    main_layout->addWidget(title.get());
    main_layout->setContentsMargins(0, 0, 0, 0);
    setLayout(main_layout.get());
    setFixedHeight(minimized_width);
    setMinimumWidth(200);

    if( radio )
        setCheckable(true);

    connect(this, &QAbstractButton::clicked, this, [&, this](){
       if( SideBarButtonWidget::radio )
            emit signal_checkedChange(this);

    });

}

void SideBarButtonWidget::setIndex(int32_t index){
    SideBarButtonWidget::button_index = index;
}

void SideBarButtonWidget::setButtonTitle(QString title){
    SideBarButtonWidget::title->setText(title);
    SideBarButtonWidget::title->setAlignment(Qt::AlignLeft | Qt::AlignVCenter );
}

void SideBarButtonWidget::setButtonIcon(QString image_path){
    icon = QIcon(image_path);
}

void SideBarButtonWidget::paintEvent(QPaintEvent* event){
    {
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
        QPushButton::paintEvent(event);
    }
    QPainter p(this);
    int32_t sub_size = minimized_width * icon_scale_value / 2.0;
    icon.paint(&p, QRect(sub_size, sub_size, minimized_width - sub_size * 2, minimized_width - sub_size * 2), Qt::AlignVCenter | Qt::AlignHCenter);
}

quint16 SideBarButtonWidget::minimizedWidth(){
    return minimized_width;
}

void SideBarButtonWidget::setMinimizedWidth( quint16 value ){
    minimized_width = value;
    spacer_item->changeSize(minimized_width,1);
    setFixedHeight(minimized_width);
    main_layout->update();
}

double SideBarButtonWidget::iconScale(){
    return icon_scale_value;
}

void SideBarButtonWidget::setIconScale(double value){
    icon_scale_value = 1 - value;
    repaint();
}

int32_t SideBarButtonWidget::index(){
    return button_index;
}

/////////////////////////////////////////////////////////////////////////////////

SideBarWidget::SideBarWidget(QWidget* parent ): QWidget(parent){
    side_animation = std::unique_ptr<QPropertyAnimation>(new QPropertyAnimation(this));
    main_layout = std::unique_ptr<QVBoxLayout>(new QVBoxLayout);

    side_animation->setTargetObject(this);
    side_animation->setPropertyName("geometry");
    side_animation->setDuration(animation_duration);

    setAutoFillBackground(true);
    setMinimumWidth(minimized_width);
    main_layout->setContentsMargins(0,0,0,0);
    main_layout->setSpacing(0);
    main_layout->setAlignment(Qt::AlignTop);
    setLayout(main_layout.get());
    setMaximumWidth(200);
}

SideBarWidget::~SideBarWidget(){
    for( SideBarButtonWidget* button : buttons )
        delete button;
}


SideBarButtonWidget* SideBarWidget::getButton(int32_t button_index ){
    if( buttons.size() > button_index )
        return buttons[button_index];
    else
        return nullptr;
}

const QVector<SideBarButtonWidget*>&  SideBarWidget::getButtons(){
    return buttons;
}

void SideBarWidget::addStretch( int stretch ){
    main_layout->addStretch( stretch );
    main_layout->update();
}

void SideBarWidget::addButton(QString image_path, QString title, bool radio ){
    addButton(title, radio);
    SideBarButtonWidget* button = buttons.back();
    button->setButtonIcon(image_path);
}

void SideBarWidget::addButton(QString title, bool radio){
    SideBarButtonWidget* button = new SideBarButtonWidget(radio);
    button->setButtonTitle(title);
    main_layout->addWidget(button);
    connect(button, &SideBarButtonWidget::signal_checkedChange, this, &SideBarWidget::checkedHandler);
    button->setIndex(buttons.size());
    buttons.push_back(button);
}


quint16 SideBarWidget::minimizedWidth(){
    return minimized_width;
}

void SideBarWidget::setMinimizedWidth( quint16 value ){
    minimized_width = value;
    setMinimumWidth(minimized_width);
    resize(minimized_width, height());
}

void SideBarWidget::paintEvent(QPaintEvent* event){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}

void SideBarWidget::enterEvent(QEvent *event){
    if( slide_maximized == false ){
        // Якщо меню згорнуто
        mouse_enter = true;
        QTimer::singleShot(delay_to_maximize, this, [&](){
            // Через 500 міллісекунд після наведення мишки
            if( mouse_enter == false )
                return;
            maximizeMenu();
        });
    }
}

void SideBarWidget::leaveEvent(QEvent *event){
    mouse_enter = false;
    if( slide_maximized == true ){
        // Якщо меню розгорнуто
        QTimer::singleShot(delay_to_minimize, this, [&](){
            // Через 500 міллісекунд після відведення мишки
            if( mouse_enter == true )
                return;
            minimizeMenu();
        });
    }
}


void SideBarWidget::maximizeMenu(){
    if( slide_maximized == true )
        return;
    slide_maximized = true;
    default_rect = rect();
    QRect maximized_rect = default_rect;
    maximized_rect.setWidth(maximumWidth());
    side_animation->setStartValue(rect());
    side_animation->setEndValue(maximized_rect);
    side_animation->setEasingCurve(QEasingCurve::InOutQuint);
    side_animation->start();
}

void SideBarWidget::minimizeMenu(){
    if( slide_maximized == false)
        return;
    slide_maximized = false;
    side_animation->setStartValue(rect());
    side_animation->setEndValue(default_rect);
    side_animation->setEasingCurve(QEasingCurve::OutQuint);
    side_animation->start();
}

void SideBarWidget::checkedHandler(QAbstractButton* button){
    for( SideBarButtonWidget* button_i : buttons )
        if( button_i != button )
            button_i->setChecked(false);
    button->setChecked(true);
    if( qobject_cast<SideBarButtonWidget*>(button)->index() != current_checked )
        emit signal_checkedChange(qobject_cast<SideBarButtonWidget*>(button)->index());
    current_checked = qobject_cast<SideBarButtonWidget*>(button)->index();
}

