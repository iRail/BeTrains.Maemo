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
}

//
// Initialization
//

void MainView::init_ui()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Window settings
    setWindowTitle(QString("BeTrains"));

    // History listview header
    QHBoxLayout *tButtons = new QHBoxLayout;
    QPushButton* tUIButtonSearch = new QPushButton(tr("Plan a journey"));
    tUIButtonSearch->setIcon(QIcon(":ui/assets/icons/train.png"));
    QPushButton *mUIButtonLiveboard = new QPushButton(tr("View departures"));
    mUIButtonLiveboard->setIcon(QIcon(":ui/assets/icons/liveboard.png"));
    tButtons->addWidget(tUIButtonSearch);
    tButtons->addWidget(mUIButtonLiveboard);
    connect(tUIButtonSearch, SIGNAL(clicked()), this, SIGNAL(launchRequest()));
    connect(mUIButtonLiveboard, SIGNAL(clicked()), this, SIGNAL(launchLiveboard()));
    mViewHeader = new QWidget();
    mViewHeader->setLayout(tButtons);

    // Populate the history list model
    mModel = new QStandardItemModel(0, 1);

    // Create the history listview
    mView = new QListView();
    setCentralWidget(mView);
    mView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mView->setModel(mModel);
    mView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mView->setSelectionMode(QAbstractItemView::SingleSelection);
    mView->setItemDelegate(new ConnectionRequestDelegate());
    mView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    connect(mView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(do_lstHistory_clicked(QModelIndex)));

    // Create the history listview dummy
    mViewDummy = new QLabel(tr("No history or favorites"));
    mViewDummy->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mViewDummy->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    mViewDummy->setEnabled(false);
    QFont font;
    font.setPointSize(24);
    mViewDummy->setFont(font);

    // TODO: load history from file
    populateModel();
}


//
// UI events
//

void MainView::do_lstHistory_clicked(QModelIndex iIndex)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    ConnectionRequestPointer tInitialRequest = iIndex.data(ConnectionRequestRole).value<ConnectionRequestPointer>();
    emit setInitialRequest(tInitialRequest);
}


//
// Controller actions
//

void MainView::setStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    delete iStations;
    qWarning() << "! " << "Handler not currently used";
}


//
// Auxiliary
//

void MainView::populateModel()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mModel->clear();

    // Button header
    mModel->appendRow(new QStandardItem());
    mView->setIndexWidget(mModel->index(0, 0), mViewHeader);
    mModel->item(0, 0)->setSelectable(false);

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
    }
    else
    {
        mModel->appendRow(new QStandardItem());
        mView->setIndexWidget(mModel->index(1, 0), mViewDummy);
        mModel->item(1, 0)->setSelectable(false);
        mViewDummy->setMinimumHeight(mView->height() - mViewHeader->height() - 60); // HACK
    }
}
