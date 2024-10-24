#include "cdirectiondelegate.h"
#include "graphics/cpointitem.h"
#include "cglobal.h"

#include <QHBoxLayout>
#include <QCheckBox>
#include <QApplication>
#include <QPainter>
#include <QDebug>

CDirectionWidget::CDirectionWidget(QWidget *parent) :
    QWidget(parent)
{
    setAutoFillBackground(true);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setMargin(0);
    m_left = new QCheckBox("左向", this);
    layout->addWidget(m_left);
    m_right = new QCheckBox("右向", this);
    layout->addWidget(m_right);
    connect(m_left, SIGNAL(clicked()), this, SLOT(checkChanged()));
    connect(m_right, SIGNAL(clicked()), this, SLOT(checkChanged()));
}


CDirectionWidget::~CDirectionWidget()
{
    delete m_left;
    delete m_right;
}

void CDirectionWidget::setDirection(uchar direction)
{
    if(direction & CPointItem::Direction_Left)
        m_left->setChecked(true);
    else
        m_left->setChecked(false);
    if(direction & CPointItem::Direction_Right)
        m_right->setChecked(true);
    else
        m_right->setChecked(false);
}

uchar CDirectionWidget::direction() const
{
    uchar drt = 0;
    if(m_left->isChecked())
        drt |= CPointItem::Direction_Left;
    if(m_right->isChecked())
        drt |= CPointItem::Direction_Right;
    return drt;
}

void CDirectionWidget::checkChanged()
{
    emit directionChanged();
    setFocus();
}

CDirectionDelegate::CDirectionDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget *CDirectionDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    CDirectionWidget* widget = new CDirectionWidget(parent);
    connect(widget, SIGNAL(directionChanged()), this, SLOT(directionChanged()));
    return widget;
}

void CDirectionDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int direction = index.data(Qt::EditRole).toInt();
    CDirectionWidget* widget = static_cast<CDirectionWidget*>(editor);
    widget->setDirection(direction);
}

void CDirectionDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    CDirectionWidget* widget = static_cast<CDirectionWidget*>(editor);
    int direction = widget->direction();
    model->setData(index, direction);
}

void CDirectionDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    CDirectionWidget* widget = static_cast<CDirectionWidget*>(editor);
    widget->setGeometry(option.rect);
}

void CDirectionDelegate::directionChanged()
{
    CDirectionWidget* widget = static_cast<CDirectionWidget*>(sender());
    emit commitData(widget);
}

