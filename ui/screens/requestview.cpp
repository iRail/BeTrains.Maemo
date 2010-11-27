//
// Configuration
//

// Includes
#include "requestview.h"
#include "ui/auxiliary/delegates/connectionrequestdelegate.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFont>
#include "ui/dialogs/stationchooser.h"
#include <QtMaemo5/QMaemo5InformationBox>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

RequestView::RequestView(QWidget* iParent) : GenericView(iParent)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Initialisation
    init_ui();
    init_children();
    this->hide();
}

RequestView::~RequestView()
{
    qDebug() << "~ " << Q_FUNC_INFO;

}

void RequestView::showUI()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    _showConnectionRequest();
    GenericView::showUI();
}

void RequestView::showUI(ConnectionRequestPointer iInitialRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    _showConnectionRequest();
    load(iInitialRequest);
    this->show();
}

void RequestView::load(ConnectionRequestPointer iConnectionRequest)
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
// UI events
//

void RequestView::_showConnectionRequest()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    emit downloadStations();
}

void RequestView::_showConnectionRequest(const QMap<QString, StationPointer>& iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mStations = iStations;
    mUIFromButton->setEnabled(true);
    mUIToButton->setEnabled(true);

    clear();
}

void RequestView::use_datetime(bool iUseDatetime)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mUITypeDeparture->setEnabled(iUseDatetime);
    mUITypeArrival->setEnabled(iUseDatetime);
    mUIDate->setEnabled(iUseDatetime);
    mUITime->setEnabled(iUseDatetime);
}

void RequestView::search()
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

        emit launchConnection(tConnectionRequest);
    }
}

void RequestView::clear_datetime()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mUITimePicker->setCurrentTime(QTime::currentTime());
    mUIDatePicker->setCurrentDate(QDate::currentDate());
}

void RequestView::clear()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mUIFromLine->clear();
    mUIToLine->clear();
    clear_datetime();
    mUITypeDeparture->setChecked(true);
}

void RequestView::stations_pick_from()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mTarget = mUIFromLine;
    stations_load();
}

void RequestView::stations_pick_to()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mTarget = mUIToLine;
    stations_load();
}

void RequestView::stations_load()
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
// Controller actions
//

void RequestView::setStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    _showConnectionRequest(*iStations);
    delete iStations;
}


//
// Initialization
//

void RequestView::init_ui()
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
    mUIFromButton = new QPushButton(QString(tr("Origin")));
    mUIFromButton->setEnabled(false);
    mUIFromLine = new QLineEdit;
    mUIFrom->addWidget(mUIFromButton);
    mUIFrom->addWidget(mUIFromLine);
    connect(mUIFromButton, SIGNAL(clicked()), this, SLOT(stations_pick_from()));
    mUILayout->addLayout(mUIFrom);

    // Destination station
    QHBoxLayout *mUITo = new QHBoxLayout();
    mUIToButton = new QPushButton(QString(tr("Destination")));
    mUIToButton->setEnabled(false);
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

void RequestView::init_children()
{
    qDebug() << "+ " << Q_FUNC_INFO;

}

