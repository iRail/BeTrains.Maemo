//
// Configuration
//

// Include guard
#ifndef CONNECTIONPOIDELEGATE_H
#define CONNECTIONPOIDELEGATE_H

// Includes
#include <QObject>
#include <QPainter>
#include <QWidget>
#include <QModelIndex>
#include <QStyleOptionViewItem>
#include <QStyledItemDelegate>

namespace iRail
{
    class ConnectionPOIDelegate : public QStyledItemDelegate
    {
        Q_OBJECT

    public:
        ConnectionPOIDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    };
}

#endif // CONNECTIONPOIDELEGATE_H
