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
    this->hide();
    init_ui();
    init_children();
}

MainView::~MainView()
{
    qDebug() << "~ " << Q_FUNC_INFO;

}


//
// Initialization
//

void MainView::init_ui()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Window settings
    setWindowTitle(QString("BeTrains"));

    // Scroll area
    QVBoxLayout* mUILayout = new QVBoxLayout(this);
    mUILayout->setMargin(0);
    QScrollArea* mUIScrollArea = new QScrollArea(this);
    mUILayout->addWidget(mUIScrollArea);

    // Parent widget
    QWidget *tWidget = new QWidget();
    mUIScrollArea->setWidget(tWidget);
    mUIScrollArea->setWidgetResizable(true);

    // Main layout
    QVBoxLayout *mUIScrollLayout = new QVBoxLayout(mUIScrollArea);
    tWidget->setLayout(mUIScrollLayout);

    // Top buttons
    QHBoxLayout *mUIButtonLayout = new QHBoxLayout;
    mUIButtonSearch = new QPushButton(tr("Plan a journey"));
    mUIButtonSearch->setIcon(QIcon(":ui/assets/icons/train.png"));
    QPushButton *mUIButtonLiveboard = new QPushButton(tr("View departures"));
    mUIButtonLiveboard->setIcon(QIcon(":ui/assets/icons/liveboard.png"));
    mUIButtonLayout->addWidget(mUIButtonSearch);
    mUIButtonLayout->addWidget(mUIButtonLiveboard);
    connect(mUIButtonSearch, SIGNAL(clicked()), this, SIGNAL(launchRequest()));
    connect(mUIButtonLiveboard, SIGNAL(clicked()), this, SIGNAL(launchLiveboard()));
    mUIScrollLayout->addLayout(mUIButtonLayout);

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
    mUIScrollLayout->addWidget(mView);
    // TODO: configure the QListView to be expanding within the QScrollArea

    // Create the history listview dummy
    mViewDummy = new QLabel(tr("No history or favorites"));
    mViewDummy->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mViewDummy->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    mViewDummy->setEnabled(false);
    QFont font;
    font.setPointSize(24);
    mViewDummy->setFont(font);
    mUIScrollLayout->addWidget(mViewDummy);
    // TODO: load history from file
    populateModel();

}

void MainView::init_children()
{
    qDebug() << "+ " << Q_FUNC_INFO;
}


//
// UI events
//

void MainView::load_history(QModelIndex iIndex)
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
