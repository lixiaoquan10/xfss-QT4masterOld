#include "ccheckboxdelegate.h"

#include <QHBoxLayout>
#include <QCheckBox>

CCheckBoxWidget::CCheckBoxWidget(const QString &trueStr, const QString &falseStr, QWidget *parent) :
    QWidget(parent), m_trueStr(trueStr), m_falseStr(falseStr)
{
    setAutoFillBackground(true);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setMargin(0);
    m_checkBox = new QCheckBox(m_falseStr, this);
    layout->addWidget(m_checkBox);
    connect(m_checkBox, SIGNAL(clicked()), this, SLOT(checkChanged()));
}
CCheckBoxWidget::~CCheckBoxWidget()
{
    delete m_checkBox;
}

void CCheckBoxWidget::setValue(bool value)
{
    m_checkBox->setChecked(value);
    m_checkBox->setText(value ? m_trueStr : m_falseStr);
}

bool CCheckBoxWidget::value() const
{
    return m_checkBox->isChecked();
}

void CCheckBoxWidget::checkChanged()
{
    m_checkBox->setText(m_checkBox->isChecked() ? m_trueStr : m_falseStr);
    emit valueChanged();
    setFocus();
}

CCheckBoxDelegate::CCheckBoxDelegate(const QString &trueStr, const QString &falseStr, QObject *parent) :
    QStyledItemDelegate(parent), m_trueStr(trueStr), m_falseStr(falseStr)
{
}

QWidget *CCheckBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    CCheckBoxWidget* widget = new CCheckBoxWidget(m_trueStr, m_falseStr, parent);
    connect(widget, SIGNAL(valueChanged()), this, SLOT(valueChanged()));
    return widget;
}

void CCheckBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    bool b = index.data(Qt::EditRole).toBool();
    CCheckBoxWidget* widget = static_cast<CCheckBoxWidget*>(editor);
    widget->setValue(b);
}

void CCheckBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    CCheckBoxWidget* widget = static_cast<CCheckBoxWidget*>(editor);
    bool b = widget->value();
    model->setData(index, b);
}

void CCheckBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    CCheckBoxWidget* widget = static_cast<CCheckBoxWidget*>(editor);
    widget->setGeometry(option.rect);
}

void CCheckBoxDelegate::valueChanged()
{
    CCheckBoxWidget* widget = static_cast<CCheckBoxWidget*>(sender());
    emit commitData(widget);
}

