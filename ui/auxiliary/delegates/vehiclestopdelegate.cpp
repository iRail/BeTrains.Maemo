//
// Configuration
//

// Includes
#include "vehiclestopdelegate.h"
#include "api/vehicle.h"
#include "ui/global.h"
#include <QStringBuilder>

// Namespaces
using namespace iRail;


void VehicleStopDelegate::paint(QPainter *iPainter, const QStyleOptionViewItem &iOption, const QModelIndex &iIndex) const
{
    QStyledItemDelegate::paint(iPainter, iOption, iIndex);

    if (! qVariantCanConvert<Vehicle::Stop>(iIndex.data(VehicleStopRole)))
    {
        qWarning() << "! " << Q_FUNC_INFO << "cannot paint" << iIndex.data().typeName();
        return;
    }
    Vehicle::Stop tStop = qVariantValue<Vehicle::Stop>(iIndex.data(VehicleStopRole));

    QStyledItemDelegate::paint(iPainter, iOption, iIndex);

    QRect rect = iOption.rect;
    rect.adjust(20, 8, -20, -8);
    QFont font = iOption.font;

    iPainter->save();

    // Station
    QString tStation = mStations[tStop.station]->name();
    iPainter->drawText(rect, Qt::AlignTop | Qt::AlignLeft, tStation);

    // Platform
    if (tStop.platform != 0)
    {
        QString tPlatform = tr("Platform") % " " % QString::number(tStop.platform);
        iPainter->drawText(rect, Qt::AlignTop | Qt::AlignRight, tPlatform);
    }

    // Time
    QString tTime = tStop.datetime.toLocalTime().time().toString(Qt::DefaultLocaleShortDate);
    iPainter->setPen(iOption.palette.mid().color());
    font.setPointSizeF(font.pointSizeF() * 0.70);
    iPainter->setFont(font);
    iPainter->drawText(rect, Qt::AlignBottom | Qt::AlignLeft, tTime);

    // TODO: delay

    iPainter->restore();
}

