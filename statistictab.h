#ifndef STATISTICTAB_H
#define STATISTICTAB_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>

class StatisticTab : public QWidget
{
    Q_OBJECT
    QVBoxLayout* main_layout;
    QGridLayout* grid;
    QLabel* header;

public:
    explicit StatisticTab(QWidget *parent = nullptr);

signals:

};

#endif // STATISTICTAB_H
