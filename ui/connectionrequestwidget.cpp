//
// Configuration
//

// Includes
#include "connectionrequestwidget.h"
#include "dialogs/stationchooser.h"
#include "connectionresultwidget.h"
#include <QtMaemo5/QMaemo5InformationBox>
#include <QButtonGroup>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

ConnectionRequestWidget::ConnectionRequestWidget(AsyncAPI *iAPI, QWidget *iParent) : QWidget(iParent), mAPI(iAPI)
{
    // Initialisation
    init_ui();
    init_children();
}


void ConnectionRequestWidget::load(ConnectionRequestPointer iConnectionRequest)
{
    mUIToLine->setText(iConnectionRequest->getDestination());
    mUIFromLine->setText(iConnectionRequest->getOrigin());
    if (iConnectionRequest->hasDateTime())
    {
        ConnectionRequest::DateTime tDateTime = iConnectionRequest->getDateTime();
        if (tDateTime.type == ConnectionRequest::Departure)
            mUITypeDeparture->setChecked(true);
        else
            mUITypeArrival->setChecked(true);
        mUITimePicker->setCurrentTime(tDateTime.datetime.time());
        mUIDatePicker->setCurrentDate(tDateTime.datetime.date());
    }
}


//
// Public slots
//

void ConnectionRequestWidget::use_datetime(bool iUseDatetime)
{
    mUITypeDeparture->setEnabled(iUseDatetime);
    mUITypeArrival->setEnabled(iUseDatetime);
    mUIDate->setEnabled(iUseDatetime);
    mUITime->setEnabled(iUseDatetime);
}

void ConnectionRequestWidget::search()
{
    if (mUIFromLine->text().length() == 0 || mUIToLine->text().length() == 0)
    {
        QMaemo5InformationBox::information(this, tr("Please fill in both locations."), QMaemo5InformationBox::DefaultTimeout);
    }
    else
    {
        ConnectionRequestPointer tConnectionRequest = ConnectionRequestPointer(new ConnectionRequest(mUIFromLine->text(), mUIToLine->text()));

        if (mUIUseTime->isChecked())
        {
            ConnectionRequest::DateTime tDateTime;
            tDateTime.type = mUITypeDeparture->isChecked() ? ConnectionRequest::Departure : ConnectionRequest::Arrival;
            tDateTime.datetime.setTime(mUITimePicker->currentTime());
            tDateTime.datetime.setDate(mUIDatePicker->currentDate());
            tConnectionRequest->setDateTime(tDateTime);
        }

        emit search(tConnectionRequest);

        ConnectionResultWidget *tWidget = new ConnectionResultWidget(mAPI, tConnectionRequest, this);
        tWidget->setWindowFlags(Qt::Window);
        tWidget->show();
    }
}

void ConnectionRequestWidget::clear_datetime()
{
    mUITimePicker->setCurrentTime(QTime::currentTime());
    mUIDatePicker->setCurrentDate(QDate::currentDate());
}

void ConnectionRequestWidget::clear()
{
    mUIFromLine->clear();
    mUIToLine->clear();
    clear_datetime();
    mUITypeDeparture->setChecked(true);
}

void ConnectionRequestWidget::stations_fetch()
{
    // Configure progress dialog
    // TODO: maybe put this in the StationChooser widget?
    mUIProgressIndicator = new ProgressIndicator(QString(tr("Fetching list of stations")), this);
    mUIProgressIndicator->next(QString(tr("Performing network query...")));
    connect(mAPI, SIGNAL(progress(int)), mUIProgressIndicator, SLOT(setSubProgress(int)));

    mAPI->stations_request();
    connect(mAPI, SIGNAL(stations_reply(QList<StationPointer>)), this, SLOT(stations_pick(QList<StationPointer>)));
}

void ConnectionRequestWidget::stations_pick_from()
{
    mTarget = mUIFromLine;
    try
    {
        stations_pick(mAPI->stations_cache());
    }
    catch (StorageException&)
    {
        stations_fetch();
    }
}

void ConnectionRequestWidget::stations_pick_to()
{
    mTarget = mUIToLine;
    try
    {
        stations_pick(mAPI->stations_cache());
    }
    catch (StorageException&)
    {
        stations_fetch();
    }
}

void ConnectionRequestWidget::stations_pick(QList<StationPointer> iStations)
{
    // Close progress dialog
    if (mUIProgressIndicator)
    {
        mUIProgressIndicator->done();
        delete mUIProgressIndicator;
        mUIProgressIndicator = 0;
    }

    StationChooser tChooser(&iStations, this);
    int tReturn = tChooser.exec();
    if (tReturn == QDialog::Accepted)
    {
        StationPointer tStation = tChooser.getSelection();
        mTarget->setText(tStation->getName());
    }
}

