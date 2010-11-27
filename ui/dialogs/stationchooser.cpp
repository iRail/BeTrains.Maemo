//
// Configuration
//

// Includes
#include "stationchooser.h"
#include <QVBoxLayout>
#include <QtMaemo5/QMaemo5InformationBox>
#include <QStringBuilder>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

StationChooser::StationChooser(const QMap<QString, StationPointer>& iStations, QWidget* iParent) : QDialog(iParent), mStations(iStations)
{
    // Initialisation
    init_ui();
    init_children();

    // Populate the model
    populateModel();

    // Initial selection
    QModelIndex tInitial = mModel->index(0, 0);
    mView->setCurrentIndex(tInitial);
    mStation = tInitial.data(StationRole).value<StationPointer>();
}

StationChooser::~StationChooser()
{
    delete mModel;
}



//
// Initialization
//

void StationChooser::init_ui()
{
    // Dialog configuration
    resize(parentWidget()->size());
    setWindowTitle(QString(tr("Pick a station")));

    // Populate the list model
    mModel = new QStandardItemModel(0, 1);

    // Create the listview
    mView = new QListView();
    mView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mView->setModel(mModel);
    mView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mView->setSelectionMode(QAbstractItemView::SingleSelection);

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

void StationChooser::init_children()
{
}


//
// Basic IO
//

QString StationChooser::getSelection()
{
    StationPointer tStation = mView->currentIndex().data(StationRole).value<StationPointer>();
    return tStation->id();
}

//
// Auxiliary
//

bool compareStationsByName(const StationPointer& iFirst, const StationPointer& iSecond)
{
    return iFirst->name().toLower() < iSecond->name().toLower();
}

void StationChooser::populateModel()
{
    QList<StationPointer> tStationList = mStations.values();
    qSort(tStationList.begin(), tStationList.end(), compareStationsByName);

    for (int i = 0; i < tStationList.size(); i++)
    {
        StationPointer tStation = tStationList.at(i);
        // TODO: delegate
        QStandardItem *tItem = new QStandardItem(tStation->name());
        tItem->setData(QVariant::fromValue(tStation), StationRole);
        tItem->setTextAlignment(Qt::AlignCenter);
        tItem->setEditable(false);
        mModel->appendRow(tItem);
    }
}
