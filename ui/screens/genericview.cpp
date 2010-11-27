//
// Configuration
//

// Includes
#include "genericview.h"
#include <QStringBuilder>
#include <QtMaemo5/QMaemo5InformationBox>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

GenericView::GenericView(QWidget* iParent) : QWidget(iParent)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    setWindowFlags(windowFlags() | Qt::Window);
    setAttribute(Qt::WA_Maemo5StackedWindow);

    mLoader = 0;
    mView = 0;

    this->hide();
}

GenericView::~GenericView()
{
    qDebug() << "~ " << Q_FUNC_INFO;

}


//
// UI events
//

void GenericView::_deleteLoader()
{
    qDebug() << "+ " << Q_FUNC_INFO;
    Q_ASSERT(mLoader);
    Q_ASSERT(mView);

    delete mLoader;
    mLoader = 0;
    delete mView;
    mView = 0;

    this->setEnabled(true);
}


//
// Controller actions
//

void GenericView::showError(const QString &iError)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    stopLoader();
    QMaemo5InformationBox::information(this, tr("Error: ") % iError, QMaemo5InformationBox::DefaultTimeout);
}

void GenericView::startLoader()
{
    qDebug() << "+ " << Q_FUNC_INFO;
    Q_ASSERT(!mLoader);

    mLoader = new LoaderWidget();
    connect(mLoader, SIGNAL(finished()), this, SLOT(_deleteLoader()));

    mLoader->setZValue(1.0);

    QGraphicsScene* tScene = new QGraphicsScene();
    mView = new QGraphicsView(tScene, this);
    mView->setFixedSize(800, 420);
    mView->setStyleSheet("background: transparent; border: none");
    tScene->addItem(mLoader);
    mView->show();

    mLoader->startEntryAnimation();
}

void GenericView::stopLoader()
{
    if (mLoader != 0)
        mLoader->startExitAnimation();
}
