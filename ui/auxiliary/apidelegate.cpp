//
// Configuration
//

// Includes
#include "apidelegate.h"
#include "api/connection.h"
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include "ui/global.h"
#include <QStringBuilder>
#define QT_USE_FAST_CONCATENATION

// Namespaces
using namespace iRail;


void APIDelegate::paint(QPainter *iPainter, const QStyleOptionViewItem &iOption, const QModelIndex &iIndex) const
{
    QString tHTML = getHTML(iIndex);

    if (tHTML.length() > 0)
        paintHTML(iPainter, iOption, iIndex, tHTML);
    else
        QStyledItemDelegate::paint(iPainter, iOption, iIndex);
}

QSize APIDelegate::sizeHint(const QStyleOptionViewItem &iOption, const QModelIndex &iIndex) const
{
    QString tHTML = getHTML(iIndex);

    if (tHTML.length() > 0)
        return sizeHintHTML(iOption, iIndex, tHTML);
    else
        return QStyledItemDelegate::sizeHint(iOption, iIndex);
}

QString APIDelegate::getHTML(const QModelIndex &iIndex) const
{
    QString oHTML = "";

    if (qVariantCanConvert<ConnectionPointer>(iIndex.data(ConnectionRole)))
    {
        ConnectionPointer tConnection = qVariantValue<ConnectionPointer>(iIndex.data(ConnectionRole));

        oHTML += "<table>";

        oHTML += "<tr>";
        oHTML += "<td>" % tConnection->transfer().departure.station % " - " % tConnection->transfer().arrival.station % "</td>";
        oHTML += "<td>" % QString(tConnection->transfers().size()) % "</td>";
        oHTML += "</tr>";

        oHTML += "<tr>";
        oHTML += "<td>" % tConnection->transfer().departure.datetime.time().toString(Qt::LocaleDate) % " - " + tConnection->transfer().arrival.datetime.time().toString(Qt::LocaleDate) % "</td>";
        int tSeconds = tConnection->transfer().departure.datetime.secsTo(tConnection->transfer().arrival.datetime);
        oHTML += "<td>" % QString(tSeconds) % "</td>";
        oHTML += "</tr>";
    }

    return oHTML;

}

void APIDelegate::paintHTML(QPainter *iPainter, const QStyleOptionViewItem &iOption, const QModelIndex &iIndex, const QString& iHTML) const
{
    QStyleOptionViewItemV4 tOptions = iOption;
    initStyleOption(&tOptions, iIndex);

    iPainter->save();

    QTextDocument tDocument;
    tDocument.setHtml(iHTML);

    tOptions.text = "";
    tOptions.widget->style()->drawControl(QStyle::CE_ItemViewItem, &tOptions, iPainter);

    // shift text right to make icon visible
    QSize iconSize = tOptions.icon.actualSize(tOptions.rect.size());
    iPainter->translate(tOptions.rect.left()+iconSize.width(), tOptions.rect.top());
    QRect clip(0, 0, tOptions.rect.width()+iconSize.width(), tOptions.rect.height());

    //doc.drawContents(painter, clip);

    iPainter->setClipRect(clip);
    QAbstractTextDocumentLayout::PaintContext ctx;

    // set text color to red for selected item
    if (iOption.state & QStyle::State_Selected)
        ctx.palette.setColor(QPalette::Text, tOptions.palette.color(QPalette::Active, QPalette::HighlightedText));

    ctx.clip = clip;
    tDocument.documentLayout()->draw(iPainter, ctx);

    iPainter->restore();
}

QSize APIDelegate::sizeHintHTML(const QStyleOptionViewItem &iOption, const QModelIndex &iIndex, const QString& iHTML) const
{
    QStyleOptionViewItemV4 options = iOption;
    initStyleOption(&options, iIndex);

    QTextDocument doc;
    doc.setHtml(iHTML);
    doc.setTextWidth(options.rect.width());
    return QSize(doc.idealWidth(), doc.size().height());
}

