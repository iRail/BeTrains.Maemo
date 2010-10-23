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

StationChooser::StationChooser(const QList<StationPointer>* iStations, QWidget* parent) : QDialog(parent), mStations(iStations)
{
    // Dialog configuration
    resize(parent->size());
    setWindowTitle(QString(tr("Pick a station")));

    // Populate the list model
    mModel = new QStandardItemModel(0, 1);
    getStationsModel(mModel);

    // Create the listview
    mView = new QListView();
    mView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mView->setModel(mModel);
    mView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mView->setSelectionMode(QAbstractItemView::SingleSelection);

    // Initial selection
    // TODO: initial selection by construction
    QModelIndex tInitial = mModel->index(0, 0);
    mView->setCurrentIndex(tInitial);
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
    mUILayout->addWidget(mView);
    mUILayout->addLayout(mUIButtonContainer);
}

StationChooser::~StationChooser()
{
    delete mModel;
}


//
// Basic IO
//

StationPointer StationChooser::getSelection()
{
    return mView->currentIndex().data(StationRole).value<StationPointer>();
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
