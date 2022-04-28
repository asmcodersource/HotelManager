#ifndef DATEUSERINFORMERWIDGET_H
#define DATEUSERINFORMERWIDGET_H

#include <memory>
#include <QWidget>
#include <QLabel>
#include <QLayout>

class DateUserInformerWidget : public QWidget
{
    Q_OBJECT
    std::unique_ptr<QLabel> label_ptr;
    std::unique_ptr<QVBoxLayout> main_layout;

public:
    explicit DateUserInformerWidget(QWidget *parent = nullptr);

public slots:
    void setLabelValue(QString value);


signals:

};

#endif // DATEUSERINFORMERWIDGET_H
