//
// Configuration
//

// Includes
#include "connectionviewimpl.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFont>
#include "ui/global.h"
#include "ui/auxiliary/delegates/connectiondelegate.h"
#include <QHeaderView>

// Namespaces
using namespace iRail;


//
// Confuction and destruction
//

ConnectionViewImpl::ConnectionViewImpl() : ConnectionView()
{
    qDebug() << "+ " << Q_FUNC_INFO;
    proxy = new GenericViewImpl(this);

    // Initialisation
    init_ui();
}


//
// UI events
//

void ConnectionViewImpl::do_lstConnections_activated(QModelIndex iIndex)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    if (qVariantCanConvert<ConnectionPointer>(iIndex.data(ConnectionRole)))
    {
        // ConnectionPointer tConnection = qVariantValue<ConnectionPointer>(iIndex.data(ConnectionRole));
        /* Maybe show a vehicle view of _all_ the lines? But only when expanded! */

        // Currently: just expand/collapse the item
        if (mView->isExpanded(iIndex))
            mView->collapse(iIndex);
        else
            mView->expand(iIndex);
    }
    else if (qVariantCanConvert<Connection::Line>(iIndex.data(ConnectionLineRole)))
    {
        Connection::Line tConnectionLine = qVariantValue<Connection::Line>(iIndex.data(ConnectionLineRole));
        emit launchVehicle(tConnectionLine);
    }

}


//
// Controller actions
//

void ConnectionViewImpl::reset()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    populateModel(QList<ConnectionPointer>());
}

void ConnectionViewImpl::load(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    emit downloadConnections(iConnectionRequest);
}

void ConnectionViewImpl::setStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;
    stopLoader();

    mStations = *iStations;
    delete iStations;

    populateModel(*tConnections);
    delete tConnections;
}

void ConnectionViewImpl::setConnections(QList<ConnectionPointer>* iConnections)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    tConnections = iConnections;

    emit downloadStations();
}


//
// Initialisation
//


void ConnectionViewImpl::init_ui()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Window settings
    setWindowTitle(QString(tr("Connections")));

    // Main layout
    QVBoxLayout *mUILayout = new QVBoxLayout(centralWidget());
    mUILayout->setAlignment(Qt::AlignTop);

    // Create the connection list model
    mModel = new QStandardItemModel(0, 1);

    // Create the connection list view
    mView = new QTreeView();
    mView->header()->hide();
    mView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mView->setModel(mModel);
    mView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mView->setSelectionMode(QAbstractItemView::SingleSelection);
    mView->setItemDelegate(new ConnectionDelegate(mStations, mView));
    mView->setExpandsOnDoubleClick(false); // We'll do this manually on activation, a single click on Maemo.
                                           // Don't use setItemsExpandable, as that disables the icon click as well
    connect(mView, SIGNAL(activated(QModelIndex)), this, SLOT(do_lstConnections_activated(QModelIndex)));
    mUILayout->addWidget(mView);

    // Create the connection list view dummy
    mViewDummy = new QLabel(tr("No connections to be shown."));
    mViewDummy->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mViewDummy->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
    mViewDummy->setEnabled(false);
    QFont font;
    font.setPointSize(24);
    mViewDummy->setFont(font);
    mUILayout->addWidget(mViewDummy);
}


//
// Auxiliary
//

void ConnectionViewImpl::populateModel(const QList<ConnectionPointer>& iConnections)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mModel->clear();
    if (iConnections.size() > 0)
    {
        for (int i = 0; i < iConnections.size(); i++)
        {
            ConnectionPointer tConnection = iConnections.at(i);
            QStandardItem *tConnectionItem = new QStandardItem();
            tConnectionItem->setData(QVariant::fromValue(tConnection), ConnectionRole);

            foreach (Connection::Line tLine, tConnection->lines())
            {
                QStandardItem *tLineItem = new QStandardItem();
                tLineItem->setData(QVariant::fromValue(tLine), ConnectionLineRole);
                tConnectionItem->appendRow(tLineItem);
            }

            mModel->appendRow(tConnectionItem);
        }

        mViewDummy->setVisible(false);
        mView->setVisible(true);
    }
    else
    {
        mViewDummy->setVisible(true);
        mView->setVisible(false);
    }
}
