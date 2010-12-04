//
// Configuration
//

// Includes
#include "liveboardview.h"
#include "ui/dialogs/stationchooser.h"
#include <QScrollArea>
#include "ui/auxiliary/delegates/liveboarddeparturedelegate.h"
#include <QtMaemo5/QMaemo5InformationBox>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

LiveboardView::LiveboardView(QWidget* iParent) : GenericView(iParent)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Initialisation
    hide();
    init_ui();
}

void LiveboardView::load()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    clear();
    emit downloadStations();
}

void LiveboardView::load(const QMap<QString, StationPointer>& iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;
    stopLoader();

    mStations = iStations;
    mUIStationButton->setEnabled(true);
}

void LiveboardView::load(LiveboardRequestPointer iLiveboardRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    emit downloadLiveboard(iLiveboardRequest);
}

void LiveboardView::load(LiveboardPointer iLiveboard)
{
    qDebug() << "+ " << Q_FUNC_INFO;
    stopLoader();

    // Initial request: scroll to the top
    if (mDepartures.count() == 0)
        mView->scrollToTop();

    // Append the results
    int tAppendedItems = 0;
    for (int i = 0; i < iLiveboard->departures().size(); i++)
    {
        // Check if the item isn't present already
        const Liveboard::Departure tDeparture = iLiveboard->departures().at(i);
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
            for (int j = i; j < iLiveboard->departures().size(); j++)
            {
                mDepartures.append(iLiveboard->departures().at(j));
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
}


//
// UI events
//

void LiveboardView::clear()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mUIStationEdit->clear();
    mDepartures.clear();
    populateModel();
}

void LiveboardView::do_btnStations_clicked()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mDepartures.clear();

    StationChooser tChooser(mStations, centralWidget());
    int tReturn = tChooser.exec();
    if (tReturn == QDialog::Accepted)
    {
        tLiveboardRequest = LiveboardRequestPointer(new LiveboardRequest(tChooser.getSelection()));
        mUIStationEdit->setText(mStations[tLiveboardRequest->station()]->name());

        emit downloadLiveboard(tLiveboardRequest);
    }
}

void LiveboardView::do_lstDepartures_doubleClicked(QModelIndex iIndex)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    emit launchVehicle(tLiveboardRequest->station(), iIndex.data(LiveboardDepartureRole).value<Liveboard::Departure>());
}

void LiveboardView::do_btnMore_clicked()
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

void LiveboardView::setStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    load(*iStations);
    delete iStations;
}

void LiveboardView::setLiveboard(LiveboardPointer* iLiveboard)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    load(*iLiveboard);
    delete iLiveboard;
}
//
// Initialization
//

void LiveboardView::init_ui()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Window settings
    setWindowTitle(tr("Departures"));

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
    mView->setItemDelegate(new LiveboardDepartureDelegate(mStations));
    connect(mView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(do_lstDepartures_doubleClicked(QModelIndex)));
    tUILayout->addWidget(mView);

    // Create the history listview dummy
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
    QHBoxLayout *tViewHeaderLayout = new QHBoxLayout;
    mViewHeader->setLayout(tViewHeaderLayout);

    // Station button
    mUIStationButton = new QPushButton(tr("Station"));
    mUIStationButton->setEnabled(false);
    tViewHeaderLayout->addWidget(mUIStationButton);
    connect(mUIStationButton, SIGNAL(clicked()), this, SLOT(do_btnStations_clicked()));

    // Station edit
    mUIStationEdit = new QLineEdit;
    mUIStationEdit->setEnabled(false);
    tViewHeaderLayout->addWidget(mUIStationEdit);

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

void LiveboardView::populateModel()
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
        mView->setFixedHeight(centralWidget()->height());   // HACK
    }
}
