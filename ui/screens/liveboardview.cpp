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
    this->hide();
    init_ui();
}

void LiveboardView::load()
{
    qDebug() << "+ " << Q_FUNC_INFO;
    startLoader();

    emit downloadStations();
}

void LiveboardView::load(const QMap<QString, StationPointer>& iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;
    stopLoader();

    mStations = iStations;
    mUIStationButton->setEnabled(true);
    clear();
}

void LiveboardView::load(QString iStationId)
{
    qDebug() << "+ " << Q_FUNC_INFO;
    startLoader();

    emit downloadLiveboard(iStationId);
}

void LiveboardView::load(LiveboardPointer iLiveboard)
{
    qDebug() << "+ " << Q_FUNC_INFO;
    stopLoader();

    // Show the results
    if (iLiveboard->departures().count() > 0)
        populateModel(iLiveboard->departures());
    else
        showError(tr("The liveboard seems to be empty."));
}


//
// UI events
//

void LiveboardView::do_btnSearch_clicked()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    if (mUIStationEdit->text().length() == 0)
    {
        QMaemo5InformationBox::information(this, tr("Please fill in the station."), QMaemo5InformationBox::DefaultTimeout);
    }
    else
    {
        emit load(tStationId);
    }
}

void LiveboardView::clear()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mUIStationEdit->clear();
    populateModel(QList<Liveboard::Departure>());
}

void LiveboardView::do_btnStations_clicked()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    StationChooser tChooser(mStations, this);
    int tReturn = tChooser.exec();
    if (tReturn == QDialog::Accepted)
    {
        tStationId = tChooser.getSelection();
        mUIStationEdit->setText(mStations[tStationId]->name());
    }
}

void LiveboardView::do_lstDepartures_doubleClicked(QModelIndex iIndex)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    //emit finished(iIndex.data(LiveboardDepartureRole).value<Liveboard::Departure>());
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

void LiveboardView::setVehicle(VehiclePointer* iVehicle)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    delete iVehicle;
    qWarning() << "! " << "Handler not currently used";
}

void LiveboardView::setLiveboard(LiveboardPointer* iLiveboard)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    load(*iLiveboard);
}
//
// Initialization
//

void LiveboardView::init_ui()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Window settings
    setWindowTitle(tr("Departures"));

    // Scroll area
    QVBoxLayout* mUILayout = new QVBoxLayout(this);
    mUILayout->setMargin(0);
    mUIScrollArea = new QScrollArea(this);
    mUILayout->addWidget(mUIScrollArea);

    // Parent widget
    QWidget *tWidget = new QWidget();
    mUIScrollArea->setWidget(tWidget);
    mUIScrollArea->setWidgetResizable(true);

    // Main layout
    QVBoxLayout *mUIScrollLayout = new QVBoxLayout(mUIScrollArea);
    tWidget->setLayout(mUIScrollLayout);


    // STATION //

    QHBoxLayout *mUIStation = new QHBoxLayout();

    // Station Button
    mUIStationButton = new QPushButton(QString(tr("Station")));
    mUIStationButton->setEnabled(false);
    mUIStation->addWidget(mUIStationButton);
    connect(mUIStationButton, SIGNAL(clicked()), this, SLOT(do_btnStations_clicked()));

    // Station Edit
    mUIStationEdit = new QLineEdit;
    mUIStationEdit->setEnabled(false);
    mUIStation->addWidget(mUIStationEdit);

    // Search button
    QPushButton *mUISearchButton = new QPushButton(QString(tr("Search")));
    mUIStation->addWidget(mUISearchButton);
    connect(mUISearchButton, SIGNAL(clicked()), this, SLOT(do_btnSearch_clicked()));

    mUIScrollLayout->addLayout(mUIStation);


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
    connect(mView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(do_lstDepartures_doubleClicked(QModelIndex)));
    mUIScrollLayout->addWidget(mView);

    // Create the history listview dummy
    mViewDummy = new QLabel();
    mViewDummy->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mViewDummy->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    mViewDummy->setEnabled(false);
    QFont font;
    font.setPointSize(24);
    mViewDummy->setFont(font);
    mUIScrollLayout->addWidget(mViewDummy);
    populateModel(QList<Liveboard::Departure>());

}


//
// Auxiliary
//

void LiveboardView::populateModel(const QList<Liveboard::Departure>& iDepartures)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mModel->clear();
    if (iDepartures.size() == 0)
    {
        mViewDummy->setText(tr("No departures to be shown."));
        mViewDummy->setVisible(true);
        mView->setVisible(false);
    }
    else
    {
        for (int i = 0; i < iDepartures.size(); i++)
        {
            Liveboard::Departure tDeparture = iDepartures.at(i);
            QStandardItem *tItem = new QStandardItem;

            tItem->setData(QVariant::fromValue(tDeparture), LiveboardDepartureRole);
            mModel->appendRow(tItem);
        }

        mViewDummy->setVisible(false);
        mView->setVisible(true);
        mView->setModel(mModel);
        mView->setFixedHeight(70*mModel->rowCount());   // HACK
    }

    // Fix the scroll location
    mUIScrollArea->ensureVisible(0, 0, 0, 0);
}
