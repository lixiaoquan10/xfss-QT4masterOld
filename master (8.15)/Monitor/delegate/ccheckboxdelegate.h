#ifndef CCHECKBOXDELEGATE_H
#define CCHECKBOXDELEGATE_H

#include <QStyledItemDelegate>
#include <QWidget>

class QCheckBox;
class CCheckBoxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CCheckBoxWidget(const QString& trueStr, const QString& falseStr, QWidget* parent = 0);
    ~CCheckBoxWidget();
    void setValue(bool value);
    bool value() const;
private slots:
    void checkChanged();
signals:
    void valueChanged();
private:
    QCheckBox* m_checkBox;
    QString m_trueStr;
    QString m_falseStr;
};

class CCheckBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CCheckBoxDelegate(const QString& trueStr = QString(), const QString& falseStr = QString(), QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private slots:
    void valueChanged();
private:
    QString m_trueStr;
    QString m_falseStr;
};

#endif // CCHECKBOXDELEGATE_H
