#ifndef CDIRECTIONDELEGATE_H
#define CDIRECTIONDELEGATE_H

#include <QStyledItemDelegate>
#include <QWidget>

class QCheckBox;
class CDirectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CDirectionWidget(QWidget* parent = 0);
    ~CDirectionWidget();
    void setDirection(uchar direction);
    uchar direction() const;
private slots:
    void checkChanged();
signals:
    void directionChanged();
private:
    QCheckBox* m_left;
    QCheckBox* m_right;
};

class CDirectionDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CDirectionDelegate(QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private slots:
    void directionChanged();
};

#endif // CDIRECTIONDELEGATE_H
