//
// Configuration
//

// Include guard
#ifndef CONNECTIONREQUESTDELEGATE_H
#define CONNECTIONREQUESTDELEGATE_H

// Includes
#include <QObject>
#include <QPainter>
#include <QWidget>
#include <QModelIndex>
#include <QStyleOptionViewItem>
#include <QStyledItemDelegate>

namespace iRail
{
    class ConnectionRequestDelegate : public QStyledItemDelegate
    {
        Q_OBJECT

    public:
        ConnectionRequestDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    };
}

#endif // CONNECTIONREQUESTDELEGATE_H
