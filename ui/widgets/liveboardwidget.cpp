//
// Configuration
//

// Includes
#include "liveboardwidget.h"
#include "ui/dialogs/stationchooser.h"
#include "ui/dialogs/optionalprogressdialog.h"
#include "ui/auxiliary/delegates/liveboarddeparturedelegate.h"
#include <QtMaemo5/QMaemo5InformationBox>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

LiveboardWidget::LiveboardWidget(const QMap<QString, StationPointer>& iStations, QWidget *iParent) : QWidget(iParent), mStations(iStations)
{
    // Initialisation
    init_ui();
    init_children();
}

LiveboardWidget::~LiveboardWidget()
{
}


void LiveboardWidget::load(StationPointer iStation)
{
    // TODO
}


//
// UI Events
//

void LiveboardWidget::do_search()
{
    if (mUIStationEdit->text().length() == 0)
    {
        QMaemo5InformationBox::information(this, tr("Please fill in the station."), QMaemo5InformationBox::DefaultTimeout);
    }
    else
    {
        LiveboardPointer tLiveboard;

        // TODO
        if (tStationId.length() > 0)
        {
            // Lookup using station ID
        }
        else
        {
            // Lookup using textbox string
            // TODO: event -> typing in textbox string == clear saved id
        }

        //emit finished(oLiveboardDeparture);
    }
}

void LiveboardWidget::clear()
{
    mUIStationEdit->clear();
    tStationId = "";
    mLiveboard.clear();
    populateModel();
}

void LiveboardWidget::clear_id()
{
    tStationId = "";
}

void LiveboardWidget::do_stations()
{
    StationChooser tChooser(mStations, this);
    int tReturn = tChooser.exec();
    if (tReturn == QDialog::Accepted)
    {
        // TODO: chooser returns stationpointer??
        StationPointer tStation = tChooser.getSelection();
        mUIStationEdit->setText(tStation->name());
        tStationId = tStation->id();
    }
}

void LiveboardWidget::do_detail(QModelIndex iIndex)
{
    // Bug in Qt? Non-selectable QStandardItem can be doubleClicked...
    if (mLiveboard->departures().size() == 0)
        return;

    emit finished(iIndex.data(LiveboardDepartureRole).value<Liveboard::Departure>());
}


//
// Initialization
//

void LiveboardWidget::init_ui()
{
    // Window settings
    this->setWindowTitle(tr("Departures"));

    // Main layout
    QVBoxLayout *mUILayout = new QVBoxLayout(this);
    mUILayout->setAlignment(Qt::AlignTop);


    // STATION //

    QHBoxLayout *mUIStation = new QHBoxLayout();

    // Station Button
    QPushButton *mUIStationButton = new QPushButton(QString(tr("Station")));
    mUIStation->addWidget(mUIStationButton);
    connect(mUIStationButton, SIGNAL(clicked()), this, SLOT(do_stations()));

    // Station Edit
    mUIStationEdit = new QLineEdit;
    mUIStation->addWidget(mUIStationEdit);
    connect(mUIStationEdit, SIGNAL(editingFinished()), this, SLOT(clear_id()));

    // Search button
    QPushButton *mUISearchButton = new QPushButton(QString(tr("Search")));
    mUIStation->addWidget(mUISearchButton);
    connect(mUISearchButton, SIGNAL(clicked()), this, SLOT(do_search()));

    mUILayout->addLayout(mUIStation);


    // RESULT VIEW //

    // Populate the history list model
    mModel = new QStandardItemModel(0, 1);

    // Create the history listview
    mView = new QListView();
    mView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mView->setModel(mModel);
    mView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mView->setSelectionMode(QAbstractItemView::SingleSelection);
    mView->setItemDelegate(new LiveboardDepartureDelegate(mStations));
    mView->setResizeMode(QListView::Adjust);
    connect(mView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(do_detail(QModelIndex)));
    mUILayout->addWidget(mView);

    // Create the history listview dummy
    mViewDummy = new QLabel();
    mViewDummy->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mViewDummy->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    mViewDummy->setEnabled(false);
    QFont font;
    font.setPointSize(24);
    mViewDummy->setFont(font);
    mUILayout->addWidget(mViewDummy);
    populateModel();

    // OTHER //

    // Submit by return
    connect(mUIStationEdit, SIGNAL(returnPressed()), this, SLOT(do_search()));

}

void LiveboardWidget::init_children()
{
}


//
// Auxiliary
//

void LiveboardWidget::populateModel()
{
    mModel->clear();
    if (mLiveboard.isNull())
    {
        mViewDummy->setText(tr("No station selected."));

        mViewDummy->setVisible(true);
        mView->setVisible(false);
    }
    else if (mLiveboard->departures().size() == 0)
    {
        mViewDummy->setText(tr("No departures from selected station."));
        mViewDummy->setVisible(true);
        mView->setVisible(false);
    }
    else
    {
        const QList<Liveboard::Departure>& tDepartures = mLiveboard->departures();
        for (int i = 0; i < tDepartures.size(); i++)
        {
            Liveboard::Departure tDeparture = tDepartures.at(i);
            QStandardItem *tItem = new QStandardItem;

            tItem->setData(QVariant::fromValue(tDeparture), LiveboardDepartureRole);
            mModel->appendRow(tItem);
        }

        mViewDummy->setVisible(false);
        mView->setVisible(true);
        mView->setModel(mModel);
        mView->setFixedHeight(70*mModel->rowCount());   // HACK
    }
}
