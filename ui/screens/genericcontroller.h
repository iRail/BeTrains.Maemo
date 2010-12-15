//
// Configuration
//

// Include guard
#ifndef GENERICCONTROLLER_H
#define GENERICCONTROLLER_H

// Includes
#include <QDebug>
#include "cachedapi.h"
#include "genericview.h"


namespace iRail
{
    class GenericController : public QObject
    {
    Q_OBJECT
    public:
        explicit GenericController(CachedAPI* iAPI, QWidget *parent = 0);
        virtual void showView(GenericController* parent = 0);

        // Signals
    signals:
        void done();

        // Auxiliary
    protected:
        void setView(GenericView* view);
        GenericView* view() const;
        CachedAPI* api() const;

    private:
        // Member data
        CachedAPI *mAPI;
        GenericView *mView;
    };
}

#endif // GENERICCONTROLLER_H
