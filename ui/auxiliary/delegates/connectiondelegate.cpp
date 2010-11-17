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
    QStyledItemDelegate::paint(iPainter, iOption, iIndex);

    if (! qVariantCanConvert<ConnectionPointer>(iIndex.data(ConnectionRole)))
    {
        // Warn
        return;
    }
    ConnectionPointer tConnection = qVariantValue<ConnectionPointer>(iIndex.data(ConnectionRole));

    QRect rect = iOption.rect;
    rect.adjust(20, 8, -20, -8);
    QFont font = iOption.font;

    iPainter->save();

    // Header
    QString tHeader = tConnection->departure().station % tr(" to ") % tConnection->arrival().station;
    iPainter->drawText(rect, Qt::AlignTop | Qt::AlignLeft, tHeader);

    // Transfer count
    QString tTransfers;
    switch (tConnection->lines().size())
    {
    case 1:
        tTransfers = tr("Direct");
        break;
    case 2:
        tTransfers = QString::number(1) % " " % tr("transfer");
        break;
    default:
        tTransfers = QString::number(tConnection->lines().size() - 1) % " " % tr("transfers");
    }
    iPainter->drawText(rect, Qt::AlignTop | Qt::AlignRight, tTransfers);

    // Hours
    QString tHours = tConnection->departure().datetime.toLocalTime().time().toString(Qt::DefaultLocaleShortDate)
            % " - " % tConnection->arrival().datetime.toLocalTime().time().toString(Qt::DefaultLocaleShortDate);
    iPainter->setPen(iOption.palette.mid().color());
    font.setPointSizeF(font.pointSizeF() * 0.70);
    iPainter->setFont(font);
    iPainter->drawText(rect, Qt::AlignBottom | Qt::AlignLeft, tHours);

    iPainter->restore();
}
