#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QTableWidget>

class TableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit TableWidget(QWidget *parent = 0);
    explicit TableWidget(int rows, int columns, QWidget *parent = 0);
    explicit TableWidget(int rows, int columns, QStringList headerlist, QStringList icolist, QWidget *parent = 0);
    explicit TableWidget(QStringList headerlist, QWidget *parent = 0);
    
signals:
    
public slots:    
    bool InsertSingleRow(int row, QStringList content, QBrush brush=QBrush(QColor(Qt::transparent)));
    void AppendSingleRow(QStringList content, QBrush brush=QBrush(QColor(Qt::transparent)));
    void RemoveSingleRow(int row);
    void RemoveLastSingleRow();
    void SetItemText(int nrow, int ncolumn, QString strtext);
    void SetRowColor(int nrow, const QColor &color);
    
};

#endif // TABLEWIDGET_H
