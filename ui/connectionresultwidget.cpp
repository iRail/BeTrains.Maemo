//
// Configuration
//

// Includes
#include "connectionresultwidget.h"
#include <QVBoxLayout>
#include <QProgressDialog>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

ConnectionResultWidget::ConnectionResultWidget(CachedAPI *iAPI, ConnectionRequestPointer iConnectionRequest, QWidget *iParent) : QWidget(iParent), mAPI(iAPI), mConnectionRequest(iConnectionRequest)
{
    // Initialisation
    init_ui();
    init_children();

    // Progress dialog    
    mChildProgressDialog->setEnabled(true);
    mChildProgressDialog->setWindowTitle(tr("Fetching list of connections"));

    // Fetch the stations
    // requestStations fails here?
    mAPI->requestConnections(iConnectionRequest);
    connect(mAPI, SIGNAL(replyConnections(QList<ConnectionPointer>)), this, SLOT(show_connections(QList<ConnectionPointer>)));
}

ConnectionResultWidget::~ConnectionResultWidget()
{
    delete mChildProgressDialog;
}


//
// UI Events
//

void ConnectionResultWidget::show_connections(const QList<ConnectionPointer>& iConnections)
{
    mChildProgressDialog->setEnabled(false);

    mConnections = &iConnections;

    populateModel();
}


//
// Initialisation
//


void ConnectionResultWidget::init_ui()
{
    // Window settings
    this->setWindowTitle(QString(tr("Connections - %1 to %2")
                                 .arg(mConnectionRequest->origin())
                                 .arg(mConnectionRequest->destination()))
                         );

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
    //connect(tView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(load_details(QModelIndex)));
    mUILayout->addWidget(tView);
}

void ConnectionResultWidget::init_children()
{
    // Construct and connect the progress dialog (we can persistently connect
    // as the dialog'll only be used for API progress)
    mChildProgressDialog = new OptionalProgressDialog(this);
    connect(mAPI, SIGNAL(progress_start()), mChildProgressDialog, SLOT(show()));
    connect(mAPI, SIGNAL(action(QString)), mChildProgressDialog, SLOT(setLabelText(QString)));
}

//
// Auxiliary
//

void ConnectionResultWidget::populateModel()
{
    mModel->clear();
    if (mConnections->size() > 0)
    {
        for (int i = 0; i < mConnections->size(); i++)
        {
            ConnectionPointer tConnection = mConnections->at(i);
            QStandardItem *tItem;

            if (tConnection->transfers().size() == 0)
            {
                    tItem = new QStandardItem(tr("%1 (%2) to %3 (%4)")
                                              .arg(tConnection->transfer().departure.station)
                                              .arg(tConnection->transfer().departure.datetime.time().toString(Qt::LocaleDate))
                                              .arg(tConnection->transfer().arrival.station)
                                              .arg(tConnection->transfer().arrival.datetime.time().toString(Qt::LocaleDate))
                                              );
            }
            else
            {
                tItem = new QStandardItem(tr("%1 (%2) to %3 (%4), via %5 others")
                                          .arg(tConnection->transfer().departure.station)
                                          .arg(tConnection->transfer().departure.datetime.time().toString(Qt::LocaleDate))
                                          .arg(tConnection->transfer().arrival.station)
                                          .arg(tConnection->transfer().arrival.datetime.time().toString(Qt::LocaleDate))
                                          .arg(tConnection->transfers().size())
                                          );
            }

            tItem->setData(QVariant::fromValue(tConnection), ConnectionsRole);
            tItem->setEditable(false);
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
