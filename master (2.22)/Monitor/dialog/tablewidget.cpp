#include <QHeaderView>
#include <QTableWidgetItem>
#include "tablewidget.h"

TableWidget::TableWidget(QWidget *parent) :
    QTableWidget(parent)
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setShowGrid(true);
    setGridStyle(Qt::SolidLine);
    setFrameShape(QFrame::NoFrame);
    horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->setDefaultSectionSize(70);
}

TableWidget::TableWidget(int rows, int columns, QWidget *parent) :
    QTableWidget(rows, columns, parent)
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setShowGrid(true);
    setGridStyle(Qt::SolidLine);
    setFrameShape(QFrame::NoFrame);
    horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->setDefaultSectionSize(70);
}

TableWidget::TableWidget(int rows, int columns, QStringList headerlist, QStringList icolist, QWidget *parent) :
    QTableWidget(rows, columns, parent)
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setShowGrid(true);
    setGridStyle(Qt::SolidLine);
    setFrameShape(QFrame::NoFrame);
    horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->setDefaultSectionSize(70);
    int columnnum = qMin(columns, headerlist.length());
    columnnum = qMin(columnnum,icolist.length());
    for(int i=0; i<columnnum; i++)
    {
        QTableWidgetItem *newhorizontalHeaderItem = new QTableWidgetItem(QIcon(icolist.at(i)), headerlist.at(i));
        setHorizontalHeaderItem(i, newhorizontalHeaderItem);
    }
}

TableWidget::TableWidget(QStringList headerlist, QWidget *parent) :
    QTableWidget(parent)
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setShowGrid(true);
    setGridStyle(Qt::SolidLine);
    setFrameShape(QFrame::NoFrame);
    horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->setDefaultSectionSize(70);
    setRowCount(0);
    setColumnCount(headerlist.length());
    setHorizontalHeaderLabels(headerlist);
}


bool TableWidget::InsertSingleRow(int row, QStringList content, QBrush brush)
{
    insertRow(row);
    QTableWidgetItem *newVerticalHeaderItem = new QTableWidgetItem(QString::number(row+1));
    setVerticalHeaderItem(row, newVerticalHeaderItem);
    int nCount = qMin(columnCount(), content.length());
    for(int i=0; i<nCount; i++)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem(content.at(i));
        newItem->setBackground(brush);
        setItem(row, i, newItem);
    }
    for(int j=row; j<rowCount();j++)
    {
        verticalHeaderItem(j)->setText(QString::number(j+1));
    }
    setCurrentCell(row,0);
    return true;
}

void TableWidget::AppendSingleRow(QStringList content, QBrush brush)
{
    int row = rowCount();
    insertRow(row);
    QTableWidgetItem *newVerticalHeaderItem = new QTableWidgetItem(QString::number(row+1));
    setVerticalHeaderItem(row, newVerticalHeaderItem);
    int nCount = qMin(columnCount(), content.length());
    for(int i=0; i<nCount; i++)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem(content.at(i));
        newItem->setBackground(brush);
        setItem(row, i, newItem);
    }
    setCurrentCell(row,0);
}

void TableWidget::RemoveSingleRow(int row)
{
    removeRow(row);
    for(int j=row; j<rowCount();j++)
    {
        verticalHeaderItem(j)->setText(QString::number(j+1));
    }
}

void TableWidget::RemoveLastSingleRow()
{
    removeRow(rowCount()-1);
}

void TableWidget::SetItemText(int nrow, int ncolumn, QString strtext)
{
    QTableWidgetItem *newItem = new QTableWidgetItem(strtext);
    setItem(nrow, ncolumn, newItem);
}

void TableWidget::SetRowColor(int nrow, const QColor &color)
{
    for(int i=0; i< columnCount(); i++)
    {
        QTableWidgetItem *Item = item(nrow, i);
        Item->setBackground(QBrush(color));
    }
}
