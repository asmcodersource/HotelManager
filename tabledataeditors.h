#ifndef TABLEDATAEDITORS_H
#define TABLEDATAEDITORS_H

#include <memory>
#include <tableviewfilters.h>
#include <QVector>
#include <QPainter>
#include <QObject>
#include <QWidget>
#include <QLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QStandardItemModel>
#include <QResizeEvent>
#include <QAbstractItemDelegate>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QItemDelegate>
#include <QStandardItem>
#include <QRegularExpressionValidator>
#include <QTableView>
#include <QHeaderView>
#include <QtSql>
#include <QDebug>
#include <scrollbar.h>
#include <tabledata.h>

class DataEditor: public QWidget{
    Q_OBJECT

public:
    DataEditor( QWidget* parent = nullptr);
    virtual void setData(QStandardItemModel* model, int32_t row) = 0;

public slots:
    virtual void slot_valueChanged() = 0;
    virtual void slot_buttonConfirmPressed() = 0;
    virtual void slot_buttonCancelPressed() = 0;
};

//-----------------------------------------------------------------------------------------------

class RoomsCategoriesEditor: public DataEditor{
    Q_OBJECT

    std::unique_ptr<QVBoxLayout> main_layout;
    std::unique_ptr<QHBoxLayout> buttons_layout;
    std::unique_ptr<QLineEdit> category_name;
    std::unique_ptr<QTextEdit> category_desc;
    std::unique_ptr<QLineEdit> category_places;
    std::unique_ptr<QLineEdit> category_price;
    std::unique_ptr<QPushButton> button_confirm;
    std::unique_ptr<QPushButton> button_cancel;
    QStandardItemModel* model;
    int32_t row;

public:
    RoomsCategoriesEditor(QWidget* parent = nullptr);
    void setData(QStandardItemModel* model, int32_t row) override;

public slots:
    void slot_valueChanged() override;
    void slot_buttonConfirmPressed() override;
    void slot_buttonCancelPressed() override;
};

//------------------------------------------------------------------------------------------------

class RoomsEditor: public DataEditor{
    Q_OBJECT

    std::unique_ptr<QVBoxLayout> main_layout;
    std::unique_ptr<QHBoxLayout> buttons_layout;
    std::unique_ptr<QLineEdit> room_name;
    std::unique_ptr<QTextEdit> room_desc;
    std::unique_ptr<QComboBox> room_type;
    std::unique_ptr<QPushButton> button_confirm;
    std::unique_ptr<QPushButton> button_cancel;
    QStandardItemModel* model;
    int32_t row;

public:
    RoomsEditor(QWidget* parent = nullptr);
    void setData(QStandardItemModel* model, int32_t row) override;

public slots:
    void slot_valueChanged() override;
    void slot_buttonConfirmPressed() override;
    void slot_buttonCancelPressed() override;
};

//------------------------------------------------------------------------------------------------

class GuestsEditor: public DataEditor{
    Q_OBJECT

    std::unique_ptr<QVBoxLayout> main_layout;
    std::unique_ptr<QHBoxLayout> buttons_layout;
    std::unique_ptr<QLineEdit> guest_name;
    std::unique_ptr<QComboBox> guest_room_id;
    std::unique_ptr<QComboBox> guest_state_id;
    std::unique_ptr<QTextEdit> guest_desc;
    std::unique_ptr<QTextEdit> guest_contacts;
    std::unique_ptr<QLineEdit> guest_adress;
    std::unique_ptr<QLineEdit> guest_first_day;
    std::unique_ptr<QLineEdit> guest_last_day;
    std::unique_ptr<QLineEdit> guest_debt;
    std::unique_ptr<QPushButton> button_confirm;
    std::unique_ptr<QPushButton> button_cancel;
    QStandardItemModel* model;
    int32_t row;
    QRegularExpressionValidator date_validator;

public:
    GuestsEditor(QWidget* parent = nullptr);
    void setData(QStandardItemModel* model, int32_t row) override;

public slots:
    void slot_valueChanged() override;
    void slot_buttonConfirmPressed() override;
    void slot_buttonCancelPressed() override;
};

//------------------------------------------------------------------------------------------------

#endif // TABLEDATAEDITORS_H
