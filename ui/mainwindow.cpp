//
// Configuration
//

// Includes
#include "mainwindow.h"
#include "auxiliary/delegates/connectionrequestdelegate.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFont>
#include <QStringBuilder>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

MainWindow::MainWindow(CachedAPI* iAPI, QWidget* parent) : QScrollArea(parent), mAPI(iAPI)
{
    init_ui();
    init_children();

    mChildConnectionRequest = 0;
    mChildConnectionResult = 0;
    mChildConnectionDetail = 0;
}

MainWindow::~MainWindow()
{
    delete mChildProgressDialog;
}

//
// Initialization
//

void MainWindow::init_ui()
{
    // Window settings
    this->setWindowTitle(QString("BeTrains"));
    this->setAttribute(Qt::WA_Maemo5StackedWindow);

    // Parent widget
    QWidget *tWidget = new QWidget();
    setWidget(tWidget);
    setWidgetResizable(true);

    // Main layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);
    tWidget->setLayout(layout);

    // Top buttons
    QHBoxLayout *blayout = new QHBoxLayout;
    mUIButtonSearch = new QPushButton(tr("New query"));
    mUIButtonSearch->setIcon(QIcon(":ui/assets/icons/train.png"));
    QPushButton *button2 = new QPushButton(tr("Chat"));
    button2->setIcon(QIcon::fromTheme("general_chat_button"));
    blayout->addWidget(mUIButtonSearch);
    blayout->addWidget(button2);
    connect(mUIButtonSearch, SIGNAL(clicked()), this, SLOT(show_requestwidget()));
    layout->addLayout(blayout);

    // Populate the history list model
    mModel = new QStandardItemModel(0, 1);

    // Create the history listview
    mView = new QListView();
    mView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mView->setModel(mModel);
    mView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mView->setSelectionMode(QAbstractItemView::SingleSelection);
    mView->setItemDelegate(new ConnectionRequestDelegate());
    mView->setResizeMode(QListView::Adjust);
    connect(mView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(load_history(QModelIndex)));
    layout->addWidget(mView);    
    // TODO: configure the QListView to be expanding within the QScrollArea

    // Create the history listview dummy
    mViewDummy = new QLabel(tr("No history"));
    mViewDummy->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mViewDummy->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    mViewDummy->setEnabled(false);
    QFont font;
    font.setPointSize(24);
    mViewDummy->setFont(font);
    layout->addWidget(mViewDummy);
    // TODO: load history from file
    populateModel();

}

void MainWindow::init_children()
{    
    // Construct and connect the progress dialog (we can persistently connect
    // as the dialog'll only be used for API progress)
    mChildProgressDialog = new OptionalProgressDialog(this);
    connect(mAPI, SIGNAL(miss()), mChildProgressDialog, SLOT(show()));
    connect(mAPI, SIGNAL(action(QString)), mChildProgressDialog, SLOT(setLabelText(QString)));
}


//
// Public slots
//


void MainWindow::load_requestwidget(QMap<QString, StationPointer>* iStations)
{
    mChildProgressDialog->setEnabled(false);
    disconnect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(load_requestwidget(QMap<QString, StationPointer>*)));

    if (iStations != 0)
    {
        // Connection request widget
        mChildConnectionRequest = new ConnectionRequestWidget(*iStations, this);
        mChildConnectionRequest->setWindowFlags(this->windowFlags() | Qt::Window);
        mChildConnectionRequest->setAttribute(Qt::WA_Maemo5StackedWindow);
        connect(mChildConnectionRequest, SIGNAL(finished(ConnectionRequestPointer)), this, SLOT(process_requestwidget(ConnectionRequestPointer)));

        // Finish up
        delete iStations;
    }
    else
    {
        if (mAPI->hasError())
            QMaemo5InformationBox::information(this, tr("Error: ") % mAPI->errorString(), QMaemo5InformationBox::DefaultTimeout);
        else
            QMaemo5InformationBox::information(this, tr("Unknown error"), QMaemo5InformationBox::DefaultTimeout);
    }

    // Actually show the widget
    show_requestwidget();
}

