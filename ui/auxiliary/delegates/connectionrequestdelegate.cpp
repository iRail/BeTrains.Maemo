//
// Configuration
//

// Includes
#include "connectionrequestdelegate.h"
#include "api/connectionrequest.h"
#include "ui/global.h"
#include <QStringBuilder>

// Namespaces
using namespace iRail;


void ConnectionRequestDelegate::paint(QPainter *iPainter, const QStyleOptionViewItem &iOption, const QModelIndex &iIndex) const
{
    //
    // Fetch data
    //

    QStyledItemDelegate::paint(iPainter, iOption, iIndex);

    if (! qVariantCanConvert<ConnectionRequestPointer>(iIndex.data(ConnectionRequestRole)))
    {
        qWarning() << "! " << Q_FUNC_INFO << "cannot paint" << iIndex.data().typeName();
        return;
    }

    ConnectionRequestPointer tConnectionRequest = qVariantValue<ConnectionRequestPointer>(iIndex.data(ConnectionRequestRole));


    //
    // Configure layout
    //

    // Main drawing rectangle
    QRect rect = iOption.rect;
    rect.adjust(20, 8, -20, -8);

    // Main font
    QFont font = iOption.font;


    //
    // Draw text
    //

    iPainter->save();

    // Header
    QString tHeader = tConnectionRequest->origin() % tr(" to ") % tConnectionRequest->destination();
    if (tConnectionRequest->timed())
        iPainter->drawText(rect, Qt::AlignTop | Qt::AlignLeft, tHeader);
    else
        iPainter->drawText(rect, Qt::AlignVCenter | Qt::AlignLeft, tHeader);

    // Time
    if (tConnectionRequest->timed())
    {
        QString tTime = (tConnectionRequest->time()->type == ConnectionRequest::Arrival ? tr("Arrival") : tr("Departure"))
                % " " % tr("at") % " " % tConnectionRequest->time()->datetime.toString(Qt::DefaultLocaleLongDate);

        iPainter->setPen(iOption.palette.mid().color());
        font.setPointSizeF(font.pointSizeF() * 0.70);
        iPainter->setFont(font);
        iPainter->drawText(rect, Qt::AlignBottom | Qt::AlignLeft, tTime);
    }

    iPainter->restore();
}
