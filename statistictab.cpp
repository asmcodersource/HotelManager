#include "statistictab.h"

StatisticTab::StatisticTab(QWidget *parent) : QWidget{parent}{
    main_layout = new QVBoxLayout;
    header = new QLabel;
    grid = new QGridLayout;
    header->setText("Загальна інформація");
    header->setMaximumHeight(50);
    main_layout->setSpacing(0);
    main_layout->addWidget(header);
    main_layout->addItem(grid);
    setLayout(main_layout);
    main_layout->setContentsMargins(0,0,0,0);
    setAutoFillBackground(true);
}
