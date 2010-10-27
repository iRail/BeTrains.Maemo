//
// Configuration
//

// Includes
#include "mainwidget.h"
#include "connectionrequestwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

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
    this->setWindowTitle(QString("BETrains"));
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
    // TODO: load history from file
    populateModel();

    // Create the history listview
    QListView *tView = new QListView();
    tView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tView->setModel(mModel);
    tView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tView->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(tView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(load_history(QModelIndex)));
    layout->addWidget(tView);
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

    ConnectionRequestPointer tConnectionRequest = iIndex.data(HistoryRole).value<ConnectionRequestPointer>();
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
            QStandardItem *tItem;
            if (tConnectionRequest->timed())
            {
                const ConnectionRequest::Time *tTime = tConnectionRequest->time();
                tItem = new QStandardItem(tr("%1 to %2 (%3 at %4, %5)")
                                          .arg(tConnectionRequest->origin())
                                          .arg(tConnectionRequest->destination())
                                          .arg(tTime->type == ConnectionRequest::Departure ? tr("depart") : tr("arrival"))
                                          .arg(tTime->datetime.date().toString(Qt::LocaleDate))
                                          .arg(tTime->datetime.time().toString(Qt::LocaleDate))
                                          );
            }
            else
            {
                tItem = new QStandardItem(tr("%1 to %2")
                                          .arg(tConnectionRequest->origin())
                                          .arg(tConnectionRequest->destination())
                                          );
            }

            tItem->setData(QVariant::fromValue(tConnectionRequest), HistoryRole);
            tItem->setEditable(false);
            mModel->appendRow(tItem);
        }
    }
    else
    {
        QStandardItem *tDummy = new QStandardItem(tr("History is empty."));
        tDummy->setEditable(false);
        tDummy->setSelectable(false);
        mModel->appendRow(tDummy);
    }
}
