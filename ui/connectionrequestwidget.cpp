//
// Configuration
//

// Includes
#include "connectionrequestwidget.h"
#include "dialogs/stationchooser.h"
#include "connectionresultwidget.h"
#include <QtMaemo5/QMaemo5InformationBox>
#include <QButtonGroup>
#include <QProgressDialog>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

ConnectionRequestWidget::ConnectionRequestWidget(CachedAPI *iAPI, QWidget *iParent) : QWidget(iParent), mAPI(iAPI)
{
    // Initialisation
    init_ui();
    init_children();
}

ConnectionRequestWidget::~ConnectionRequestWidget()
{
    delete mChildProgressDialog;
}


void ConnectionRequestWidget::load(ConnectionRequestPointer iConnectionRequest)
{
    mUIToLine->setText(iConnectionRequest->destination());
    mUIFromLine->setText(iConnectionRequest->origin());
    if (iConnectionRequest->timed())
    {
        mUIUseTime->setChecked(true);
        const ConnectionRequest::Time *tTime = iConnectionRequest->time();
        if (tTime->type == ConnectionRequest::Departure)
            mUITypeDeparture->setChecked(true);
        else
            mUITypeArrival->setChecked(true);
        mUITimePicker->setCurrentTime(tTime->datetime.time());
        mUIDatePicker->setCurrentDate(tTime->datetime.date());
    }
    else
        mUIUseTime->setChecked(false);
}


//
// UI Events
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
            ConnectionRequest::Time tTime;
            tTime.type = mUITypeDeparture->isChecked() ? ConnectionRequest::Departure : ConnectionRequest::Arrival;
            tTime.datetime.setTime(mUITimePicker->currentTime());
            tTime.datetime.setDate(mUIDatePicker->currentDate());
            tConnectionRequest->setTime(tTime);
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

void ConnectionRequestWidget::stations_pick_from()
{
    mTarget = mUIFromLine;
    stations_load();
}

void ConnectionRequestWidget::stations_pick_to()
{
    mTarget = mUIToLine;
    stations_load();
}

void ConnectionRequestWidget::stations_load()
{
    // Progress dialog
    mChildProgressDialog->setEnabled(true);
    mChildProgressDialog->setWindowTitle(tr("Fetching list of stations"));

    // Fetch the stations
    mAPI->requestStations();
    connect(mAPI, SIGNAL(replyStations(QList<StationPointer>)), this, SLOT(show_station(QList<StationPointer>)));
}

void ConnectionRequestWidget::show_station(const QList<StationPointer>& iStations)
{
    mChildProgressDialog->setEnabled(false);

    StationChooser tChooser(&iStations, this);
    int tReturn = tChooser.exec();
    if (tReturn == QDialog::Accepted)
    {
        StationPointer tStation = tChooser.getSelection();
        mTarget->setText(tStation->name());
    }
    disconnect(mAPI, SIGNAL(replyStations(QList<StationPointer>)), this, SLOT(show_station(QList<StationPointer>)));
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
    // Construct and connect the progress dialog (we can persistently connect
    // as the dialog'll only be used for API progress)
    mChildProgressDialog = new OptionalProgressDialog(this);
    connect(mAPI, SIGNAL(progress_start()), mChildProgressDialog, SLOT(show()));
    connect(mAPI, SIGNAL(action(QString)), mChildProgressDialog, SLOT(setLabelText(QString)));
}

