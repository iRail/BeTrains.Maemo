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

MainWidget::MainWidget(CachedAPI* iAPI, QWidget* parent) : QWidget(parent), mAPI(iAPI)
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

    // Main layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);

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
    connect(mView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(load_history(QModelIndex)));
    layout->addWidget(mView);

    // Create the history listview dummy
    mViewDummy = new QLabel(tr("No history"));
    mViewDummy->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mViewDummy->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
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
    // Query widgets
    mConnectionRequestWidget = new ConnectionRequestWidget(mAPI, this);
    mConnectionRequestWidget->setWindowFlags(this->windowFlags() | Qt::Window);
    mConnectionRequestWidget->setAttribute(Qt::WA_Maemo5StackedWindow);
    connect(mConnectionRequestWidget, SIGNAL(search(ConnectionRequestPointer)), this, SLOT(add_history(ConnectionRequestPointer)));
}


//
// Public slots
//


void MainWidget::show_connectionquerywidget()
{
    mConnectionRequestWidget->show();
}

void MainWidget::add_history(ConnectionRequestPointer iConnectionRequest)
{
    mConnectionRequestHistory.prepend(iConnectionRequest);
    populateModel();
}

void MainWidget::load_history(QModelIndex iIndex)
{
    // Bug in Qt? Non-selectable QStandardItem can be doubleClicked...
    if (mConnectionRequestHistory.size() == 0)
        return;

    ConnectionRequestPointer tConnectionRequest = iIndex.data(ConnectionRequestRole).value<ConnectionRequestPointer>();
    mConnectionRequestWidget->load(tConnectionRequest);
    mConnectionRequestWidget->show();
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
            tItem->setEditable(false);
            mModel->appendRow(tItem);
        }

        mViewDummy->setVisible(false);
        mView->setVisible(true);
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
