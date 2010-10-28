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
    Q_ASSERT(qVariantCanConvert<ConnectionPointer>(iIndex.data(ConnectionRole)));
    ConnectionPointer tConnection = qVariantValue<ConnectionPointer>(iIndex.data(ConnectionRole));

    QStyledItemDelegate::paint(iPainter, iOption, iIndex);

    QRect rect = iOption.rect;
    rect.adjust(20, 8, -20, -8);
    QFont font = iOption.font;

    iPainter->save();

    // Header
    QString tHeader = tConnection->transfer().departure.station % tr(" to ") % tConnection->transfer().arrival.station;
    iPainter->drawText(rect, Qt::AlignTop | Qt::AlignLeft, tHeader);

    // Transfer count
    QString tTransfers;
    switch (tConnection->transfers().size())
    {
    case 0:
        tTransfers = tr("Direct");
        break;
    case 1:
        tTransfers = QString::number(tConnection->transfers().size()) % " " % tr("transfer");
        break;
    default:
        tTransfers = QString::number(tConnection->transfers().size()) % " " % tr("transfers");
    }
    iPainter->setPen(iOption.palette.mid().color());
    font.setPointSizeF(font.pointSizeF() * 0.70);
    iPainter->setFont(font);
    iPainter->drawText(rect, Qt::AlignBottom | Qt::AlignLeft, tTransfers);

    iPainter->restore();
}
