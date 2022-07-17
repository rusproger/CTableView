#ifndef CTableView_H
#define CTableView_H

// CTableView - компонент реализующий функционал таблицы с плавающим заголовком
// Версия 05.05.2019
// Описание методов размещено над их реализацией

#include <QPainter>
#include <QAbstractButton>
#include <QHeaderView>
#include <QTableView>
#include <QScrollBar>
#include <QScrollArea>

class CTableView: public QTableView
{
    Q_OBJECT

    class QTableCornerButton : public QAbstractButton
    {
    public:
        QTableCornerButton(QWidget *parent) : QAbstractButton(parent) {}
        void paintEvent(QPaintEvent*) {
            QStyleOptionHeader opt;
            opt.init(this);
            QStyle::State state = QStyle::State_None;
            if (isEnabled())
                state |= QStyle::State_Enabled;
            if (isActiveWindow())
                state |= QStyle::State_Active;
            if (isDown())
                state |= QStyle::State_Sunken;
            opt.state = state;
            opt.rect = rect();
            opt.position = QStyleOptionHeader::OnlyOneSection;
            QPainter painter(this);
            style()->drawControl(QStyle::CE_Header, &opt, &painter, this);
        }
    };
public:
    CTableView(QWidget *parent, QScrollArea *scrollArea);
    CTableView(QWidget *parent);
    void setHorizontalHeader(QHeaderView *header);
    void setVerticalHeader(QHeaderView *header);
private slots:
    void scrollXChanged(int value);
    void scrollYChanged(int value);
protected:
    void updateGeometries();
private:
    // Положение вертикального заголовка относительно левого края таблицы
    int Left, // Left>0 соответствует положению над таблицей
    // Положение горизонтального заголовка относительно верхнего края таблицы
        Top;  // Top>0 соответствует положению над таблицей
    // Кнопка выделения ячеек таблицы
    QWidget *cornerWidget;
};

#endif // CTableView_H
