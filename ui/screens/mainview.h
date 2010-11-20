//
// Configuration
//

// Include guard
#ifndef MAINVIEW_H
#define MAINVIEW_H

// Inclused
#include <QObject>
#include <QScrollArea>
#include <QListView>
#include <QStandardItem>
#include <QModelIndex>
#include <QLabel>
#include <QtMaemo5/QMaemo5InformationBox>

namespace iRail
{
    class MainView : public QScrollArea
    {

    Q_OBJECT
    public:
        MainView(QWidget *iParent);
        ~MainView();

        // UI events
    private slots:
        void load_history(QModelIndex iIndex);

    private:
        // UI members
        QLabel *mViewDummy;
        QListView *mView;
        QStandardItemModel *mModel;
        QPushButton* mUIButtonSearch;

        // UI children
        OptionalProgressDialog* mChildProgressDialog;
        ConnectionRequestWidget* mChildConnectionRequest;
        ConnectionResultWidget* mChildConnectionResult;
        ConnectionDetailWidget* mChildConnectionDetail;
        LiveboardWidget* mChildLiveboard;

        // Initialization
        void init_ui();
        void init_children();

        // Auxiliary
        void populateModel();
    };
}

#endif // MAINVIEW_H
