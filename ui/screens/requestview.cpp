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
    qDebug() << "+ " << Q_FUNC_INFO;

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
    qDebug() << "~ " << Q_FUNC_INFO;

}

void RequestView::showUI()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    _showConnectionRequest();
    this->show();
}

void RequestView::showUI(ConnectionRequestPointer iInitialRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    _showConnectionRequest();
    mChildConnectionRequest->load(iInitialRequest);
    this->show();
}


//
// UI events
//

void RequestView::_showConnectionRequest()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    emit downloadStations();
}

void RequestView::_showConnectionRequest(const QMap<QString, StationPointer>& iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;

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
    qDebug() << "+ " << Q_FUNC_INFO;

    _showConnectionRequest(*iStations);
    delete iStations;
}

void RequestView::showError(const QString &iError)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    QMaemo5InformationBox::information(this, tr("Error: ") % iError, QMaemo5InformationBox::DefaultTimeout);
}
