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

    // Divide drawing area in columns
    QRect rect_c1 = rect.adjusted(0, 0, -670, 0);
    qDebug() << rect_c1;
    QRect rect_c2 = rect_c1.adjusted(rect_c1.width(), 0, rect_c1.width(), 0);
    qDebug() << rect_c2;
    QRect rect_c3 = rect_c2.adjusted(rect_c2.width(), 0, rect_c2.width()+500, 0);
    qDebug() << rect_c3;

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
    iPainter->drawText(rect_c1, Qt::AlignTop | Qt::AlignHCenter, tHours);

    // "to" text
    iPainter->setPen(iOption.palette.mid().color());
    iPainter->drawText(rect_c2, Qt::AlignTop | Qt::AlignHCenter, tr("to"));

    // Station
    iPainter->setPen(iOption.palette.foreground().color());
    QString tStation = mStations[tLiveboardDeparture.station]->name();
    iPainter->drawText(rect_c3, Qt::AlignTop | Qt::AlignLeft, tStation);

    // Platform
    QString tPlatform = tr("Platform") % " " % QString::number(tLiveboardDeparture.platform);
    iPainter->drawText(rect, Qt::AlignTop | Qt::AlignRight, tPlatform);

    // Delay
    if (tLiveboardDeparture.delay != 0)
    {
        iPainter->setPen(Qt::red);
        iPainter->setFont(font_small);

        // Delay time
        QString tDelay = tr("%n minute(s) delay", "", tLiveboardDeparture.delay / 60);
        iPainter->drawText(rect_c3, Qt::AlignBottom | Qt::AlignLeft, tDelay);

        // New hour
        QString tHours = tLiveboardDeparture.datetime.addSecs(tLiveboardDeparture.delay).toLocalTime().time().toString(Qt::DefaultLocaleShortDate);
        iPainter->drawText(rect_c1, Qt::AlignBottom | Qt::AlignHCenter, tHours);

    }

    iPainter->restore();
}
