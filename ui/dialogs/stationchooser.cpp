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
    populateModel("");

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
// UI events
//

void StationChooser::do_txtFilter_textEdited(QString iText)
{
    // TODO: the following code, albeit nice, causes events to be
    //       dropped when the filter edit is hidden
    /*
    if (iText.length())
        mFilter->show();
    else
        mFilter->hide();
    */
    populateModel(iText);
}



//
// Initialization
//

void StationChooser::init_ui()
{
    // Dialog configuration
    resize(parentWidget()->size());
    setWindowTitle(QString(tr("Pick a station")));


    // LISTVIEW //

    // Create the listview layout
    mViewLayout = new QVBoxLayout();

    // Populate the list model
    mModel = new QStandardItemModel(0, 1);

    // Create the listview
    mView = new QListView();
    mView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mView->setModel(mModel);
    mView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mView->setSelectionMode(QAbstractItemView::SingleSelection);
    mView->setResizeMode(QListView::Adjust);
    mViewLayout->addWidget(mView);

    // Create the listview dummy
    mViewDummy = new QLabel(tr("No stations found"));
    mViewDummy->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mViewDummy->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    mViewDummy->setEnabled(false);
    QFont font;
    font.setPointSize(24);
    mViewDummy->setFont(font);
    mViewLayout->addWidget(mViewDummy);

    // Create the listview filter
    mFilter = new QLineEdit();
    connect(mFilter, SIGNAL(textEdited(QString)), SLOT(do_txtFilter_textEdited(QString)));
    mView->setFocusProxy(mFilter);
    mViewLayout->addWidget(mFilter);


    // BUTTON //

    // Create the button
    QVBoxLayout *mUIButtonContainer = new QVBoxLayout();
    QPushButton *mUIButton = new QPushButton(QString(tr("Select")));
    connect(mUIButton, SIGNAL(clicked()), this, SLOT(accept()));
    mUIButtonContainer->addStretch();
    mUIButtonContainer->addWidget(mUIButton);

    // Create the layout
    QHBoxLayout *mUILayout = new QHBoxLayout(this);
    mUILayout->setAlignment(Qt::AlignBottom);
    mUILayout->addLayout(mViewLayout);
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

void StationChooser::populateModel(QString iPrefix)
{
    QList<StationPointer> tStationList = mStations.values();
    qSort(tStationList.begin(), tStationList.end(), compareStationsByName);

    mModel->clear();

    for (int i = 0; i < tStationList.size(); i++)
    {
        StationPointer tStation = tStationList.at(i);
        if (tStation->name().startsWith(iPrefix, Qt::CaseInsensitive))
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

    if (mModel->rowCount() > 0)
    {
        mViewDummy->setVisible(false);
        mView->setVisible(true);
    }
    else
    {
        mViewDummy->setVisible(true);
        mView->setVisible(false);
    }
}
