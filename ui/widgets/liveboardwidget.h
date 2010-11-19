//
// Configuration
//

// Include guard
#ifndef LIVEBOARDWIDGET_H
#define LIVEBOARDWIDGET_H

// Includes
#include <QWidget>
#include <QMap>
#include <QLabel>
#include <QListView>
#include <QLineEdit>
#include <QStandardItemModel>
#include "api/station.h"
#include "api/liveboard.h"

namespace iRail
{
    class LiveboardWidget : public QWidget
    {
    Q_OBJECT
    public:
        explicit LiveboardWidget(const QMap<QString, StationPointer>& iStations, QWidget *iParent);
        ~LiveboardWidget();
        void load(LiveboardPointer iLiveboard);

        // Public slots
    public slots:
        void clear();

        // UI Events
    private slots:
        void do_search();
        void do_stations();
        void do_detail(QModelIndex iIndex);

    signals:
        void request(QString iStationId);
        void finished(Liveboard::Departure iLiveboardDeparture);

    private:
        // Member data
        const QMap<QString, StationPointer> mStations;
        QString tStationId;

        // UI Members
        QLineEdit *mUIStationEdit;
        QLabel *mViewDummy;
        QListView *mView;
        QStandardItemModel *mModel;

        // Initialization
        void init_ui();
        void init_children();

        // Auxiliary
        void populateModel(const QList<Liveboard::Departure>& iDepartures);
    };
}

#endif // LIVEBOARDWIDGET_H
