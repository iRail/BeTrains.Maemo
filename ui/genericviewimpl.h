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
    // TODO: use the diamond when Qt supports this
    //class GenericViewImpl : virtual public GenericView
    class GenericViewImpl : public QObject
    {
    Q_OBJECT
    public:
        GenericViewImpl(GenericView*);
        ~GenericViewImpl();

        // Controller actions
    public slots:
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

        // Base member (pseudo diamond)
        GenericView* base;

        // Proxy object
        GenericViewImpl* proxy;
    };
}

#endif // GENERICVIEWIMPL_H
