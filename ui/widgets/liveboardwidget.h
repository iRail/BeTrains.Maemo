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
        void load(StationPointer iStation);

        // Public slots
    public slots:
        void clear();
        void clear_id();

        // UI Events
    private slots:
        void do_search();
        void do_stations();
        void do_detail(QModelIndex iIndex);

    signals:
        void finished(Liveboard::Departure iDeparture);

    private:
        // Member data
        const QMap<QString, StationPointer> mStations;
        LiveboardPointer mLiveboard;
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
        void populateModel();
    };
}

#endif // LIVEBOARDWIDGET_H
