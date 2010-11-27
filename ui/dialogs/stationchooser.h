//
// Configuration
//

// Include guard
#ifndef STATIONCHOOSER_H
#define STATIONCHOOSER_H

// Includes
#include <QDialog>
#include <QMap>
#include <QStandardItemModel>
#include <QListView>
#include <QHBoxLayout>
#include <QPushButton>
#include "api/station.h"
#include "ui/global.h"
#include "cachedapi.h"

namespace iRail
{
    class StationChooser : public QDialog
    {
    Q_OBJECT
    public:
        explicit StationChooser(const QMap<QString, StationPointer>& iStations, QWidget *iParent);
        StationPointer getSelection();
        ~StationChooser();

    private:
        const QMap<QString, StationPointer>& mStations;
        StationPointer mStation;

        // Initialization
        void init_ui();
        void init_children();

        // UI members
        QListView *mView;
        QStandardItemModel *mModel;

        // Auxiliary
        void populateModel();
    };
}

#endif // STATIONCHOOSER_H
