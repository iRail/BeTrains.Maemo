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
#include "optionalprogressdialog.h"
#include "cachedapi.h"

namespace iRail
{
    class StationChooser : public QDialog
    {
    Q_OBJECT
    public:
        explicit StationChooser(CachedAPI* iAPI, QWidget *iParent);
        StationPointer getSelection();
        ~StationChooser();

        // Slots
    public slots:
        void gotStations(QList<StationPointer>* iStations);

    private:
        CachedAPI* mAPI;
        const QList<StationPointer>* mStations;
        StationPointer mStation;

        // Initialization
        void init_ui();
        void init_children();

        // UI members
        QWidget* mParent;
        QListView *mView;
        QStandardItemModel *mModel;

        // UI children
        OptionalProgressDialog* mChildProgressDialog;

        // Auxiliary
        void populateModel(const QList<StationPointer>* iStations);
    };
}

#endif // STATIONCHOOSER_H
