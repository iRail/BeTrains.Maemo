//
// Configuration
//

// Includes
#include "connectiondelegate.h"
#include "ui/global.h"
#include <QStringBuilder>

// Namespaces
using namespace iRail;


void ConnectionDelegate::paint(QPainter *iPainter, const QStyleOptionViewItem &iOption, const QModelIndex &iIndex) const
{
    //
    // Fetch data
    //

    QStyledItemDelegate::paint(iPainter, iOption, iIndex);

    if (qVariantCanConvert<ConnectionPointer>(iIndex.data(ConnectionRole)))
    {
        ConnectionPointer tConnection = qVariantValue<ConnectionPointer>(iIndex.data(ConnectionRole));
        paint(iPainter, iOption, tConnection);
    }
    else if (qVariantCanConvert<Connection::Line>(iIndex.data(ConnectionLineRole)))
    {
        Connection::Line tConnectionLine = qVariantValue<Connection::Line>(iIndex.data(ConnectionLineRole));
        paint(iPainter, iOption, tConnectionLine);
    }
    else
    {
        qWarning() << "! " << Q_FUNC_INFO << "cannot paint" << iIndex.data().typeName();
        return;
    }

}


void ConnectionDelegate::paint(QPainter *iPainter, const QStyleOptionViewItem &iOption, ConnectionPointer iConnection) const
{
    //
    // Configure layout
    //


    // Main drawing rectangle
    QRect rect = iOption.rect;
    rect.adjust(0, 8, -20, -8);

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
    QString tDeparture = iConnection->departure().datetime.toLocalTime().time().toString(Qt::DefaultLocaleShortDate);
    iPainter->drawText(rect_c1, Qt::AlignTop | Qt::AlignHCenter, tDeparture);

    // Arrow
    iPainter->setPen(iOption.palette.mid().color());
    iPainter->drawText(rect_c2, Qt::AlignTop | Qt::AlignHCenter, "-");

    // Arrival
    iPainter->setPen(iOption.palette.foreground().color());
    QString tArrival = iConnection->arrival().datetime.toLocalTime().time().toString(Qt::DefaultLocaleShortDate);
    iPainter->drawText(rect_c3, Qt::AlignTop | Qt::AlignHCenter, tArrival);

    // Connection
    QString tConnectionString = mStations[iConnection->departure().station]->name() % " " % tr("to") % " " % mStations[iConnection->arrival().station]->name();
    iPainter->drawText(rect_c4, Qt::AlignTop | Qt::AlignLeft, tConnectionString);

    // Transfers
    QString tTransfers;
    switch (iConnection->lines().size())
    {
    case 1:
        tTransfers = tr("Direct");
        break;
    default:
        tTransfers = QString::number(iConnection->lines().size() - 1) % " " % tr("transfer(s)", "", (iConnection->lines().size() - 1));
        break;
    }
    iPainter->drawText(rect, Qt::AlignTop | Qt::AlignRight, tTransfers);

    // Delay
    if (iConnection->departure().delay != 0 || iConnection->arrival().delay != 0)
    {
        iPainter->setPen(Qt::red);
        iPainter->setFont(font_small);

        // Delay time
        QString tDelay = tr("%n minute(s) delay", "", (iConnection->departure().delay + iConnection->arrival().delay) / 60);
        iPainter->drawText(rect_c4, Qt::AlignBottom | Qt::AlignLeft, tDelay);

        // New departure hour
        if (iConnection->departure().delay  != 0)
        {
            QString tHours = iConnection->departure().datetime.addSecs(iConnection->departure().delay ).toLocalTime().time().toString(Qt::DefaultLocaleShortDate);
            iPainter->drawText(rect_c1, Qt::AlignBottom | Qt::AlignHCenter, tHours);
        }

        // New arrival hour
        if (iConnection->arrival().delay  != 0)
        {
            QString tHours = iConnection->arrival().datetime.addSecs(iConnection->arrival().delay ).toLocalTime().time().toString(Qt::DefaultLocaleShortDate);
            iPainter->drawText(rect_c3, Qt::AlignBottom | Qt::AlignHCenter, tHours);
        }
    }

    iPainter->restore();
}
void ConnectionDelegate::paint(QPainter *iPainter, const QStyleOptionViewItem &iOption, Connection::Line iLine) const
{
    //
    // Configure layout
    //


    // Main drawing rectangle
    QRect rect = iOption.rect;
    rect.adjust(-20, 8, -20, -8);

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
    QString tDeparture = iLine.departure.datetime.toLocalTime().time().toString(Qt::DefaultLocaleShortDate);
    iPainter->drawText(rect_c1, Qt::AlignTop | Qt::AlignHCenter, tDeparture);

    // Arrow
    iPainter->setPen(iOption.palette.mid().color());
    iPainter->drawText(rect_c2, Qt::AlignTop | Qt::AlignHCenter, "-");

    // Arrival
    iPainter->setPen(iOption.palette.foreground().color());
    QString tArrival = iLine.arrival.datetime.toLocalTime().time().toString(Qt::DefaultLocaleShortDate);
    iPainter->drawText(rect_c3, Qt::AlignTop | Qt::AlignHCenter, tArrival);

    // Connection
    QString tConnectionString = mStations[iLine.departure.station]->name() % " " % tr("to") % " " % mStations[iLine.arrival.station]->name();
    iPainter->drawText(rect_c4, Qt::AlignTop | Qt::AlignLeft, tConnectionString);

    // Delay
    if (iLine.departure.delay != 0 || iLine.arrival.delay != 0)
    {
        iPainter->setPen(Qt::red);
        iPainter->setFont(font_small);

        // Delay time
        QString tDelay = tr("%n minute(s) delay", "", (iLine.departure.delay + iLine.arrival.delay) / 60);
        iPainter->drawText(rect_c4, Qt::AlignBottom | Qt::AlignLeft, tDelay);

        // New departure hour
        if (iLine.departure.delay  != 0)
        {
            QString tHours = iLine.departure.datetime.addSecs(iLine.departure.delay ).toLocalTime().time().toString(Qt::DefaultLocaleShortDate);
            iPainter->drawText(rect_c1, Qt::AlignBottom | Qt::AlignHCenter, tHours);
        }

        // New arrival hour
        if (iLine.arrival.delay  != 0)
        {
            QString tHours = iLine.arrival.datetime.addSecs(iLine.arrival.delay ).toLocalTime().time().toString(Qt::DefaultLocaleShortDate);
            iPainter->drawText(rect_c3, Qt::AlignBottom | Qt::AlignHCenter, tHours);
        }
    }

    iPainter->restore();
}
