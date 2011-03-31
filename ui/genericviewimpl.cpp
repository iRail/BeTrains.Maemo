//
// Configuration
//

// Includes
#include "genericviewimpl.h"
#include <QStringBuilder>
#include <QtMaemo5/QMaemo5InformationBox>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

GenericViewImpl::GenericViewImpl(GenericView* iBase) : base(iBase)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Center widget
    base->setCentralWidget(new QWidget());
    base->setAttribute(Qt::WA_Maemo5StackedWindow);

    // Initialize member
    mLoaderRefcount = 0;
    mLoader = 0;
    mView = 0;

    base->hide();
}

GenericViewImpl::~GenericViewImpl()
{
    qDebug() << "~ " << Q_FUNC_INFO;
}


//
// UI events
//

void GenericViewImpl::_deleteLoader()
{
    qDebug() << "+ " << Q_FUNC_INFO;
    Q_ASSERT(mLoader);
    Q_ASSERT(mView);

    delete mLoader;
    mLoader = 0;
    delete mView;
    mView = 0;

    base->setEnabled(true);
}


//
// Controller actions
//

void GenericViewImpl::showError(const QString &iError)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    stopLoader();
    QMaemo5InformationBox::information(base, tr("Error: ") % iError, QMaemo5InformationBox::DefaultTimeout);
}

void GenericViewImpl::startLoader()
{
    qDebug() << "+ " << Q_FUNC_INFO << "refcount=" % QString::number(mLoaderRefcount+1);

    mLoaderRefcount++;
}

void GenericViewImpl::stopLoader()
{
    qDebug() << "+ " << Q_FUNC_INFO << "refcount=" % QString::number(mLoaderRefcount-1);

    if (--mLoaderRefcount == 0)
    {
        if (mLoader != 0 && mView != 0)
            mLoader->startExitAnimation();
    }
}

void GenericViewImpl::showProgress()
{
    // TODO: what if two loaders get scheduled really quickly after one other?
    Q_ASSERT(mLoader == 0);

    mLoader = new LoaderWidget();
    connect(mLoader, SIGNAL(finished()), this, SLOT(_deleteLoader()));

    mLoader->setZValue(1.0);

    QGraphicsScene* tScene = new QGraphicsScene();
    mView = new QGraphicsView(tScene, base->centralWidget());
    mView->setFixedSize(800, 420);
    mView->setStyleSheet("background: transparent; border: none");
    tScene->addItem(mLoader);
    mView->show();

    mLoader->startEntryAnimation();
}
