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
    connect(mAPI, SIGNAL(replyConnections(QList<ConnectionPointer>*)), this, SLOT(show_connections(QList<ConnectionPointer>*)));
}

ConnectionResultWidget::~ConnectionResultWidget()
{
    delete mChildProgressDialog;
}


//
// UI Events
//

void ConnectionResultWidget::show_connections(QList<ConnectionPointer>* iConnections)
{
    mChildProgressDialog->setEnabled(false);

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
    tView->setItemDelegate(new ConnectionDelegate());
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
