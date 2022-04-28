#include "dateuserinformerwidget.h"

DateUserInformerWidget::DateUserInformerWidget(QWidget *parent) : QWidget{parent}{
    // Створення об'єктів даного вікна
    label_ptr = std::unique_ptr<QLabel>(new QLabel);
    main_layout = std::unique_ptr<QVBoxLayout>(new QVBoxLayout);

    // Ініцізація віджету, та його об'єктів
    setAutoFillBackground(true);
    label_ptr->setText("дата");

    // Ініціалізація макету вікна
    main_layout->addWidget(label_ptr.get());
    main_layout->setContentsMargins(0,0,0,0);
    setLayout(main_layout.get());
}

void DateUserInformerWidget::setLabelValue(QString value){
    label_ptr->setText(value);
}
