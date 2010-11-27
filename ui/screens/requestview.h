//
// Configuration
//

// Include guard
#ifndef REQUESTVIEW_H
#define REQUESTVIEW_H

// Inclused
#include <QDebug>
#include "ui/global.h"
#include "api/connection.h"
#include "api/connectionrequest.h"
#include "api/vehicle.h"
#include "api/liveboard.h"
#include <QObject>
#include <QScrollArea>
#include <QListView>
#include <QStandardItem>
#include <QModelIndex>
#include <QLabel>
#include <QtMaemo5/QMaemo5DatePickSelector>
#include <QtMaemo5/QMaemo5TimePickSelector>
#include <QtMaemo5/QMaemo5ValueButton>
#include <QRadioButton>
#include <QLineEdit>
#include <QButtonGroup>

namespace iRail
{
    class RequestView : public QWidget
    {

    Q_OBJECT
    public:
        RequestView(QWidget *iParent);
        ~RequestView();
        void showUI();
        void showUI(ConnectionRequestPointer iInitialRequest);
        void load(ConnectionRequestPointer iConnectionRequest);

        // UI events
    private slots:
        void _showConnectionRequest();
        void _showConnectionRequest(const QMap<QString, StationPointer>& iStations);
        void use_datetime(bool iUseDatetime);
        void search();
        void clear_datetime();
        void clear();
        void stations_pick_to();
        void stations_pick_from();
        void stations_load();

        // Controller actions
    public slots:
        void setStations(QMap<QString, StationPointer>* iStations);
        void showError(const QString& iError);

        // Controller signals
    signals:
        void downloadStations();
        void launchConnection(ConnectionRequestPointer iConnectionRequest);

    private:
        // Member data
        QMap<QString, StationPointer> mStations;

        // Initialization
        void init_ui();
        void init_children();

        // UI members
        QPushButton *mUIFromButton, *mUIToButton;
        QLineEdit *mUIFromLine, *mUIToLine;
        QMaemo5ValueButton *mUIDate, *mUITime;
        QMaemo5DatePickSelector *mUIDatePicker;
        QMaemo5TimePickSelector *mUITimePicker;
        QRadioButton *mUIUseTime, *mUITypeDeparture, *mUITypeArrival;

        // Temporary data
        QLineEdit *mTarget;
    };
}

#endif // REQUESTVIEW_H