//
// Initialization
//

void ConnectionRequestWidget::init_ui()
{
    // Window settings
    this->setWindowTitle(tr("New query"));

    // Main layout
    QVBoxLayout *mUILayout = new QVBoxLayout(this);
    mUILayout->setAlignment(Qt::AlignTop);


    // FROM AND TO //

    // Origin station
    QHBoxLayout *mUIFrom = new QHBoxLayout();
    QPushButton *mUIFromButton = new QPushButton(QString(tr("Origin")));
    mUIFromLine = new QLineEdit;
    mUIFrom->addWidget(mUIFromButton);
    mUIFrom->addWidget(mUIFromLine);
    connect(mUIFromButton, SIGNAL(clicked()), this, SLOT(stations_pick_from()));
    mUILayout->addLayout(mUIFrom);

    // Destination station
    QHBoxLayout *mUITo = new QHBoxLayout();
    QPushButton *mUIToButton = new QPushButton(QString(tr("Destination")));
    mUIToLine = new QLineEdit;
    mUITo->addWidget(mUIToButton);
    mUITo->addWidget(mUIToLine);
    connect(mUIToButton, SIGNAL(clicked()), this, SLOT(stations_pick_to()));
    mUILayout->addLayout(mUITo);


    // TIME SELECTION //

    // Departure/Arrival
    QHBoxLayout *mUIType = new QHBoxLayout();
    QButtonGroup *mUITypeGroup = new QButtonGroup();
    mUIType->setSpacing(0);
    mUITypeDeparture = new QRadioButton(QString(tr("Departure")));
    mUITypeArrival = new QRadioButton(QString(tr("Arrival")));
    mUITypeGroup->addButton(mUITypeDeparture);
    mUITypeGroup->addButton(mUITypeArrival);
    mUIType->addWidget(mUITypeDeparture);
    mUIType->addWidget(mUITypeArrival);
    mUITypeDeparture->setChecked(true);
    mUITypeDeparture->setEnabled(false);
    mUITypeArrival->setEnabled(false);

    // Use time?
    QVBoxLayout *mUITypeAndUse = new QVBoxLayout();
    mUIUseTime = new QRadioButton(tr("Specify time and date"), this);
    mUITypeAndUse->addWidget(mUIUseTime);
    mUITypeAndUse->addLayout(mUIType);
    mUIUseTime->setChecked(false);
    connect(mUIUseTime, SIGNAL(toggled(bool)), this, SLOT(use_datetime(bool)));

    // DateTime
    QVBoxLayout *mUIDateTime = new QVBoxLayout();

    // Time
    mUITime = new QMaemo5ValueButton();
    mUITimePicker = new QMaemo5TimePickSelector();
    mUITime->setPickSelector(mUITimePicker);
    mUITime->setValueLayout(QMaemo5ValueButton::ValueBesideText);
    mUITime->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    mUITime->setEnabled(false);

    // Date
    mUIDateTime->addWidget(mUITime);
    mUIDate = new QMaemo5ValueButton();
    mUIDatePicker = new QMaemo5DatePickSelector();
    mUIDate->setPickSelector(mUIDatePicker);
    mUIDate->setValueLayout(QMaemo5ValueButton::ValueBesideText);
    mUIDate->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    mUIDateTime->addWidget(mUIDate);
    mUIDate->setEnabled(false);

    // When?
    QHBoxLayout *mWhen = new QHBoxLayout();
    mWhen->addLayout(mUITypeAndUse);
    mWhen->addLayout(mUIDateTime);
    mUILayout->addStretch();
    mUILayout->addLayout(mWhen);


    // FINAL BUTTONS //

    // Clear and search
    QPushButton *tButtonClear = new QPushButton(QString(tr("Clear")));
    QPushButton *tButtonNow = new QPushButton(QString(tr("Set to now")));
    QPushButton *tButtonSearch = new QPushButton(QString(tr("Search")));
    connect(tButtonClear, SIGNAL(clicked()), this, SLOT(clear()));
    connect(tButtonNow, SIGNAL(clicked()), this, SLOT(clear_datetime()));
    connect(tButtonSearch, SIGNAL(clicked()), this, SLOT(search()));
    QHBoxLayout *tActionButtons = new QHBoxLayout();
    tActionButtons->addWidget(tButtonClear);
    tActionButtons->addWidget(tButtonNow);
    tActionButtons->addStretch();
    tActionButtons->addWidget(tButtonSearch);
    mUILayout->addStretch();
    mUILayout->addLayout(tActionButtons);
}

void ConnectionRequestWidget::init_children()
{

}
