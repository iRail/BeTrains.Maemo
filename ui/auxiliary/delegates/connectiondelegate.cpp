//
// Configuration
//

// Includes
#include "connectiondelegate.h"
#include "api/connection.h"
#include "ui/global.h"
#include <QStringBuilder>

// Namespaces
using namespace iRail;


void ConnectionDelegate::paint(QPainter *iPainter, const QStyleOptionViewItem &iOption, const QModelIndex &iIndex) const
{
    //
    // Fetch data
    //

    if (! qVariantCanConvert<ConnectionPointer>(iIndex.data(ConnectionRole)))
    {
        // Warn
        return;
    }

    ConnectionPointer tConnection = qVariantValue<ConnectionPointer>(iIndex.data(ConnectionRole));


    //
    // Configure layout
    //

    QStyledItemDelegate::paint(iPainter, iOption, iIndex);

    // Main drawing rectangle
    QRect rect = iOption.rect;
    rect.adjust(20, 8, -20, -8);

    // Divide drawing area in columns
    QRect rect_c1 = rect.adjusted(0, 0, -660, 0);
    QRect rect_c2 = rect_c1.adjusted(rect_c1.width(), 0, 20, 0);
    QRect rect_c3 = rect_c2.adjusted(rect_c2.width(), 0, rect_c1.width(), 0);
    QRect rect_c4 = rect_c3.adjusted(rect_c3.width()+25, 0, rect_c3.width()+400, 0);

    // Main font
    QFont font = iOption.font;

    // Small font
    QFont font_small = font;
    font_small.setPointSizeF(font.pointSizeF() * 0.70);


    //
    // Draw text
    //

    iPainter->save();

    // Departure
    QString tDeparture = tConnection->departure().datetime.toLocalTime().time().toString(Qt::DefaultLocaleShortDate);
    iPainter->drawText(rect_c1, Qt::AlignTop | Qt::AlignHCenter, tDeparture);

    // Arrow
    iPainter->setPen(iOption.palette.mid().color());
    iPainter->drawText(rect_c2, Qt::AlignTop | Qt::AlignHCenter, "-");

    // Arrival
    iPainter->setPen(iOption.palette.foreground().color());
    QString tArrival = tConnection->arrival().datetime.toLocalTime().time().toString(Qt::DefaultLocaleShortDate);
    iPainter->drawText(rect_c3, Qt::AlignTop | Qt::AlignHCenter, tArrival);

    // Connection
    QString tConnectionString = mStations[tConnection->departure().station]->name() % " " % tr("to") % " " % mStations[tConnection->arrival().station]->name();
    iPainter->drawText(rect_c4, Qt::AlignTop | Qt::AlignLeft, tConnectionString);

    // Transfers
    QString tTransfers;
    switch (tConnection->lines().size())
    {
    case 1:
        tTransfers = tr("Direct");
        break;
    default:
        tTransfers = QString::number(1) % " " % tr("transfer(s)", "", (tConnection->lines().size() - 1));
        break;
    }
    iPainter->drawText(rect, Qt::AlignTop | Qt::AlignRight, tTransfers);

    // Delay
    if (tConnection->departure().delay != 0 || tConnection->arrival().delay != 0)
    {
        iPainter->setPen(Qt::red);
        iPainter->setFont(font_small);

        // Delay time
        QString tDelay = tr("%n minute(s) delay", "", (tConnection->departure().delay + tConnection->arrival().delay) / 60);
        iPainter->drawText(rect_c4, Qt::AlignBottom | Qt::AlignLeft, tDelay);

        // New departure hour
        if (tConnection->departure().delay  != 0)
        {
            QString tHours = tConnection->departure().datetime.addSecs(tConnection->departure().delay ).toLocalTime().time().toString(Qt::DefaultLocaleShortDate);
            iPainter->drawText(rect_c1, Qt::AlignBottom | Qt::AlignHCenter, tHours);
        }

        // New arrival hour
        if (tConnection->arrival().delay  != 0)
        {
            QString tHours = tConnection->arrival().datetime.addSecs(tConnection->arrival().delay ).toLocalTime().time().toString(Qt::DefaultLocaleShortDate);
            iPainter->drawText(rect_c3, Qt::AlignBottom | Qt::AlignHCenter, tHours);
        }
    }

    iPainter->restore();
}
