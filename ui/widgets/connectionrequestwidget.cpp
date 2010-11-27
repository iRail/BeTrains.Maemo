//
// Configuration
//

// Includes
#include <QDebug>
#include "connectionrequestwidget.h"
#include "ui/dialogs/stationchooser.h"
#include <QtMaemo5/QMaemo5InformationBox>
#include <QButtonGroup>
#include <QProgressDialog>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

ConnectionRequestWidget::ConnectionRequestWidget(const QMap<QString, StationPointer>& iStations, QWidget *iParent) : QWidget(iParent), mStations(iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Initialisation
    init_ui();
    init_children();
}

ConnectionRequestWidget::~ConnectionRequestWidget()
{
    qDebug() << "~ " << Q_FUNC_INFO;

}


void ConnectionRequestWidget::load(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

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
    qDebug() << "+ " << Q_FUNC_INFO;

    mUITypeDeparture->setEnabled(iUseDatetime);
    mUITypeArrival->setEnabled(iUseDatetime);
    mUIDate->setEnabled(iUseDatetime);
    mUITime->setEnabled(iUseDatetime);
}

void ConnectionRequestWidget::search()
{
    qDebug() << "+ " << Q_FUNC_INFO;

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

        emit finished(tConnectionRequest);
    }
}

void ConnectionRequestWidget::clear_datetime()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mUITimePicker->setCurrentTime(QTime::currentTime());
    mUIDatePicker->setCurrentDate(QDate::currentDate());
}

void ConnectionRequestWidget::clear()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mUIFromLine->clear();
    mUIToLine->clear();
    clear_datetime();
    mUITypeDeparture->setChecked(true);
}

void ConnectionRequestWidget::stations_pick_from()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mTarget = mUIFromLine;
    stations_load();
}

void ConnectionRequestWidget::stations_pick_to()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mTarget = mUIToLine;
    stations_load();
}

void ConnectionRequestWidget::stations_load()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    StationChooser tChooser(mStations, this);
    int tReturn = tChooser.exec();
    if (tReturn == QDialog::Accepted)
    {
        StationPointer tStation = tChooser.getSelection();
        mTarget->setText(tStation->name());
    }
}


//
// Initialization
//

void ConnectionRequestWidget::init_ui()
{
    qDebug() << "+ " << Q_FUNC_INFO;

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


    // OTHER //

    // Tab order
    setTabOrder(mUIFromLine, mUIToLine);

    // Submit by return
    connect(mUIFromLine, SIGNAL(returnPressed()), this, SLOT(search()));
    connect(mUIToLine, SIGNAL(returnPressed()), this, SLOT(search()));

}

void ConnectionRequestWidget::init_children()
{
    qDebug() << "+ " << Q_FUNC_INFO;

}

