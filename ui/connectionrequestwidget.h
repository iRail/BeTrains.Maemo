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
#include "cachedapi.h"
#include "api/station.h"
#include "api/connectionrequest.h"
#include "dialogs/optionalprogressdialog.h"

namespace iRail
{
    class ConnectionRequestWidget : public QWidget
    {
    Q_OBJECT
    public:
        explicit ConnectionRequestWidget(CachedAPI* iAPI, QWidget *iParent);
        ~ConnectionRequestWidget();
        void load(ConnectionRequestPointer iConnectionRequest);

        // UI Events
    private slots:
        void use_datetime(bool iUseDatetime);
        void clear();
        void clear_datetime();
        void search();
        void stations_pick_to();
        void stations_pick_from();
        void stations_load();
        void show_station(const QList<StationPointer>& iStations);

    signals:
        void search(ConnectionRequestPointer iConnectionRequest);

    private:
        // Member data
        CachedAPI* mAPI;

        // Initialization
        void init_ui();
        void init_children();

        // UI members
        QLineEdit *mUIFromLine, *mUIToLine;
        QMaemo5ValueButton *mUIDate, *mUITime;
        QMaemo5DatePickSelector *mUIDatePicker;
        QMaemo5TimePickSelector *mUITimePicker;
        QRadioButton *mUIUseTime, *mUITypeDeparture, *mUITypeArrival;

        // UI children
        OptionalProgressDialog* mChildProgressDialog;

        // Temporary data
        QLineEdit *mTarget;
    };
}

#endif // CONNECTIONREQUESTWIDGET_H
