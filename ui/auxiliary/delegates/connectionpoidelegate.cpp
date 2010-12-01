//
// Configuration
//

// Includes
#include "connectionpoidelegate.h"
#include "api/connection.h"
#include "ui/global.h"
#include <QStringBuilder>

// Namespaces
using namespace iRail;


void ConnectionPOIDelegate::paint(QPainter *iPainter, const QStyleOptionViewItem &iOption, const QModelIndex &iIndex) const
{
    QStyledItemDelegate::paint(iPainter, iOption, iIndex);

    if (! qVariantCanConvert<Connection::POI>(iIndex.data(ConnectionPOIRole)))
    {
        qWarning() << "! " << Q_FUNC_INFO << "cannot paint" << iIndex.data().typeName();
        return;
    }
    Connection::POI tConnectionPOI = qVariantValue<Connection::POI>(iIndex.data(ConnectionPOIRole));

    QStyledItemDelegate::paint(iPainter, iOption, iIndex);

    QRect rect = iOption.rect;
    rect.adjust(20, 8, -20, -8);
    QFont font = iOption.font;

    iPainter->save();

    // Station
    QString tStation = mStations[tConnectionPOI.station]->name();
    iPainter->drawText(rect, Qt::AlignTop | Qt::AlignLeft, tStation);

    // Platform (TODO: might not be available for stops between endpoints)
    QString tPlatform = tr("Platform") % " " % QString::number(tConnectionPOI.platform);
    iPainter->drawText(rect, Qt::AlignTop | Qt::AlignRight, tPlatform);

    // Time
    QString tTime = tConnectionPOI.datetime.toLocalTime().time().toString(Qt::DefaultLocaleShortDate);
    iPainter->setPen(iOption.palette.mid().color());
    font.setPointSizeF(font.pointSizeF() * 0.70);
    iPainter->setFont(font);
    iPainter->drawText(rect, Qt::AlignBottom | Qt::AlignLeft, tTime);

    iPainter->restore();
}

