//
// Configuration
//

// Includes
#include "mainview.h"
#include "ui/auxiliary/delegates/requestdelegate.h"
#include "ui/dialogs/about.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFont>
#include <QMenu>
#include <QMenuBar>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

MainView::MainView(QWidget* iParent) : GenericView(iParent)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Initialisation
    hide();
    init_ui();
    init_menu();
}

void MainView::load()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    emit downloadStations();
}

void MainView::load(const QList<QVariant>& iHistory)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    populateModel(iHistory);
}

void MainView::load(const QMap<QString, StationPointer>& iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;
    stopLoader();

    mStations = iStations;
    mView->setItemDelegate(new RequestDelegate(mStations));
    // TODO: load the history
    populateModel(QList<QVariant>());
}

//
// Initialization
//

void MainView::init_ui()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Window settings
    setWindowTitle(QString("BeTrains"));

    // Main layout
    QVBoxLayout *tUILayout = new QVBoxLayout();
    tUILayout->setAlignment(Qt::AlignTop);
    centralWidget()->setLayout(tUILayout);


    // VIEW //

    // Populate the history list model
    mModel = new QStandardItemModel(0, 1);

    // Create the history listview
    mView = new QListView();
    mView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mView->setModel(mModel);
    mView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mView->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(mView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(do_lstHistory_clicked(QModelIndex)));
    tUILayout->addWidget(mView);

    // Create the history listview dummy
    mViewDummy = new QLabel(tr("No history or favorites"));
    mViewDummy->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mViewDummy->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
    mViewDummy->setEnabled(false);
    QFont font;
    font.setPointSize(24);
    mViewDummy->setFont(font);
    tUILayout->addWidget(mViewDummy);


    // VIEW HEADER //

    mViewHeader = new QWidget();
    QHBoxLayout *tViewHeaderLayout = new QHBoxLayout;
    mViewHeader->setLayout(tViewHeaderLayout);

    // Journey button
    QPushButton* tUIButtonSearch = new QPushButton(tr("Plan a journey"));
    tUIButtonSearch->setIcon(QIcon(":ui/assets/icons/train.png"));
    tViewHeaderLayout->addWidget(tUIButtonSearch);
    connect(tUIButtonSearch, SIGNAL(clicked()), this, SIGNAL(launchRequest()));

    // Liveboard button
    QPushButton *mUIButtonLiveboard = new QPushButton(tr("View departures"));
    mUIButtonLiveboard->setIcon(QIcon(":ui/assets/icons/liveboard.png"));
    tViewHeaderLayout->addWidget(mUIButtonLiveboard);
    connect(mUIButtonLiveboard, SIGNAL(clicked()), this, SIGNAL(launchLiveboard()));

    // Add to the view
    mModel->appendRow(new QStandardItem());
    mView->setIndexWidget(mModel->index(0, 0), mViewHeader);
    mModel->item(0, 0)->setSelectable(false);


    // OTHER //

    // Populate the model
    populateModel(QList<QVariant>());
}

void MainView::init_menu()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    QMenu *tMenuTools = menuBar()->addMenu(tr("&Tools"));
    QAction *tActionPreferences = tMenuTools->addAction(tr("&Preferences"));

    QMenu *tMenuHelp = menuBar()->addMenu(tr("&Help"));
    QAction *tActionAbout = tMenuHelp->addAction(tr("&About"));
    connect(tActionAbout, SIGNAL(triggered()), this, SLOT(do_actAbout_triggered()));
}


//
// UI events
//

void MainView::do_lstHistory_clicked(QModelIndex iIndex)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    if (iIndex.data(ConnectionRequestRole).canConvert<ConnectionRequestPointer>())
    {
        ConnectionRequestPointer tConnectionRequest = iIndex.data(ConnectionRequestRole).value<ConnectionRequestPointer>();
        emit launchRequest(tConnectionRequest);
    }
    else if (iIndex.data(LiveboardRequestRole).canConvert<LiveboardRequestPointer>())
    {
        LiveboardRequestPointer tLiveboardRequest = iIndex.data(LiveboardRequestRole).value<LiveboardRequestPointer>();
        emit launchLiveboard(tLiveboardRequest);
    }
}

void MainView::do_actAbout_triggered()
{
    About tAbout(this);
    tAbout.exec();
}


//
// Controller actions
//

void MainView::setStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    load(*iStations);
    delete iStations;
}


//
// Auxiliary
//

void MainView::populateModel(const QList<QVariant>& iHistory)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Remove all rows but the header
    if (mModel->rowCount() > 1)
    {
        mModel->removeRows(1, mModel->rowCount()-1);
    }

    if (iHistory.size() == 0)
    {
        mViewDummy->setVisible(true);
        mView->setFixedHeight(mViewHeader->height());   // HACK
    }
    else
    {
        // Add the contents
        for (int i = 0; i < iHistory.size(); i++)
        {
            QStandardItem *tItem = new QStandardItem();
            tItem->setIcon(QIcon(":ui/assets/favourite_unchecked.png"));

            QVariant tRequest = iHistory.at(i);
            if (tRequest.canConvert<LiveboardRequestPointer>())
                tItem->setData(tRequest, LiveboardRequestRole);
            else if (tRequest.canConvert<ConnectionRequestPointer>())
                tItem->setData(tRequest, ConnectionRequestRole);
            else
                continue;

            mModel->appendRow(tItem);
        }

        mViewDummy->setVisible(false);
        mView->setFixedHeight(centralWidget()->height());   // HACK
    }
}
