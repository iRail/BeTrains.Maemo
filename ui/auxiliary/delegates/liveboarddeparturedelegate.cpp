//
// Configuration
//

// Includes
#include "liveboarddeparturedelegate.h"
#include "api/liveboard.h"
#include "ui/global.h"
#include <QStringBuilder>

// Namespaces
using namespace iRail;


void LiveboardDepartureDelegate::paint(QPainter *iPainter, const QStyleOptionViewItem &iOption, const QModelIndex &iIndex) const
{
    //
    // Fetch data
    //

    if (! qVariantCanConvert<Liveboard::Departure>(iIndex.data(LiveboardDepartureRole)))
    {
        // Warn
        return;
    }

    Liveboard::Departure tLiveboardDeparture = qVariantValue<Liveboard::Departure>(iIndex.data(LiveboardDepartureRole));


    //
    // Configure layout
    //

    QStyledItemDelegate::paint(iPainter, iOption, iIndex);

    // Main drawing rectangle
    QRect rect = iOption.rect;
    rect.adjust(20, 8, -20, -8);

    // Main font
    QFont font = iOption.font;

    // Small font
    QFont font_small = font;
    font_small.setPointSizeF(font.pointSizeF() * 0.70);


    //
    // Draw text
    //

    iPainter->save();

    // Hour
    QString tHours = tLiveboardDeparture.datetime.toLocalTime().time().toString(Qt::DefaultLocaleShortDate);
    iPainter->drawText(rect, Qt::AlignTop | Qt::AlignLeft, tHours);

    // "to" text
    iPainter->setPen(iOption.palette.mid().color());
    iPainter->drawText(rect.adjusted(80, 0, 0, 0), Qt::AlignTop | Qt::AlignLeft, tr("to"));

    // Station
    iPainter->setPen(iOption.palette.foreground().color());
    QString tStation = mStations[tLiveboardDeparture.station]->name();
    iPainter->drawText(rect.adjusted(125, 0, 0, 0), Qt::AlignTop | Qt::AlignLeft, tStation);

    // Platform
    QString tPlatform = tr("Platform") % " " % QString::number(tLiveboardDeparture.platform);
    iPainter->drawText(rect, Qt::AlignTop | Qt::AlignRight, tPlatform);

    // Delay
    if (tLiveboardDeparture.delay != 0)
    {
        QString tDelay = tr("%n minute(s) delay", "", tLiveboardDeparture.delay / 60);
        iPainter->setPen(Qt::red);
        iPainter->setFont(font_small);
        iPainter->drawText(rect.adjusted(125, 0, 0, 0), Qt::AlignBottom | Qt::AlignLeft, tDelay);
    }

    iPainter->restore();
}
