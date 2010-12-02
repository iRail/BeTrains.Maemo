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

    // Scroll area
    QVBoxLayout* mUILayout = new QVBoxLayout(centralWidget());
    mUILayout->setMargin(0);
    QScrollArea* mUIScrollArea = new QScrollArea(centralWidget());
    mUILayout->addWidget(mUIScrollArea);

    // Scroll widget
    QWidget *tUIScrollWidget = new QWidget();
    mUIScrollArea->setWidget(tUIScrollWidget);
    mUIScrollArea->setWidgetResizable(true);

    // Main layout
    mUIScrollLayout = new QVBoxLayout(mUIScrollArea);
    tUIScrollWidget->setLayout(mUIScrollLayout);

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
    connect(mView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(do_lstHistory_clicked(QModelIndex)));
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

    // Create the history listview spacer (TODO: this is a hack)
    mViewSpacer = new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding);
    mUIScrollLayout->addSpacerItem(mViewSpacer);

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
        mView->setFixedHeight(70*mModel->rowCount());       // HACK
        mUIScrollLayout->removeItem(mViewSpacer);           // HACK (without fixedheight we could use sizepolicy)
        mUIScrollLayout->addSpacerItem(mViewSpacer);
    }
    else
    {
        mViewDummy->setVisible(true);
        mView->setVisible(false);
        mUIScrollLayout->removeItem(mViewSpacer);           // HACK (without fixedheight we could use sizepolicy)
    }
}
