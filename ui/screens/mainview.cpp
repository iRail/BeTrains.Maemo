//
// Configuration
//

// Includes
#include "mainview.h"
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

MainView::MainView(QWidget* iParent) : QScrollArea(iParent)
{
    init_ui();
    init_children();
}


//
// Initialization
//

void MainController::init_ui()
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
    mUIButtonSearch = new QPushButton(tr("Plan a journey"));
    mUIButtonSearch->setIcon(QIcon(":ui/assets/icons/train.png"));
    QPushButton *mUIButtonLiveboard = new QPushButton(tr("View departures"));
    mUIButtonLiveboard->setIcon(QIcon(":ui/assets/icons/liveboard.png"));
    blayout->addWidget(mUIButtonSearch);
    blayout->addWidget(mUIButtonLiveboard);
    connect(mUIButtonSearch, SIGNAL(clicked()), this, SLOT(show_connectionrequestwidget()));
    connect(mUIButtonLiveboard, SIGNAL(clicked()), this, SLOT(show_liveboardwidget()));
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

void MainController::init_children()
{
    // Construct and connect the progress dialog (we can persistently connect
    // as the dialog'll only be used for API progress)
    mChildProgressDialog = new OptionalProgressDialog(this);
    connect(mAPI, SIGNAL(miss()), mChildProgressDialog, SLOT(show()));
    connect(mAPI, SIGNAL(action(QString)), mChildProgressDialog, SLOT(setLabelText(QString)));
}
//
// UI events
//

void MainController::load_history(QModelIndex iIndex)
{
    // Bug in Qt? Non-selectable QStandardItem can be doubleClicked...
    if (mConnectionRequestHistory.size() == 0)
        return;

    tInitialRequest = iIndex.data(ConnectionRequestRole).value<ConnectionRequestPointer>();
    show_connectionrequestwidget();
}


//
// Auxiliary
//

void MainController::populateModel()
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
        mViewDummy->setVisible(true);
        mView->setVisible(false);
    }
}
