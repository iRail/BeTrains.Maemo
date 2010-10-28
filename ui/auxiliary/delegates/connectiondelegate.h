//
// Configuration
//

// Include guard
#ifndef CONNECTIONDELEGATE_H
#define CONNECTIONDELEGATE_H

// Includes
#include <QObject>
#include <QPainter>
#include <QWidget>
#include <QModelIndex>
#include <QStyleOptionViewItem>
#include <QStyledItemDelegate>

namespace iRail
{
    class ConnectionDelegate : public QStyledItemDelegate
    {
        Q_OBJECT

    public:
        ConnectionDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    };
}

#endif // CONNECTIONDELEGATE_H
