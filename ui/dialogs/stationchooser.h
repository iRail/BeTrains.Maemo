//
// Configuration
//

// Include guard
#ifndef STATIONCHOOSER_H
#define STATIONCHOOSER_H

// Includes
#include <QDialog>
#include <QList>
#include <QStandardItemModel>
#include <QListView>
#include <QHBoxLayout>
#include <QPushButton>
#include "api/station.h"
#include "ui/global.h"

namespace iRail
{
    class StationChooser : public QDialog
    {
    Q_OBJECT
    public:
        explicit StationChooser(const QList<StationPointer>* iStations, QWidget *parent);
        StationPointer getSelection();
        ~StationChooser();

    private:
        const QList<StationPointer>* mStations;
        StationPointer mStation;

        // UI member
        QListView *mView;
        QStandardItemModel *mModel;

        // Auxiliary
        void getStationsModel(QStandardItemModel *iModel);
    };
}

#endif // STATIONCHOOSER_H
