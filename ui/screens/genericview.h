//
// Configuration
//

// Include guard
#ifndef GENERICVIEW_H
#define GENERICVIEW_H

// Inclused
#include <QDebug>
#include <QString>
#include <QWidget>
#include "ui/auxiliary/loaderwidget.h"

namespace iRail
{
    class GenericView : public QWidget
    {
    Q_OBJECT
    public:
        GenericView(QWidget *iParent);
        ~GenericView();
        virtual void showUI();

        // Controller actions
    public slots:
        void showError(const QString& iError);
        void startLoader();
        void stopLoader();

        // UI events
    private slots:
        void _deleteLoader();

    private:
        // UI members
        LoaderWidget* mLoader;
    };
}

#endif // GENERICVIEW_H
