#include "mainwindow.h"


QString cssStyle =  R"(
font-family: "Comic Sans MS", sans-serif;
color: rgb(255, 255, 255);
font-weight: 500;
text-decoration: none;
font-variant: normal;
text-transform: none;
text-align: center;
)";



#define wid 5

MainWindow::MainWindow(QWidget* parent): QWidget(parent){
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    main_layout = new QVBoxLayout();
    window_title = new WindowTitle();


    setLayout(main_layout);
    main_layout->setSpacing(0);
    main_layout->addWidget(window_title);
    main_layout->addWidget(new QWidget, 9999);
    main_layout->setContentsMargins(5,5,5,5);

    connect(window_title, &WindowTitle::signal_closeClicked, this, &QWidget::close);
    connect(window_title, &WindowTitle::signal_hideClicked, this, [=](){this->showMinimized(); repaint();});
    connect(window_title, &WindowTitle::signal_maximazeClicked, this, &MainWindow::slot_maximazeButtonClicked);

    setMouseTracking(true);
    setAttribute( Qt::WA_TranslucentBackground, true );
    setAttribute(Qt::WA_Hover);
    window_title->setMainWindow(this);
}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setBrush(QColor(0, 0, 0, 2));
    painter.setPen(QColor(0, 0, 0, 2));
    painter.drawRect(0, 0 ,width(),height());

    painter.setBrush(QColor(48, 50, 54));
    painter.setPen(QColor(48, 50, 54));
    if( !this->isMaximized() )
        painter.drawRect(4, 4 ,width() - 4 - 5,height() - 4 - 5);
    else
        painter.drawRect(0, 0 ,width(),height());
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event){
    right = false;
    down = false;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    QPoint cursor_pos = this->mapFromGlobal(QCursor().pos());
    setCursor(Qt::CustomCursor);
    if( event->buttons() != Qt::LeftButton ){
        if( cursor_pos.y() >= height() - wid)
            down = true;
        if( cursor_pos.x() >= width() - wid)
            right = true;
        if( right )
            setCursor(Qt::SizeHorCursor);
        if( down )
            setCursor(Qt::SizeVerCursor);
        if( right && down )
            setCursor(Qt::SizeFDiagCursor);
    }
    if ( event->buttons() == Qt::LeftButton  ){
        if( down == true )
            resize(this->size().width(), cursor_pos.y());
        if( right == true )
            resize(cursor_pos.x(), this->size().height());
        if( top == true ){
            if( cursor_pos.y() < 0 ){
                resize(this->size().width(),height() - cursor_pos.y());
                move(this->pos().x(), this->pos().y() + cursor_pos.y());
            }
        }
        if( left == true ){
            if( cursor_pos.x() < 0 ){
                resize(width() - cursor_pos.x(),this->size().height());
                move(this->pos().x() + cursor_pos.x(), this->pos().y());
            }
        }
    }
}

void MainWindow::enterEvent(QEnterEvent* event){
    top = false, down = false, left = false, right = false;
}

void MainWindow::leaveEvent(QEvent* event){
    setCursor(Qt::CustomCursor);

}

void MainWindow::slot_maximazeButtonClicked(){
    if( this->isMaximized() ){
        window_title->button_maximaze.setButtonType(buttonType::maximize);
        main_layout->setContentsMargins(5,5,5,5);
        showNormal();
    } else {
        window_title->button_maximaze.setButtonType(buttonType::minimaze);
        main_layout->setContentsMargins(0,0,0,0);
        showMaximized();
    }
}

void MainWindow::setCentralWidget(QWidget* widget){
    main_layout->addWidget(widget, 99999999);
    main_layout->update();
}

//----------------------------------------------------------------------------

WindowTitle::WindowTitle(QWidget* parent ): QWidget(parent){
    setMinimumHeight(23);
    setMaximumHeight(23);
    window_title = new QLabel();
    main_layout = new QGridLayout();

    buttons_layer = new QHBoxLayout();
    buttons_layer->addWidget(&button_hide);
    buttons_layer->addWidget(&button_maximaze);
    buttons_layer->addWidget(&button_close);
    buttons_layer->setContentsMargins(0,0,0,0);
    button_hide.setButtonType(buttonType::hide);
    connect(&button_hide, &QAbstractButton::clicked, this, &WindowTitle::signal_hideClicked);
    button_maximaze.setButtonType(buttonType::maximize);
    connect(&button_maximaze, &QAbstractButton::clicked, this, &WindowTitle::signal_maximazeClicked);
    button_close.setButtonType(buttonType::close);
    connect(&button_close, &QAbstractButton::clicked, this, &WindowTitle::signal_closeClicked);

    window_title->setText("  Tiny Hotel Manager");
    window_title->setStyleSheet(cssStyle);
    main_layout->setContentsMargins(1,1,1,1);
    main_layout->setSpacing(0);
    main_layout->addWidget(window_title, 0, 0, Qt::AlignLeft);
    main_layout->addItem(buttons_layer, 0, 2, Qt::AlignRight);
    setLayout(main_layout);
    setMouseTracking(true);
    left_button_pressed = false;
    setAttribute(Qt::WA_Hover);
}

