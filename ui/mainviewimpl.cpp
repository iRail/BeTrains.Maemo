//
// Configuration
//

// Includes
#include "mainviewimpl.h"
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

MainViewImpl::MainViewImpl() : MainView()
{
    qDebug() << "+ " << Q_FUNC_INFO;
    proxy = new GenericViewImpl(this);

    // Initialisation
    init_ui();
    init_menu();
}


//
// Initialization
//

void MainViewImpl::init_ui()
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
    mView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(do_lstHistory_contextMenu(QPoint)));
    connect(mView, SIGNAL(activated(QModelIndex)), this, SLOT(do_lstHistory_activated(QModelIndex)));
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

    mViewImplHeader = new QWidget();
    QHBoxLayout *tViewImplHeaderLayout = new QHBoxLayout;
    mViewImplHeader->setLayout(tViewImplHeaderLayout);

    // Journey button
    QPushButton* tUIButtonSearch = new QPushButton(tr("Plan a journey"));
    tUIButtonSearch->setIcon(QIcon(":ui/assets/icons/train.png"));
    tViewImplHeaderLayout->addWidget(tUIButtonSearch);
    connect(tUIButtonSearch, SIGNAL(clicked()), this, SIGNAL(launchRequest()));

    // Liveboard button
    QPushButton *mUIButtonLiveboard = new QPushButton(tr("View departures"));
    mUIButtonLiveboard->setIcon(QIcon(":ui/assets/icons/liveboard.png"));
    tViewImplHeaderLayout->addWidget(mUIButtonLiveboard);
    connect(mUIButtonLiveboard, SIGNAL(clicked()), this, SIGNAL(launchLiveboard()));

    // Add to the view
    mModel->appendRow(new QStandardItem());
    mView->setIndexWidget(mModel->index(0, 0), mViewImplHeader);
    mModel->item(0, 0)->setSelectable(false);


    // OTHER //

    // Populate the model
    populateModel(QList<QVariant>(), QList<QVariant>());
}

void MainViewImpl::init_menu()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    QMenu *tMenuTools = menuBar()->addMenu(tr("&Tools"));
    QAction *tActionClearHistory = tMenuTools->addAction(tr("&Clear history"));
    connect(tActionClearHistory, SIGNAL(triggered()), this, SLOT(do_actClearHistory_triggered()));

    QMenu *tMenuHelp = menuBar()->addMenu(tr("&Help"));
    QAction *tActionAbout = tMenuHelp->addAction(tr("&About"));
    connect(tActionAbout, SIGNAL(triggered()), this, SLOT(do_actAbout_triggered()));
}


//
// UI events
//

void MainViewImpl::do_lstHistory_activated(QModelIndex iIndex)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    if (iIndex.data(ConnectionRequestRole).type() != QVariant::Invalid)
    {
        ConnectionRequestPointer tConnectionRequest = iIndex.data(ConnectionRequestRole).value<ConnectionRequestPointer>();
        emit launchConnection(tConnectionRequest);
    }
    else if (iIndex.data(LiveboardRequestRole).type() != QVariant::Invalid)
    {
        LiveboardRequestPointer tLiveboardRequest = iIndex.data(LiveboardRequestRole).value<LiveboardRequestPointer>();
        emit launchLiveboard(tLiveboardRequest);
    }
}

void MainViewImpl::do_actAbout_triggered()
{
    About tAbout(this);
    tAbout.exec();
}

void MainViewImpl::do_lstHistory_contextMenu(const QPoint& iPosition)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Get the data
    QModelIndex tIndex = mView->indexAt(iPosition);
    if (! tIndex.data(RequestFavouriteRole).canConvert<bool>())
        return;
    bool tFavourite = tIndex.data(RequestFavouriteRole).value<bool>();

    // Create the objects
    QMenu *tContextMenu = new QMenu;
    QAction *tAction = tContextMenu->addAction("");

    // Set the data (a QStandardItem can contain multiple QVariants,
    // hence the different cases)
    if (tIndex.data(LiveboardRequestRole).type() != QVariant::Invalid)
        tAction->setData(tIndex.data(LiveboardRequestRole));
    else if (tIndex.data(ConnectionRequestRole) != QVariant::Invalid)
        tAction->setData(tIndex.data(ConnectionRequestRole));
    else
        return;

    // Populate the menu
    if (tFavourite)
    {
        tAction->setText(tr("Remove from favourites"));
        connect(tAction, SIGNAL(triggered()), this, SLOT(do_actRemoveFavourite()));
    }
    else
    {
        tAction->setText(tr("Add to favourites"));
        connect(tAction, SIGNAL(triggered()), this, SLOT(do_actAddFavourite()));
    }
    tContextMenu->exec(mView->mapToGlobal(iPosition));
}

