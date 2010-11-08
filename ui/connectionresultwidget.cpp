//
// Configuration
//

// Includes
#include "connectionresultwidget.h"
#include <QVBoxLayout>
#include <QProgressDialog>
#include "auxiliary/delegates/connectiondelegate.h"
#include <QtMaemo5/QMaemo5InformationBox>
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

ConnectionResultWidget::~ConnectionResultWidget()
{
    delete mChildProgressDialog;
}


//
// Public slots
//

void ConnectionResultWidget::setRequest(ConnectionRequestPointer iConnectionRequest)
{
    // Progress dialog
    mChildProgressDialog->setEnabled(true);
    mChildProgressDialog->setWindowTitle(tr("Fetching list of connections"));

    // Alter the UI
    update_ui(iConnectionRequest);

    // Fetch the stations
    // requestStations fails here?
    connect(mAPI, SIGNAL(replyConnections(QList<ConnectionPointer>*)), this, SLOT(show_connections(QList<ConnectionPointer>*)));
    mAPI->requestConnections(iConnectionRequest);
}


//
// UI events
//

void ConnectionResultWidget::show_connections(QList<ConnectionPointer>* iConnections)
{
    mChildProgressDialog->setEnabled(false);
    disconnect(mAPI, SIGNAL(replyConnections(QList<ConnectionPointer>*)), this, SLOT(show_connections(QList<ConnectionPointer>*)));

    if (iConnections != 0)
    {
        populateModel(iConnections);
        delete iConnections;
    }
    else
    {
        if (mAPI->hasError())
            QMaemo5InformationBox::information(this, tr("Error: ") % mAPI->errorString(), QMaemo5InformationBox::DefaultTimeout);
        else
            QMaemo5InformationBox::information(this, tr("Unknown error"), QMaemo5InformationBox::DefaultTimeout);
    }
}

void ConnectionResultWidget::load_details(QModelIndex iIndex)
{
    ConnectionPointer tConnection = iIndex.data(ConnectionRole).value<ConnectionPointer>();
    mChildConnectionDetail->show();
    mChildConnectionDetail->setConnection(tConnection);
}


//
// Initialisation
//


void ConnectionResultWidget::init_ui()
{
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
    connect(tView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(load_details(QModelIndex)));
    mUILayout->addWidget(tView);
}

void ConnectionResultWidget::update_ui(ConnectionRequestPointer iConnectionRequest)
{
    // Window settings
    this->setWindowTitle(QString(tr("Connections - %1 to %2")
                                 .arg(iConnectionRequest->origin())
                                 .arg(iConnectionRequest->destination()))
                         );

    mModel->clear();

}

void ConnectionResultWidget::init_children()
{
    // Construct and connect the progress dialog (we can persistently connect
    // as the dialog'll only be used for API progress)
    mChildProgressDialog = new OptionalProgressDialog(this);
    connect(mAPI, SIGNAL(miss()), mChildProgressDialog, SLOT(show()));
    connect(mAPI, SIGNAL(action(QString)), mChildProgressDialog, SLOT(setLabelText(QString)));

    // Detail widget
    mChildConnectionDetail = new ConnectionDetailWidget(mAPI, this);
    mChildConnectionDetail->setWindowFlags(this->windowFlags() | Qt::Window);
    mChildConnectionDetail->setAttribute(Qt::WA_Maemo5StackedWindow);
}

//
// Auxiliary
//

void ConnectionResultWidget::populateModel(QList<ConnectionPointer>* iConnections)
{
    mModel->clear();
    if (iConnections->size() > 0)
    {
        for (int i = 0; i < iConnections->size(); i++)
        {
            ConnectionPointer tConnection = iConnections->at(i);
            QStandardItem *tItem = new QStandardItem();
            tItem->setData(QVariant::fromValue(tConnection), ConnectionRole);
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
