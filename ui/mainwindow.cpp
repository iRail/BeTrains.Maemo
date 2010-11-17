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
    button1->setIcon(QIcon(":ui/assets/icons/train.png"));
    QPushButton *button2 = new QPushButton(tr("Chat"));
    button2->setIcon(QIcon::fromTheme("general_chat_button"));
    blayout->addWidget(button1);
    blayout->addWidget(button2);
    connect(button1, SIGNAL(clicked()), this, SLOT(show_request()));
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
    // Connection request widget
    mChildConnectionRequest = new ConnectionRequestWidget(mAPI, this);
    mChildConnectionRequest->setWindowFlags(this->windowFlags() | Qt::Window);
    mChildConnectionRequest->setAttribute(Qt::WA_Maemo5StackedWindow);
    connect(mChildConnectionRequest, SIGNAL(finished(ConnectionRequestPointer)), this, SLOT(show_result(ConnectionRequestPointer)));

    // Result widget
    mChildConnectionResult = new ConnectionResultWidget(mAPI, mChildConnectionRequest);
    mChildConnectionResult->setWindowFlags(this->windowFlags() | Qt::Window);
    mChildConnectionResult->setAttribute(Qt::WA_Maemo5StackedWindow);
    connect(mChildConnectionResult, SIGNAL(finished(ConnectionPointer)), this, SLOT(show_detail(ConnectionPointer)));

    // Detail widget
    mChildConnectionDetail = new ConnectionDetailWidget(mAPI, mChildConnectionResult);
    mChildConnectionDetail->setWindowFlags(this->windowFlags() | Qt::Window);
    mChildConnectionDetail->setAttribute(Qt::WA_Maemo5StackedWindow);
}


//
// Public slots
//


void MainWidget::show_request()
{
    mChildConnectionRequest->show();
}

void MainWidget::show_result(ConnectionRequestPointer iConnectionRequest)
{
    // Fix the history model
    mConnectionRequestHistory.prepend(iConnectionRequest);
    populateModel();    

    // Show the results
    mChildConnectionResult->show();
    mChildConnectionResult->setRequest(iConnectionRequest);
}

void MainWidget::show_detail(ConnectionPointer iConnection)
{
    // Show the details
    mChildConnectionDetail->show();
    mChildConnectionDetail->setConnection(iConnection);
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
