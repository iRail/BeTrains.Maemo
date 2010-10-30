//
// Configuration
//

// Includes
#include "stationchooser.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QtMaemo5/QMaemo5InformationBox>
#include <QStringBuilder>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

StationChooser::StationChooser(CachedAPI* iAPI, QWidget* iParent) : QDialog(iParent), mAPI(iAPI), mParent(iParent)
{
    // Initialisation
    init_ui();
    init_children();

    // Progress dialog
    mChildProgressDialog->setEnabled(true);
    mChildProgressDialog->setWindowTitle(tr("Fetching list of stations"));

    // Fetch the stations
    connect(mAPI, SIGNAL(replyStations(QList<StationPointer>*)), this, SLOT(gotStations(QList<StationPointer>*)));
    mAPI->requestStations();
}

StationChooser::~StationChooser()
{
    delete mModel;
    delete mChildProgressDialog;
}



//
// Initialization
//

void StationChooser::init_ui()
{
    // Dialog configuration
    resize(mParent->size());
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
    // Construct and connect the progress dialog (we can persistently connect
    // as the dialog'll only be used for API progress)
    mChildProgressDialog = new OptionalProgressDialog(this);
    connect(mAPI, SIGNAL(progress_start()), mChildProgressDialog, SLOT(show()));
    connect(mAPI, SIGNAL(action(QString)), mChildProgressDialog, SLOT(setLabelText(QString)));
}


//
// Slots
//

void StationChooser::gotStations(QList<StationPointer>* iStations)
{
    mChildProgressDialog->setEnabled(false);
    disconnect(mAPI, SIGNAL(replyStations(QList<StationPointer>*)), this, SLOT(gotStations(QList<StationPointer>*)));

    if (iStations != 0)
    {
        // Populate the model
        populateModel(iStations);
        delete iStations;

        // Initial selection
        QModelIndex tInitial = mModel->index(0, 0);
        mView->setCurrentIndex(tInitial);
        mStation = tInitial.data(StationRole).value<StationPointer>();
    }
    else
    {
        if (mAPI->hasError())
            QMaemo5InformationBox::information(this, tr("Error: ") % mAPI->errorString(), QMaemo5InformationBox::DefaultTimeout);
        else
            QMaemo5InformationBox::information(this, tr("Unknown error"), QMaemo5InformationBox::DefaultTimeout);

        reject();
    }
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

void StationChooser::populateModel(const QList<StationPointer>* iStations)
{
    for (int i = 0; i < iStations->size(); i++)
    {
        StationPointer tStation = iStations->at(i);
        // TODO: delegate
        QStandardItem *tItem = new QStandardItem(tStation->name());
        tItem->setData(QVariant::fromValue(tStation), StationRole);
        tItem->setTextAlignment(Qt::AlignCenter);
        tItem->setEditable(false);
        mModel->appendRow(tItem);
    }
}
