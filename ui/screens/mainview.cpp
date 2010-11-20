//
// Configuration
//

// Includes
#include "mainview.h"
#include "ui/auxiliary/delegates/connectionrequestdelegate.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFont>
#include <QStringBuilder>
#include <QtMaemo5/QMaemo5InformationBox>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

MainView::MainView(QWidget* iParent) : QScrollArea(iParent)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    if (iParent != 0)
    {
        setWindowFlags(windowFlags() | Qt::Window);
        setAttribute(Qt::WA_Maemo5StackedWindow);
    }

    mChildConnectionRequest = 0;

    this->hide();
    init_ui();
    init_children();
}

MainView::~MainView()
{
    qDebug() << "~ " << __PRETTY_FUNCTION__;

}

void MainView::showUI()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    this->show();
}


//
// Initialization
//

void MainView::init_ui()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

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

    // Top buttonsmScreenLiveboard
    QHBoxLayout *blayout = new QHBoxLayout;
    mUIButtonSearch = new QPushButton(tr("Plan a journey"));
    mUIButtonSearch->setIcon(QIcon(":ui/assets/icons/train.png"));
    QPushButton *mUIButtonLiveboard = new QPushButton(tr("View departures"));
    mUIButtonLiveboard->setIcon(QIcon(":ui/assets/icons/liveboard.png"));
    blayout->addWidget(mUIButtonSearch);
    blayout->addWidget(mUIButtonLiveboard);
    connect(mUIButtonSearch, SIGNAL(clicked()), this, SLOT(_showConnectionRequest()));
    connect(mUIButtonLiveboard, SIGNAL(clicked()), this, SIGNAL(launchLiveboard()));
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
    mViewDummy = new QLabel(tr("No history or favorites"));
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

void MainView::init_children()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    // Construct and connect the progress dialog (we can persistently connect
    // as the dialog'll only be used for API progress)
    mChildProgressDialog = new OptionalProgressDialog(this);
    //connect(mAPI, SIGNAL(miss()), mChildProgressDialog, SLOT(show()));
    //connect(mAPI, SIGNAL(action(QString)), mChildProgressDialog, SLOT(setLabelText(QString)));
}


//
// UI events
//

void MainView::_showConnectionRequest()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    mAction = CONNECTIONREQUEST;
    emit downloadStations();
}

void MainView::_showConnectionRequest(const QMap<QString, StationPointer>& iStations)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    if (mChildConnectionRequest == 0)
    {
        // Connection request widget
        mChildConnectionRequest = new ConnectionRequestWidget(iStations, this);
        mChildConnectionRequest->setWindowFlags(this->windowFlags() | Qt::Window);
        mChildConnectionRequest->setAttribute(Qt::WA_Maemo5StackedWindow);
        connect(mChildConnectionRequest, SIGNAL(finished(ConnectionRequestPointer)), this, SIGNAL(launchConnection(ConnectionRequestPointer)));
    }

    mChildConnectionRequest->clear();
    if (!tInitialRequest.isNull())
    {
        mChildConnectionRequest->load(tInitialRequest);
        tInitialRequest.clear();
    }
    mChildConnectionRequest->show();
}

void MainView::load_history(QModelIndex iIndex)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    tInitialRequest = iIndex.data(ConnectionRequestRole).value<ConnectionRequestPointer>();
    _showConnectionRequest();
}


//
// Controller actions
//

void MainView::setStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    switch (mAction)
    {
    case CONNECTIONREQUEST:
        _showConnectionRequest(*iStations);
        delete iStations;
        break;
    }
}

void MainView::showError(const QString &iError)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    QMaemo5InformationBox::information(this, tr("Error: ") % iError, QMaemo5InformationBox::DefaultTimeout);
}


//
// Auxiliary
//

void MainView::populateModel()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

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
        mViewDummy->setVisible(true);
        mView->setVisible(false);
    }
}
