//
// Configuration
//

// Include guard
#ifndef LIVEBOARDDEPARTUREDELEGATE_H
#define LIVEBOARDDEPARTUREDELEGATE_H

// Includes
#include <QObject>
#include <QPainter>
#include <QWidget>
#include <QModelIndex>
#include <QStyleOptionViewItem>
#include <QStyledItemDelegate>
#include "api/station.h"

namespace iRail
{
    class LiveboardDepartureDelegate : public QStyledItemDelegate
    {
        Q_OBJECT

    public:
        LiveboardDepartureDelegate(const QMap<QString, StationPointer>& iStations, QWidget *parent = 0) : QStyledItemDelegate(parent), mStations(iStations)
        {

        }

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    private:
        // Member data
        const QMap<QString, StationPointer>& mStations;
    };
}

#endif // LIVEBOARDDEPARTUREDELEGATE_H
