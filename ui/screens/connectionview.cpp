//
// Configuration
//

// Includes
#include "connectionview.h"
#include "ui/auxiliary/delegates/connectionrequestdelegate.h"
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
// Construction and destruction
//

ConnectionView::ConnectionView(QWidget* iParent) : GenericView(iParent)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Initialisation
    this->hide();
    init_ui();
}

void ConnectionView::load(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;
    startLoader();

    emit downloadConnections(iConnectionRequest);
}

void ConnectionView::load(const QMap<QString, StationPointer>& iStations, const QList<ConnectionPointer>& iConnections)
{
    qDebug() << "+ " << Q_FUNC_INFO;
    stopLoader();

    mStations = iStations;  // TODO: also in load?

    // Show the results
    populateModel(iConnections);
}


//
// UI events
//

void ConnectionView::do_lstConnections_doubleClicked(QModelIndex iIndex)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    if (qVariantCanConvert<ConnectionPointer>(iIndex.data(ConnectionRole)))
    {
        ConnectionPointer tConnection = qVariantValue<ConnectionPointer>(iIndex.data(ConnectionRole));

        if (tConnection->lines().count() > 1)
        {
            mView->expand(iIndex);
        }
        else
        {
            Connection::Line tConnectionLine = tConnection->lines()[0];
            emit launchVehicle(tConnectionLine);
        }
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

void ConnectionView::setStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    load(*iStations, *tConnections);
    delete iStations;
    delete tConnections;
}

void ConnectionView::setConnections(QList<ConnectionPointer>* iConnections)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    tConnections = iConnections;
    emit downloadStations();
}


//
// Initialisation
//


void ConnectionView::init_ui()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Window settings
    this->setWindowTitle(QString(tr("Connections")));

    // Main layout
    QVBoxLayout *mUILayout = new QVBoxLayout(this);
    mUILayout->setAlignment(Qt::AlignTop);

    // Populate the history list model
    mModel = new QStandardItemModel(0, 1);

    // Create the history listview
    QTreeView *tView = new QTreeView();
    tView->header()->hide();
    tView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tView->setModel(mModel);
    tView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tView->setSelectionMode(QAbstractItemView::SingleSelection);
    tView->setItemDelegate(new ConnectionDelegate(mStations));
    connect(tView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(do_lstConnections_doubleClicked(QModelIndex)));
    mUILayout->addWidget(tView);
}


//
// Auxiliary
//

void ConnectionView::populateModel(const QList<ConnectionPointer>& iConnections)
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

            if (tConnection->lines().count() > 1)
            {
                foreach (Connection::Line tLine, tConnection->lines())
                {
                    QStandardItem *tLineItem = new QStandardItem();
                    tLineItem->setData(QVariant::fromValue(tLine), ConnectionLineRole);
                    tConnectionItem->appendRow(tLineItem);
                }
            }

            mModel->appendRow(tConnectionItem);
        }
    }
    else
    {
        QStandardItem *tDummy = new QStandardItem(tr("No connections found."));
        tDummy->setEditable(false);
        tDummy->setSelectable(false);
        mModel->appendRow(tDummy);
    }
}
