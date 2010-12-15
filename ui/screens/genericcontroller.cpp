//
// Configuration
//

// Includes
#include "genericcontroller.h"

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

GenericController::GenericController(CachedAPI* iAPI, QWidget* iParent) : mAPI(iAPI)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mView = 0;
}

void GenericController::showView(GenericController* parent)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    if (parent != 0)
    {
        view()->setParent(parent->view());
        view()->setWindowFlags(view()->windowFlags() | Qt::Window);
    }

    view()->show();
}


//
// Auxiliary
//


void GenericController::setView(GenericView* view)
{
    mView = view;
}

GenericView* GenericController::view() const
{
    return mView;
}

CachedAPI* GenericController::api() const
{
    return mAPI;
}
