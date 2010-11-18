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
    QStyledItemDelegate::paint(iPainter, iOption, iIndex);

    if (! qVariantCanConvert<Liveboard::Departure>(iIndex.data(LiveboardDepartureRole)))
    {
        // Warn
        return;
    }
    Liveboard::Departure tLiveboardDeparture = qVariantValue<Liveboard::Departure>(iIndex.data(LiveboardDepartureRole));

    QRect rect = iOption.rect;
    rect.adjust(20, 8, -20, -8);
    QFont font = iOption.font;

    iPainter->save();

    // Header
    QString tHeader = mStations[tLiveboardDeparture.station]->name();
    iPainter->drawText(rect, Qt::AlignTop | Qt::AlignLeft, tHeader);

    // Platform
    QString tPlatform = tr("Platform") % " " % QString::number(tLiveboardDeparture.platform);
    iPainter->drawText(rect, Qt::AlignTop | Qt::AlignRight, tPlatform);

    // Hours
    QString tHours = tLiveboardDeparture.datetime.toLocalTime().time().toString(Qt::DefaultLocaleShortDate);
    iPainter->setPen(iOption.palette.mid().color());
    font.setPointSizeF(font.pointSizeF() * 0.70);
    iPainter->setFont(font);
    iPainter->drawText(rect, Qt::AlignBottom | Qt::AlignLeft, tHours);

    iPainter->restore();
}