void MainWindow::show_requestwidget()
{
    // Check if the widget is loaded already
    if (mChildConnectionRequest == 0)
    {
        mChildProgressDialog->setEnabled(true);
        mChildProgressDialog->setWindowTitle(tr("Fetching list of stations"));
        connect(mAPI, SIGNAL(replyStations(QMap<QString, StationPointer>*)), this, SLOT(load_requestwidget(QMap<QString, StationPointer>*)));
        mAPI->requestStations();
        return;
    }

    mChildConnectionRequest->clear();
    if (!tInitialRequest.isNull())
    {
        mChildConnectionRequest->load(tInitialRequest);
        tInitialRequest.clear();
    }
    mChildConnectionRequest->show();
}

void MainWindow::process_requestwidget(ConnectionRequestPointer iConnectionRequest)
{
    // Fix the history model
    mConnectionRequestHistory.prepend(iConnectionRequest);
    populateModel();

    // Request the data
    mChildProgressDialog->setEnabled(true);
    mChildProgressDialog->setWindowTitle(tr("Fetching list of connections"));
    connect(mAPI, SIGNAL(replyConnections(QList<ConnectionPointer>*)), this, SLOT(show_resultwidget(QList<ConnectionPointer>*)));
    mAPI->requestConnections(iConnectionRequest);
}

void MainWindow::load_resultwidget()
{
    // Result widget
    Q_ASSERT(mChildConnectionRequest != 0);
    mChildConnectionResult = new ConnectionResultWidget(mChildConnectionRequest->stations(), mChildConnectionRequest);
    mChildConnectionResult->setWindowFlags(this->windowFlags() | Qt::Window);
    mChildConnectionResult->setAttribute(Qt::WA_Maemo5StackedWindow);
    connect(mChildConnectionResult, SIGNAL(finished(ConnectionPointer)), this, SLOT(show_detailwidget(ConnectionPointer)));
}

void MainWindow::show_resultwidget(QList<ConnectionPointer>* iConnections)
{
    // Disconnect the signal (as the only way this widget can be shown
    // is through the API)
    disconnect(mAPI, SIGNAL(replyConnections(QList<ConnectionPointer>*)), this, SLOT(show_resultwidget(QList<ConnectionPointer>*)));

    // Check if the widget is loaded already
    if (mChildConnectionResult == 0)
        load_resultwidget();

    mChildProgressDialog->setEnabled(false);

    if (iConnections != 0)
    {
        // Show the results
        mChildConnectionResult->show();
        mChildConnectionResult->load(*iConnections);
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

void MainWindow::load_detailwidget()
{
    // Detail widget
    Q_ASSERT(mChildConnectionResult != 0);
    mChildConnectionDetail = new ConnectionDetailWidget(mChildConnectionRequest->stations(), mChildConnectionResult);
    mChildConnectionDetail->setWindowFlags(this->windowFlags() | Qt::Window);
    mChildConnectionDetail->setAttribute(Qt::WA_Maemo5StackedWindow);
}

void MainWindow::show_detailwidget(ConnectionPointer iConnection)
{
    // Check if the widget is loaded already
    if (mChildConnectionDetail == 0)
        load_detailwidget();

    // Show the details
    mChildConnectionDetail->show();
    mChildConnectionDetail->setConnection(iConnection);
}


//
// UI events
//

void MainWindow::load_history(QModelIndex iIndex)
{
    // Bug in Qt? Non-selectable QStandardItem can be doubleClicked...
    if (mConnectionRequestHistory.size() == 0)
        return;

    tInitialRequest = iIndex.data(ConnectionRequestRole).value<ConnectionRequestPointer>();
    show_requestwidget();
}


//
// Auxiliary
//

void MainWindow::populateModel()
{
    mModel->clear();
    if (mConnectionRequestHistory.size() > 0)
    {
        for (int i = 0; i < mConnectionRequestHistory.size(); i++)
        {
            ConnectionRequestPointer tConnectionRequest = mConnectionRequestHistory.at(i);
            QStandardItem *tItem = new QStandardItem;

            tItem->setData(QVariant::fromValue(tConnectionRequest), ConnectionRequestRole);
            mModel->appendRow(tItem);
        }

        mViewDummy->setVisible(false);
        mView->setVisible(true);
        mView->setModel(mModel);
        mView->setFixedHeight(70*mModel->rowCount());   // HACK
    }
    else
    {
        QStandardItem *tDummy = new QStandardItem(tr("History is empty."));
        tDummy->setEditable(false);
        tDummy->setSelectable(false);
        mModel->appendRow(tDummy);

        mViewDummy->setVisible(true);
        mView->setVisible(false);
    }
}
