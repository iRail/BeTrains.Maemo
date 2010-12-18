//
// Configuration
//

// Include guard
#ifndef GENERICVIEWIMPL_H
#define GENERICVIEWIMPL_H

// Inclused
#include <QDebug>
#include "ui/auxiliary/loaderwidget.h"
#include <QString>
#include <QMainWindow>
#include <QGraphicsView>
#include "ui/genericview.h"
#include "genericviewimpl.h"

namespace iRail
{
    class GenericViewImpl : virtual public GenericView
    {
    Q_OBJECT
    public:
        GenericViewImpl();
        ~GenericViewImpl();

        // Controller actions
    public slots:
        /*!
          The reset() function resets the view to pristing state, removing all
          previously entered or changed user data. It is called when re-using a
          already instantiated view, so it should not remove previously fetched
          data as load() won't be called again.
          */
        virtual void reset() = 0;
        /*!
          The load() function is used to fetch data required to display the
          widget. It is called when the view is active already, so the view
          should handle this by displaying things appropriately without this
          initial data (eg. a loader, or disabled buttons).
          TODO: what if only a parameterised load()?
          */
        //virtual void load();
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

#endif // GENERICVIEWIMPL_H
