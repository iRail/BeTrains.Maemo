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

LiveboardWidget::LiveboardWidget(const QMap<QString, StationPointer>& iStations, QWidget *iParent) : QScrollArea(iParent), mStations(iStations)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    // Initialisation
    init_ui();
    init_children();
}

LiveboardWidget::~LiveboardWidget()
{
    qDebug() << "~ " << __PRETTY_FUNCTION__;

}


void LiveboardWidget::load(LiveboardPointer iLiveboard)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    populateModel(iLiveboard->departures());
}


//
// UI Events
//

void LiveboardWidget::do_search()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    if (mUIStationEdit->text().length() == 0)
    {
        QMaemo5InformationBox::information(this, tr("Please fill in the station."), QMaemo5InformationBox::DefaultTimeout);
    }
    else
    {
        emit request(tStationId);
    }
}

void LiveboardWidget::clear()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    mUIStationEdit->clear();
    populateModel(QList<Liveboard::Departure>());
}

void LiveboardWidget::do_stations()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

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
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    emit finished(iIndex.data(LiveboardDepartureRole).value<Liveboard::Departure>());
}


//
// Initialization
//

void LiveboardWidget::init_ui()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    // Window settings
    this->setWindowTitle(tr("Departures"));

    // Parent widget
    QWidget *tWidget = new QWidget();
    setWidget(tWidget);
    setWidgetResizable(true);

    // Main layout
    QVBoxLayout* mUILayout = new QVBoxLayout(this);
    tWidget->setLayout(mUILayout);


    // STATION //

    QHBoxLayout *mUIStation = new QHBoxLayout();

    // Station Button
    QPushButton *mUIStationButton = new QPushButton(QString(tr("Station")));
    mUIStation->addWidget(mUIStationButton);
    connect(mUIStationButton, SIGNAL(clicked()), this, SLOT(do_stations()));

    // Station Edit
    mUIStationEdit = new QLineEdit;
    mUIStationEdit->setEnabled(false);
    mUIStation->addWidget(mUIStationEdit);

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
    populateModel(QList<Liveboard::Departure>());

}

void LiveboardWidget::init_children()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

}


//
// Auxiliary
//

void LiveboardWidget::populateModel(const QList<Liveboard::Departure>& iDepartures)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

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
}