void WindowTitle::paintEvent(QPaintEvent* event){
    QPainter painter(this);
    painter.setBrush(QColor(28, 35, 41));
    painter.setPen(QColor(28, 35, 41));
    painter.drawRect(0,0,width(),height());
}

void WindowTitle::enterEvent(QEnterEvent* event){
    setCursor(Qt::CustomCursor);
}

void WindowTitle::setMainWindow(QWidget *main_window){
    this->main_window = main_window;
}

void WindowTitle::mousePressEvent(QMouseEvent* event){
    left_button_pressed = true;
    first_press = QCursor().pos();
    first_pos = main_window->pos();
    double x_press = static_cast<double>(QCursor().pos().x()) / main_window->width();

    //qobject_cast<MainWindow*>(main_window)->slot_maximazeButtonClicked();
    if( main_window->isMaximized() ){
        main_window->showNormal();
        WindowTitle::button_maximaze.button_type = buttonType::maximize;
        first_pos = QCursor::pos() - QPoint( main_window->width() * x_press, 5);
        main_window->move(first_pos - (first_press - QCursor().pos()));
    }

}

void WindowTitle::mouseReleaseEvent(QMouseEvent* event){
    left_button_pressed = false;

}

void WindowTitle::mouseMoveEvent(QMouseEvent *event){
    setCursor(Qt::CustomCursor);
    if( left_button_pressed ){
        main_window->move(first_pos - (first_press - QCursor().pos()));

    }
}

void WindowTitle::slot_buttonCloseClicked(){
    //emit signal_closeClicked();
}

//-----------------------------------------------------------------------------


WindowTitleButton::WindowTitleButton(QAbstractButton* parent): QAbstractButton(parent){
    setFixedWidth(20);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    background_color = QColor(28, 35, 41);
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
}

void WindowTitleButton::enterEvent(QEnterEvent* event ){
    switch(button_type){
    case buttonType::close:
        background_color = QColor(227, 25, 25);
        break;
    default:
        background_color = QColor(51, 62, 71);
        break;
    }
}

void WindowTitleButton::mousePressEvent(QMouseEvent *event){
   background_color.setRed(background_color.red() + 25);
   background_color.setGreen(background_color.green() + 25);
   background_color.setBlue(background_color.blue() + 25);
   QAbstractButton::mousePressEvent(event);
}

void WindowTitleButton::mouseReleaseEvent(QMouseEvent *event){
   background_color.setRed(background_color.red() - 25);
   background_color.setGreen(background_color.green() - 25);
   background_color.setBlue(background_color.blue() - 25);
   QAbstractButton::mouseReleaseEvent(event);
}

void WindowTitleButton::leaveEvent(QEvent* event ){
   background_color = QColor(28, 35, 41);
}

void WindowTitleButton::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setPen(background_color);
    painter.setBrush(background_color);
    painter.drawRect(0,0, width(), height());
    QPen pen;

    switch(button_type){
    case buttonType::close:
        pen.setColor(QColor(255,255,255));
        pen.setWidthF(1.3);
        painter.setPen(pen);
        painter.drawLine(width()/2.0 - width()/4.0,  height()/2.0 - height()/4.0, width()/2.0 + width()/4.0, height()/2.0 + height()/4.0);
        painter.drawLine(width()/2.0 - width()/4.0,  height()/2.0 + height()/4.0, width()/2.0 + width()/4.0, height()/2.0 - height()/4.0);
        break;
    case buttonType::hide:
        pen.setColor(QColor(255,255,255));
        pen.setWidthF(1.0);
        painter.setPen(pen);
        painter.drawLine(width()/2.0 - width()/4.0,  height()/2.0, width()/2.0 + width()/4.0, height()/2.0);
        break;
    case buttonType::maximize:
        pen.setColor(QColor(255,255,255));
        pen.setWidthF(1.0);
        painter.setPen(pen);
        painter.drawRect(width()/2.0 - width()/4.0,  height()/2.0 - height()/4.0, width()/2.0, height()/2.0);
        break;
    case buttonType::minimaze:
        pen.setColor(QColor(255,255,255));
        pen.setWidthF(1.0);
        painter.setPen(pen);
        painter.drawRect(width()/2.0 - width()/4.0,  height()/2.0 - height()/4.0, width()/2.0, height()/2.0);
        painter.drawRect(width()/2.0 - width()/4.0 + 1,  height()/2.0 - height()/4.0 - 1, width()/2.0, height()/2.0);
        break;
    }

}

void WindowTitleButton::setButtonType(buttonType type){
    button_type = type;
    repaint();
}
