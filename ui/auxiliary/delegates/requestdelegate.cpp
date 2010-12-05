//
// Configuration
//

// Includes
#include "requestdelegate.h"
#include "api/connectionrequest.h"
#include "ui/global.h"
#include <QStringBuilder>

// Namespaces
using namespace iRail;


void RequestDelegate::paint(QPainter *iPainter, const QStyleOptionViewItem &iOption, const QModelIndex &iIndex) const
{
    //
    // Fetch data
    //

    QStyledItemDelegate::paint(iPainter, iOption, iIndex);

    if (qVariantCanConvert<LiveboardRequestPointer>(iIndex.data(LiveboardRequestRole)))
    {
        LiveboardRequestPointer tLiveboardRequest = qVariantValue<LiveboardRequestPointer>(iIndex.data(LiveboardRequestRole));
        paint(iPainter, iOption, iIndex, tLiveboardRequest);
    }
    else if (qVariantCanConvert<ConnectionRequestPointer>(iIndex.data(ConnectionRequestRole)))
    {
        ConnectionRequestPointer tConnectionRequestPointer = qVariantValue<ConnectionRequestPointer>(iIndex.data(ConnectionRequestRole));
        paint(iPainter, iOption, iIndex, tConnectionRequestPointer);
    }
    else
        return;
}

void RequestDelegate::paint(QPainter *iPainter, const QStyleOptionViewItem &iOption, const QModelIndex &iIndex, ConnectionRequestPointer iConnectionRequest) const
{
    //
    // Configure layout
    //

    // Main drawing rectangle
    QRect rect = iOption.rect;
    rect.adjust(60, 8, -20, -8);

    // Main font
    QFont font = iOption.font;


    //
    // Draw text
    //

    iPainter->save();

    // Header
    QString tHeader = tr("Connections from") % " " % iConnectionRequest->origin() + " " + tr("to") + " " + iConnectionRequest->destination(); // TODO: stringbuider??
    if (iConnectionRequest->timed())
        iPainter->drawText(rect, Qt::AlignTop | Qt::AlignLeft, tHeader);
    else
        iPainter->drawText(rect, Qt::AlignVCenter | Qt::AlignLeft, tHeader);

    // Time
    if (iConnectionRequest->timed())
    {
        QString tTime = (iConnectionRequest->time()->type == ConnectionRequest::Arrival ? tr("Arrival") : tr("Departure"))
                % " " % tr("at") % " " % iConnectionRequest->time()->datetime.toString(Qt::DefaultLocaleLongDate);

        iPainter->setPen(iOption.palette.mid().color());
        font.setPointSizeF(font.pointSizeF() * 0.70);
        iPainter->setFont(font);
        iPainter->drawText(rect, Qt::AlignBottom | Qt::AlignLeft, tTime);
    }

    iPainter->restore();
}



void RequestDelegate::paint(QPainter *iPainter, const QStyleOptionViewItem &iOption, const QModelIndex &iIndex, LiveboardRequestPointer iLiveboardRequest) const
{
    //
    // Configure layout
    //

    // Main drawing rectangle
    QRect rect = iOption.rect;
    rect.adjust(60, 8, -20, -8);

    // Main font
    QFont font = iOption.font;


    //
    // Draw text
    //

    iPainter->save();

    // Header
    QString tHeader = tr("Departures at") % " " % mStations[iLiveboardRequest->station()]->name();
    iPainter->drawText(rect, Qt::AlignVCenter | Qt::AlignLeft, tHeader);

    iPainter->restore();
}
