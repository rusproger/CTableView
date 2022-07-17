// CTableView - ��������� ����������� ���������� ������� � ��������� ����������
// ������ 05.05.2019
// �������� ������� ��������� ��� �� �����������

#include "CTableView.h"

// ��� ���������� ����������� ���������� ��������� ����������� �����������
CTableView::CTableView(QWidget *parent, QScrollArea *scrollArea):
    QTableView(parent), Left(0),Top(0)
{
    Q_ASSERT(scrollArea);
    // �������� ������ ��������� ����� �������
    cornerWidget = new QTableCornerButton(this);
    cornerWidget->setFocusPolicy(Qt::NoFocus);
    connect(cornerWidget, SIGNAL(clicked()), this, SLOT(selectAll()));
    cornerWidget->raise();

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    // ��������� ��������� ������ ���������
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // ���������� ������� �� ����������� ����������� QScrollArea
    connect(scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)),this, SLOT(scrollYChanged(int)));
    connect(scrollArea->horizontalScrollBar(), SIGNAL(valueChanged(int)),this, SLOT(scrollXChanged(int)));
}
// ��� ����������� �������� ������������� � QTableView ������������ �����������
CTableView::CTableView(QWidget *parent):
    QTableView(parent), Left(0),Top(0),cornerWidget(0)
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
}
// ������ �������������� �������� ����������� ������������� ��������� ������������
// ����� ������������� ��� ��������� ������ ��������� ����� �� ������� ����� �������� ������������� �������
void CTableView::setVerticalHeader(QHeaderView * header)
{
    QTableView::setVerticalHeader(header);
    if (cornerWidget)
        cornerWidget->raise();
}
// ������ �������������� �������� ����������� ��������������� ��������� ������������
// ����� ������������� ��� ��������� ������ ��������� ����� �� ������� ����� �������� ������������� �������
void CTableView::setHorizontalHeader(QHeaderView * header)
{
    QTableView::setHorizontalHeader(header);
    if (cornerWidget)
        cornerWidget->raise();
}
// ���� �������������� ������� �� �-����������� ����������� QScrollArea
void CTableView::scrollXChanged(int value){

    QPoint parrentPos = mapToParent(QPoint(0,0));
    Left = value - parrentPos.x() - frameWidth();
    updateGeometries();
}
// ���� �������������� ������� �� Y-����������� ����������� QScrollArea
void CTableView::scrollYChanged(int value)
{
    QPoint parrentPos = mapToParent(QPoint(0,0));
    Top = value - parrentPos.y() - frameWidth();
    updateGeometries();
}
// ����� ����������� ����������� ��������� ��������� ����������
void CTableView::updateGeometries()
{
        QTableView::updateGeometries();

        bool reverse = isRightToLeft();
        // ����������� ������ ������������� ���������
        int hwidth = 0;
        if (!verticalHeader()->isHidden()) {
            hwidth = qBound(verticalHeader()->minimumWidth(), verticalHeader()->sizeHint().width(), verticalHeader()->maximumWidth());
        }
        // ����������� ������ ��������������� ���������
        int hheight = 0;
        if (!horizontalHeader()->isHidden()) {
            hheight = qBound(horizontalHeader()->minimumHeight(), horizontalHeader()->sizeHint().height(), horizontalHeader()->maximumHeight());
        }
        // ����������� ������ � ������ ������� �������
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
        // ������ ������� �������������
        setFixedSize(QSize(hwidth+twidth,hheight+theight));

        QRect vg = viewport()->geometry();
        // ����������� ��������� ������������� ���������
        int verticalLeft = reverse ? vg.right() + 1 : (vg.left() - hwidth);

        if (Left>0)
            verticalLeft += qMin(Left,twidth-frameWidth());

        verticalHeader()->setGeometry(verticalLeft, vg.top(), hwidth, vg.height());
        // ����������� ��������� ��������������� ���������
        int horizontalTop = vg.top() - hheight;

        if (Top>0)
            horizontalTop += qMin(Top,theight-frameWidth());

        horizontalHeader()->setGeometry(vg.left(), horizontalTop, vg.width(), hheight);

        // ����������� ��������� ������ ��������� ����� �������
        if (cornerWidget)
        {
             cornerWidget->setHidden(horizontalHeader()->isHidden() || verticalHeader()->isHidden());
             cornerWidget->setGeometry(verticalLeft, horizontalTop, hwidth, hheight);
        }
}
