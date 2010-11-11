//
// Configuration
//

// Includes
#include "mainwidget.h"
#include "connectionrequestwidget.h"
#include "auxiliary/delegates/connectionrequestdelegate.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFont>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

MainWidget::MainWidget(CachedAPI* iAPI, QWidget* parent) : QScrollArea(parent), mAPI(iAPI)
{
    init_ui();
    init_children();
}

//
// Initialization
//

void MainWidget::init_ui()
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
    QPushButton *button1 = new QPushButton(tr("New query"));
    QPushButton *button2 = new QPushButton(tr("Chat"));
    blayout->addWidget(button1);
    blayout->addWidget(button2);
    connect(button1, SIGNAL(clicked()), this, SLOT(show_connectionquerywidget()));
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

void MainWidget::init_children()
{
    // Query widget
    mChildConnectionRequest = new ConnectionRequestWidget(mAPI, this);
    mChildConnectionRequest->setWindowFlags(this->windowFlags() | Qt::Window);
    mChildConnectionRequest->setAttribute(Qt::WA_Maemo5StackedWindow);
    connect(mChildConnectionRequest, SIGNAL(search(ConnectionRequestPointer)), this, SLOT(add_history(ConnectionRequestPointer)));
}


//
// Public slots
//


void MainWidget::show_connectionquerywidget()
{
    mChildConnectionRequest->show();
}

void MainWidget::add_history(ConnectionRequestPointer iConnectionRequest)
{
    mConnectionRequestHistory.prepend(iConnectionRequest);
    populateModel();
}


//
// UI events
//

void MainWidget::load_history(QModelIndex iIndex)
{
    // Bug in Qt? Non-selectable QStandardItem can be doubleClicked...
    if (mConnectionRequestHistory.size() == 0)
        return;

    ConnectionRequestPointer tConnectionRequest = iIndex.data(ConnectionRequestRole).value<ConnectionRequestPointer>();
    mChildConnectionRequest->load(tConnectionRequest);
    mChildConnectionRequest->show();
}


//
// Auxiliary
//

void MainWidget::populateModel()
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
