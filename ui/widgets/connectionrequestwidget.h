//
// Configuration
//

// Include guard
#ifndef CONNECTIONREQUESTWIDGET_H
#define CONNECTIONREQUESTWIDGET_H

// Includes
#include <QWidget>
#include <QList>
#include <QMap>
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
#include "api/station.h"
#include "api/connectionrequest.h"

namespace iRail
{
    class ConnectionRequestWidget : public QWidget
    {
    Q_OBJECT
    public:
        explicit ConnectionRequestWidget(const QMap<QString, StationPointer>& iStations, QWidget *iParent);
        ~ConnectionRequestWidget();
        void load(ConnectionRequestPointer iConnectionRequest);
        void configure(QMap<QString, StationPointer>* iStations);

        // Public slots
    public slots:
        void clear();

        // UI Events
    private slots:
        void use_datetime(bool iUseDatetime);
        void search();
        void clear_datetime();
        void stations_pick_to();
        void stations_pick_from();
        void stations_load();

    signals:
        void finished(ConnectionRequestPointer iConnectionRequest);

    private:
        // Member data
        QMap<QString, StationPointer> mStations;

        // Initialization
        void init_ui();
        void init_children();

        // UI members
        QLineEdit *mUIFromLine, *mUIToLine;
        QMaemo5ValueButton *mUIDate, *mUITime;
        QMaemo5DatePickSelector *mUIDatePicker;
        QMaemo5TimePickSelector *mUITimePicker;
        QRadioButton *mUIUseTime, *mUITypeDeparture, *mUITypeArrival;

        // Temporary data
        QLineEdit *mTarget;
    };
}

#endif // CONNECTIONREQUESTWIDGET_H
