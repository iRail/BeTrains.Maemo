//
// Configuration
//

// Includes
#include "stationchooser.h"
#include <QDebug>
#include <QVBoxLayout>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

StationChooser::StationChooser(QList<StationPointer>* iStations, QWidget* parent) : QDialog(parent), mStations(iStations)
{
    // Dialog configuration
    resize(parent->size());
    setWindowTitle(QString(tr("Pick a station")));

    // Populate the list model
    tModel = new QStandardItemModel(0, 1);
    getStationsModel(tModel);

    // Create the listview
    QListView *tView = new QListView();
    tView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tView->setModel(tModel);
    tView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tView->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(tView, SIGNAL(clicked(QModelIndex)), this, SLOT(clicked(QModelIndex)));

    // Initial selection
    // TODO: initial selection by construction
    QModelIndex tInitial = tModel->index(0, 0);
    tView->setCurrentIndex(tInitial);
    mStation = tInitial.data(StationRole).value<StationPointer>();

    // Create the button
    QVBoxLayout *mUIButtonContainer = new QVBoxLayout();
    QPushButton *mUIButton = new QPushButton(QString(tr("Select")));
    connect(mUIButton, SIGNAL(clicked()), this, SLOT(accept()));
    mUIButtonContainer->addStretch();
    mUIButtonContainer->addWidget(mUIButton);

    // Create the layout
    QHBoxLayout *mUILayout = new QHBoxLayout(this);
    mUILayout->setAlignment(Qt::AlignBottom);
    mUILayout->addWidget(tView);
    mUILayout->addLayout(mUIButtonContainer);
}

StationChooser::~StationChooser()
{
    delete tModel;
}


//
// Basic IO
//

StationPointer StationChooser::getSelection()
{
    return mStation;
}


//
// Public slots
//

void StationChooser::clicked(const QModelIndex &index)
{
    mStation = index.data(StationRole).value<StationPointer>();
}


//
// Auxiliary
//

void StationChooser::getStationsModel(QStandardItemModel *iModel)
{
    for (int i = 0; i < mStations->size(); i++)
    {
        StationPointer tStation = mStations->at(i);
        QStandardItem *tItem = new QStandardItem(tStation->getName());
        tItem->setData(QVariant::fromValue(tStation), StationRole);
        tItem->setTextAlignment(Qt::AlignCenter);
        tItem->setEditable(false);
        iModel->appendRow(tItem);
    }
}
