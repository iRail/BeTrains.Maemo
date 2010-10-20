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
    QProgressDialog tProgressIndicator(this);
    tProgressIndicator.setWindowTitle(tr("Fetching list of connections"));
    tProgressIndicator.setModal(true);
    connect(mAPI, SIGNAL(action(QString)), &tProgressIndicator, SLOT(setLabelText(QString)));
    connect(mAPI, SIGNAL(progress(int)), &tProgressIndicator, SLOT(setValue(int)));
    tProgressIndicator.show();

    // Load the connections
    mConnections = mAPI->connections(iConnectionRequest);
    tProgressIndicator.hide();
    populateModel();
}



//
// Initialisation
//


void ConnectionResultWidget::init_ui()
{
    // Window settings
    this->setWindowTitle(QString(tr("Connections - %1 to %2")
                                 .arg(mConnectionRequest->getOrigin())
                                 .arg(mConnectionRequest->getDestination()))
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

}

//
// Auxiliary
//

void ConnectionResultWidget::populateModel()
{
    mModel->clear();
    if (mConnections.size() > 0)
    {
        for (int i = 0; i < mConnections.size(); i++)
        {
            ConnectionPointer tConnection = mConnections.at(i);
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
