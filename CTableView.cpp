// CTableView - компонент реализующий функционал таблицы с плавающим заголовком
// Версия 05.05.2019
// Описание методов размещено над их реализацией

#include "CTableView.h"

// Для реализации функционала плавающего заголовка используйте конструктор
CTableView::CTableView(QWidget *parent, QScrollArea *scrollArea):
    QTableView(parent), Left(0),Top(0)
{
    Q_ASSERT(scrollArea);
    // Создание кнопки выделения ячеек таблицы
    cornerWidget = new QTableCornerButton(this);
    cornerWidget->setFocusPolicy(Qt::NoFocus);
    connect(cornerWidget, SIGNAL(clicked()), this, SLOT(selectAll()));
    cornerWidget->raise();

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Запрещаем появление полосы прокрутки
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // Определяем реакцию на перемещение содержимого QScrollArea
    connect(scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)),this, SLOT(scrollYChanged(int)));
    connect(scrollArea->horizontalScrollBar(), SIGNAL(valueChanged(int)),this, SLOT(scrollXChanged(int)));
}
// Для обеспечения обратной совместимости с QTableView используется конструктор
CTableView::CTableView(QWidget *parent):
    QTableView(parent), Left(0),Top(0),cornerWidget(0)
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
}
// Методы обеспечивающие создание подключение вертикального заголовка пользователя
// Метод переопределен для помещения кнопки выделения ячеек на вершину стека виджетов родительского виджета
void CTableView::setVerticalHeader(QHeaderView * header)
{
    QTableView::setVerticalHeader(header);
    if (cornerWidget)
        cornerWidget->raise();
}
// Методы обеспечивающие создание подключение горизонтального заголовка пользователя
// Метод переопределен для помещения кнопки выделения ячеек на вершину стека виджетов родительского виджета
void CTableView::setHorizontalHeader(QHeaderView * header)
{
    QTableView::setHorizontalHeader(header);
    if (cornerWidget)
        cornerWidget->raise();
}
// Слот обеспечивающий реакцию на ’-перемещение содержимого QScrollArea
void CTableView::scrollXChanged(int value){

    QPoint parrentPos = mapToParent(QPoint(0,0));
    Left = value - parrentPos.x() - frameWidth();
    updateGeometries();
}
// Слот обеспечивающий реакцию на Y-перемещение содержимого QScrollArea
void CTableView::scrollYChanged(int value)
{
    QPoint parrentPos = mapToParent(QPoint(0,0));
    Top = value - parrentPos.y() - frameWidth();
    updateGeometries();
}
// Метод реализующий определение положения плавающих заголовков
void CTableView::updateGeometries()
{
        QTableView::updateGeometries();

        bool reverse = isRightToLeft();
        // Определение ширины вертикального заголовка
        int hwidth = 0;
        if (!verticalHeader()->isHidden()) {
            hwidth = qBound(verticalHeader()->minimumWidth(), verticalHeader()->sizeHint().width(), verticalHeader()->maximumWidth());
        }
        // Определение высоты горизонтального заголовка
        int hheight = 0;
        if (!horizontalHeader()->isHidden()) {
            hheight = qBound(horizontalHeader()->minimumHeight(), horizontalHeader()->sizeHint().height(), horizontalHeader()->maximumHeight());
        }
        // Определение ширины и высоты области таблицы
        int theight = 2*frameWidth(),
            twidth  = 2*frameWidth();

        const int colCount = horizontalHeader()->count();
        for (int  column = colCount - 1; column >= 0; --column) {
                twidth += this->columnWidth(column);
        }

        const int rowCount = verticalHeader()->count();
        for (int row = rowCount - 1; row >= 0; --row) {
                theight += this->rowHeight(row);
        }
        // Размер таблицы фиксированный
        setFixedSize(QSize(hwidth+twidth,hheight+theight));

        QRect vg = viewport()->geometry();
        // Определение положения вертикального заголовка
        int verticalLeft = reverse ? vg.right() + 1 : (vg.left() - hwidth);

        if (Left>0)
            verticalLeft += qMin(Left,twidth-frameWidth());

        verticalHeader()->setGeometry(verticalLeft, vg.top(), hwidth, vg.height());
        // Определение положения горизонтального заголовка
        int horizontalTop = vg.top() - hheight;

        if (Top>0)
            horizontalTop += qMin(Top,theight-frameWidth());

        horizontalHeader()->setGeometry(vg.left(), horizontalTop, vg.width(), hheight);

        // Определение положения кнопки выделения ячеек таблицы
        if (cornerWidget)
        {
             cornerWidget->setHidden(horizontalHeader()->isHidden() || verticalHeader()->isHidden());
             cornerWidget->setGeometry(verticalLeft, horizontalTop, hwidth, hheight);
        }
}
