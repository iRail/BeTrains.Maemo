//
// Configuration
//

// Includes
#include "connectionresultwidget.h"
#include <QVBoxLayout>
#include <QProgressDialog>
#include "ui/auxiliary/delegates/connectiondelegate.h"
#include <QStringBuilder>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

ConnectionResultWidget::ConnectionResultWidget(CachedAPI *iAPI, QWidget *iParent) : QWidget(iParent), mAPI(iAPI)
{
    // Initialisation
    init_ui();
    init_children();
}

void ConnectionResultWidget::load(const QList<ConnectionPointer>& iConnections)
{
    populateModel(iConnections);
}


//
// Public slots
//


//
// UI events
//

void ConnectionResultWidget::activated(QModelIndex iIndex)
{
    ConnectionPointer tConnection = iIndex.data(ConnectionRole).value<ConnectionPointer>();
    emit finished(tConnection);
}


//
// Initialisation
//


void ConnectionResultWidget::init_ui()
{
    // Window settings
    this->setWindowTitle(QString(tr("Connections")));

    // Main layout
    QVBoxLayout *mUILayout = new QVBoxLayout(this);
    mUILayout->setAlignment(Qt::AlignTop);

    // Populate the history list model
    mModel = new QStandardItemModel(0, 1);

    // Create the history listview
    QListView *tView = new QListView();
    tView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tView->setModel(mModel);
    tView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tView->setSelectionMode(QAbstractItemView::SingleSelection);
    tView->setItemDelegate(new ConnectionDelegate());
    connect(tView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(activated(QModelIndex)));
    mUILayout->addWidget(tView);
}

void ConnectionResultWidget::init_children()
{
}

//
// Auxiliary
//

void ConnectionResultWidget::populateModel(const QList<ConnectionPointer>& iConnections)
{
    mModel->clear();
    if (iConnections.size() > 0)
    {
        for (int i = 0; i < iConnections.size(); i++)
        {
            ConnectionPointer tConnection = iConnections.at(i);
            QStandardItem *tItem = new QStandardItem();
            tItem->setData(QVariant::fromValue(tConnection), ConnectionRole);
            mModel->appendRow(tItem);
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
