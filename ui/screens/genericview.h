//
// Configuration
//

// Include guard
#ifndef GENERICVIEW_H
#define GENERICVIEW_H

// Inclused
#include <QDebug>
#include "ui/auxiliary/loaderwidget.h"
#include <QString>
#include <QMainWindow>
#include <QGraphicsView>

namespace iRail
{
    class GenericView : public QMainWindow
    {
    Q_OBJECT
    public:
        GenericView(QWidget *iParent);
        ~GenericView();

        // Controller actions
    public slots:
        void showError(const QString& iError);
        void startLoader();
        void stopLoader();
        void showProgress();

        // UI events
    private slots:
        void _deleteLoader();

    private:
        // UI members
        LoaderWidget* mLoader;
        QGraphicsView* mView;
    };
}

#endif // GENERICVIEW_H
