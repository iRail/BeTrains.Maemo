//
// Configuration
//

// Includes
#include "liveboardviewimpl.h"
#include "ui/dialogs/stationchooser.h"
#include <QScrollArea>
#include "ui/auxiliary/delegates/liveboarddeparturedelegate.h"
#include "api/auxiliary.h"

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

LiveboardViewImpl::LiveboardViewImpl() : LiveboardView()
{
    qDebug() << "+ " << Q_FUNC_INFO;
    proxy = new GenericViewImpl(this);

    // Initialisation
    hide();
    init_ui();
}


//
// UI events
//

void LiveboardViewImpl::do_btnStations_clicked()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mDepartures.clear();

    StationChooser tChooser(mStations, centralWidget());
    int tReturn = tChooser.exec();
    if (tReturn == QDialog::Accepted)
    {
        tLiveboardRequest = LiveboardRequestPointer(new LiveboardRequest(tChooser.getSelection()));
        mUIStationEdit->setText(stationName(mStations, tLiveboardRequest->station()));

        emit downloadLiveboard(tLiveboardRequest);
    }
}

void LiveboardViewImpl::do_lstDepartures_activated(QModelIndex iIndex)
{
    qDebug() << "+ " << Q_FUNC_INFO;
    if (iIndex.row() == 0)    //  The header (containing a button & search field) cannot be clicked
        return;             //  TODO: do this nicely, using the hypothetical item(0, 0)->setActivatable(false)

    emit launchVehicle(tLiveboardRequest->station(), iIndex.data(LiveboardDepartureRole).value<Liveboard::Departure>());
}

void LiveboardViewImpl::do_btnMore_clicked()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Lookup last departure
    Q_ASSERT(mDepartures.size());
    Liveboard::Departure tDeparture = mDepartures.last();

    // Make a new request
    tLiveboardRequest->setTime(tDeparture.datetime);
    emit downloadLiveboard(tLiveboardRequest);

}


//
// Controller actions
//

void LiveboardViewImpl::reset()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mUIStationEdit->clear();
    mDepartures.clear();
    populateModel();
}

void LiveboardViewImpl::load()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    emit downloadStations();
}

void LiveboardViewImpl::load(LiveboardRequestPointer iLiveboardRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    tLiveboardRequest = iLiveboardRequest;
    emit downloadLiveboard(iLiveboardRequest);
}

void LiveboardViewImpl::setStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;
    stopLoader();

    mStations = *iStations;
    delete iStations;

    mUIStationButton->setEnabled(true);
}

void LiveboardViewImpl::setLiveboard(LiveboardPointer* iLiveboard)
{
    qDebug() << "+ " << Q_FUNC_INFO;    
    stopLoader();

    // Preserve the scroll position
    bool tScrollToTop = (mDepartures.count() == 0);

    // Append the results
    int tAppendedItems = 0;
    for (int i = 0; i < (*iLiveboard)->departures().size(); i++)
    {
        // Check if the item isn't present already
        const Liveboard::Departure tDeparture = (*iLiveboard)->departures().at(i);
        bool tPresent = false;
        for (int j = 0; j < mDepartures.size(); j++)
        {
            // TODO: comparison operator for data objects
            if (mDepartures.at(j).station == tDeparture.station && mDepartures.at(j).datetime == tDeparture.datetime)
            {
                tPresent = true;
            }
        }

        // If not found, add this and all others
        if (!tPresent)
        {
            for (int j = i; j < (*iLiveboard)->departures().size(); j++)
            {
                mDepartures.append((*iLiveboard)->departures().at(j));
                tAppendedItems++;
            }
            break;
        }
    }

    // Show the results
    if (tAppendedItems == 0)
    {
        if (mDepartures.count() == 0)
        {
            showError(tr("The liveboard seems to be empty."));
            populateModel();
        }
        else
            showError(tr("No new items received."));
    }
    else
        populateModel();

    // Scroll
    if (tScrollToTop)
        mView->scrollToTop();
    else
        mView->scrollTo(mModel->index(mModel->rowCount()-tAppendedItems-1, 0));

    delete iLiveboard;
}
//
// Initialization
//

void LiveboardViewImpl::init_ui()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Window settings
    setWindowTitle(tr("Departures"));

    // Main layout
    QVBoxLayout *tUILayout = new QVBoxLayout();
    tUILayout->setAlignment(Qt::AlignTop);
    centralWidget()->setLayout(tUILayout);


    // VIEW //

    // Create the departure list model
    mModel = new QStandardItemModel(0, 1);

    // Create the departure listview
    mView = new QListView();
    mView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mView->setModel(mModel);
    mView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mView->setSelectionMode(QAbstractItemView::SingleSelection);
    mView->setItemDelegate(new LiveboardDepartureDelegate(mStations));
    connect(mView, SIGNAL(activated(QModelIndex)), this, SLOT(do_lstDepartures_activated(QModelIndex)));
    tUILayout->addWidget(mView);

    // Create the departure listview dummy
    mViewDummy = new QLabel(tr("No departures to be shown."));
    mViewDummy->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mViewDummy->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
    mViewDummy->setEnabled(false);
    QFont font;
    font.setPointSize(24);
    mViewDummy->setFont(font);
    tUILayout->addWidget(mViewDummy);


    // VIEW HEADER //

    mViewHeader = new QWidget();
    QHBoxLayout *tViewImplHeaderLayout = new QHBoxLayout;
    mViewHeader->setLayout(tViewImplHeaderLayout);

    // Station button
    mUIStationButton = new QPushButton(tr("Station"));
    mUIStationButton->setEnabled(false);
    tViewImplHeaderLayout->addWidget(mUIStationButton);
    connect(mUIStationButton, SIGNAL(clicked()), this, SLOT(do_btnStations_clicked()));

    // Station edit
    mUIStationEdit = new QLineEdit;
    mUIStationEdit->setEnabled(false);
    tViewImplHeaderLayout->addWidget(mUIStationEdit);

    // Add to the view
    mModel->appendRow(new QStandardItem());
    mView->setIndexWidget(mModel->index(0, 0), mViewHeader);
    mModel->item(0, 0)->setSelectable(false);

    // OTHER //

    // Populate the model
    populateModel();

}


//
// Auxiliary
//

void LiveboardViewImpl::populateModel()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Remove all rows but the header
    if (mModel->rowCount() > 1)
    {
        mModel->removeRows(1, mModel->rowCount()-1);
    }

    if (mDepartures.size() == 0)
    {
        mViewDummy->setVisible(true);
        mView->setFixedHeight(mViewHeader->height());   // HACK
    }
    else
    {
        // Add the departures
        for (int i = 0; i < mDepartures.size(); i++)
        {
            Liveboard::Departure tDeparture = mDepartures.at(i);
            QStandardItem *tItem = new QStandardItem;

            tItem->setData(QVariant::fromValue(tDeparture), LiveboardDepartureRole);
            mModel->appendRow(tItem);
        }

        // Add the "more" button
        QPushButton *tMore = new QPushButton(tr("More"));
        connect(tMore, SIGNAL(clicked()), this, SLOT(do_btnMore_clicked()));
        mModel->appendRow(new QStandardItem());
        mView->setIndexWidget(mModel->index(mModel->rowCount()-1, 0), tMore);
        mModel->item(mModel->rowCount()-1, 0)->setSelectable(false);

        mViewDummy->setVisible(false);
        mView->setFixedHeight(centralWidget()->maximumHeight());   // HACK
    }
}