void MainViewImpl::do_actRemoveFavourite()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    QAction *tAction = qobject_cast<QAction *>(sender());
    if (tAction)
        emit removeFavourite(tAction->data());
}

void MainViewImpl::do_actAddFavourite()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    QAction *tAction = qobject_cast<QAction *>(sender());
    if (tAction)
        emit addFavourite(tAction->data());
}

void MainViewImpl::do_actClearHistory_triggered()
{
    emit clearHistory();
}


//
// Controller actions
//

void MainViewImpl::reset()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    populateModel(QList<QVariant>(), QList<QVariant>());
}

void MainViewImpl::load()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    emit downloadStations();
}

void MainViewImpl::load(const QList<QVariant>& iHistory, const QList<QVariant>& iFavourites)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    populateModel(iHistory, iFavourites);
}

void MainViewImpl::load(const QMap<QString, StationPointer>& iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Set the stations
    mStations = iStations;
    mView->setItemDelegate(new RequestDelegate(mStations));

    // Request the history and favourites
    emit getHistoryFavourites();
}

void MainViewImpl::setStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;
    stopLoader();

    load(*iStations);
    delete iStations;
}


//
// Auxiliary
//

void MainViewImpl::populateModel(const QList<QVariant>& iHistory, const QList<QVariant>& iFavourites)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Remove all rows but the header
    if (mModel->rowCount() > 1)
    {
        mModel->removeRows(1, mModel->rowCount()-1);
    }

    if (iHistory.size() + iFavourites.size() == 0)
    {
        mViewDummy->setVisible(true);
        mView->setFixedHeight(mViewImplHeader->height());   // HACK
    }
    else
    {        
        // Add the favourites
        for (int i = 0; i < iFavourites.size(); i++)
        {
            QStandardItem *tItem = new QStandardItem();
            tItem->setIcon(QIcon(":/ui/assets/favourite.svg"));
            tItem->setData(QVariant::fromValue(true), RequestFavouriteRole);

            QVariant tRequest = iFavourites.at(i);
            if (tRequest.canConvert<LiveboardRequestPointer>())
                tItem->setData(tRequest, LiveboardRequestRole);
            else if (tRequest.canConvert<ConnectionRequestPointer>())
                tItem->setData(tRequest, ConnectionRequestRole);
            else
                continue;

            mModel->appendRow(tItem);
        }

        // Add the history
        for (int i = 0; i < iHistory.size(); i++)
        {
            QStandardItem *tItem = new QStandardItem();
            tItem->setData(QVariant::fromValue(false), RequestFavouriteRole);

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
        mView->setFixedHeight(centralWidget()->maximumHeight());   // HACK

        /*
         What we try to accomplish: 2 buttons always visible, with some items
         under that, or when there are no items available, a message indicating
         that. The buttons should be scrollable together with the items (if any).

         Solution 1: 2 buttons in a scroll area, together with a stacked layout
         displaying the listview or a label.
         Issue: the listview doesn't expand within the scroll area, and using
         a fixed height to solve this messes up the layout management of items
         within the listview.

         Solution 2: buttons in the listview, and depending on the availability
         of more items, a label indicating there being none.
         Issue: the listview doesn't expand properly with the label as an item,
         as size policies only apply to layouts and not to item views. This could
         be fixed with using a fixed size for the label, but that wasn't always
         as easy to compute.

         Solution 3: buttons and items in the listview, and a label outside it,
         only visible when there are no items.
         Issue: the listview doesn't minimize properly (always uses 50% of the
         window space when one extra widget -- the label -- is present), even when
         internally there is unused space left (due to the lack of items).

         Eventually I've hacked the last solution, by making the listview force to
         take a size. Although this needs me to manually compute the layout for the
         widget, it doesn't mess up item management within the listview (which is hard),
         nor does it reqiure managing nested scroll area's (which sucks).
         */
    }
}
