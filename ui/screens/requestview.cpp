//
// Configuration
//

// Includes
#include "requestview.h"
#include "ui/auxiliary/delegates/connectionrequestdelegate.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFont>
#include <QStringBuilder>
#include <QtMaemo5/QMaemo5InformationBox>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

RequestView::RequestView(QWidget* iParent) : QWidget(iParent)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    if (iParent != 0)
    {
        setWindowFlags(windowFlags() | Qt::Window);
        setAttribute(Qt::WA_Maemo5StackedWindow);
    }

    mUILayout = new QVBoxLayout(this);
    mUILayout->setMargin(0);

    mChildConnectionRequest = 0;

    this->hide();
}

RequestView::~RequestView()
{
    qDebug() << "~ " << __PRETTY_FUNCTION__;

}

void RequestView::showUI()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    _showConnectionRequest();
    this->show();
}

void RequestView::showUI(ConnectionRequestPointer iInitialRequest)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    _showConnectionRequest();
    mChildConnectionRequest->load(iInitialRequest);
    this->show();
}


//
// UI events
//

void RequestView::_showConnectionRequest()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    emit downloadStations();
}

void RequestView::_showConnectionRequest(const QMap<QString, StationPointer>& iStations)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    if (mChildConnectionRequest == 0)
    {
        // Connection request widget
        mChildConnectionRequest = new ConnectionRequestWidget(iStations, this);
        mUILayout->addWidget(mChildConnectionRequest);
        connect(mChildConnectionRequest, SIGNAL(finished(ConnectionRequestPointer)), this, SIGNAL(launchConnection(ConnectionRequestPointer)));
    }

    mChildConnectionRequest->clear();
    mChildConnectionRequest->show();
}


//
// Controller actions
//

void RequestView::setStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    _showConnectionRequest(*iStations);
    delete iStations;
}

void RequestView::showError(const QString &iError)
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    QMaemo5InformationBox::information(this, tr("Error: ") % iError, QMaemo5InformationBox::DefaultTimeout);
}
