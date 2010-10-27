//
// Configuration
//

// Include guard
#ifndef APIDELEGATE_H
#define APIDELEGATE_H

// Includes
#include <QObject>
#include <QPainter>
#include <QWidget>
#include <QModelIndex>
#include <QStyleOptionViewItem>
#include <QStyledItemDelegate>

namespace iRail
{
    class APIDelegate : public QStyledItemDelegate
    {
        Q_OBJECT

    public:
        APIDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    private slots:
        void paintHTML(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QString& iHTML) const;
        QSize sizeHintHTML(const QStyleOptionViewItem &option, const QModelIndex &index, const QString& iHTML) const;
        QString getHTML(const QModelIndex &iIndex) const;
    };
}

#endif // APIDELEGATE_H
