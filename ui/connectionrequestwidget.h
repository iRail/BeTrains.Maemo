//
// Configuration
//

// Include guard
#ifndef CONNECTIONREQUESTWIDGET_H
#define CONNECTIONREQUESTWIDGET_H

// Includes
#include <QWidget>
#include <QList>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QListView>
#include <QDialog>
#include <QRadioButton>
#include <QtMaemo5/QMaemo5DatePickSelector>
#include <QtMaemo5/QMaemo5TimePickSelector>
#include <QtMaemo5/QMaemo5ValueButton>
#include <QProgressDialog>
#include "asyncapi.h"
#include "api/station.h"
#include "api/connectionrequest.h"
#include "dialogs/progressindicator.h"

namespace iRail
{
    class ConnectionRequestWidget : public QWidget
    {
    Q_OBJECT
    public:
        explicit ConnectionRequestWidget(AsyncAPI* iAPI, QWidget *iParent);
        void load(ConnectionRequestPointer iConnectionRequest);

    public slots:
        void use_datetime(bool iUseDatetime);
        void clear();
        void clear_datetime();
        void search();
        void stations_fetch();
        void stations_pick_to();
        void stations_pick_from();
        void stations_pick(QList<StationPointer> iStations);

    signals:
        void search(ConnectionRequestPointer iConnectionRequest);

    private:
        // Member data
        AsyncAPI* mAPI;

        // Initialization
        void init_ui();
        void init_children();

        // UI members
        QLineEdit *mUIFromLine, *mUIToLine;
        QMaemo5ValueButton *mUIDate, *mUITime;
        QMaemo5DatePickSelector *mUIDatePicker;
        QMaemo5TimePickSelector *mUITimePicker;
        QRadioButton *mUIUseTime, *mUITypeDeparture, *mUITypeArrival;

        // Dialogs
        ProgressIndicator *mUIProgressIndicator;

        // Temporary data
        QLineEdit *mTarget;
    };
}

#endif // CONNECTIONREQUESTWIDGET_H
