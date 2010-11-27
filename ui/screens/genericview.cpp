//
// Configuration
//

// Includes
#include "genericview.h"
#include <QStringBuilder>
#include <QtMaemo5/QMaemo5InformationBox>
#include <QGraphicsView>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

GenericView::GenericView(QWidget* iParent) : QWidget(iParent)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    if (iParent != 0)
    {
        setWindowFlags(windowFlags() | Qt::Window);
        setAttribute(Qt::WA_Maemo5StackedWindow);
    }

    mLoader = 0;
    this->hide();
}

GenericView::~GenericView()
{
    qDebug() << "~ " << Q_FUNC_INFO;

}

void GenericView::showUI()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    this->show();
}


//
// UI events
//

void GenericView::_deleteLoader()
{
    qDebug() << "+ " << Q_FUNC_INFO;
    Q_ASSERT(mLoader);

    delete mLoader;
    mLoader = 0;
    this->setEnabled(true);
}


//
// Controller actions
//

void GenericView::showError(const QString &iError)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    QMaemo5InformationBox::information(this, tr("Error: ") % iError, QMaemo5InformationBox::DefaultTimeout);
}

void GenericView::startLoader()
{
    qDebug() << "+ " << Q_FUNC_INFO;
    Q_ASSERT(!mLoader);

    mLoader = new LoaderWidget();
    connect(mLoader, SIGNAL(finished()), this, SLOT(_deleteLoader()));

    mLoader->setZValue(1.0);
    QGraphicsRectItem *bg = mLoader->getBackground();
    bg->setOpacity(0.6);

    QGraphicsScene* tScene = new QGraphicsScene();
    QGraphicsView* tView = new QGraphicsView(tScene, this);
    tView->setFixedSize(800, 420);
    tView->setStyleSheet("background: transparent; border: none");
    tScene->addItem(mLoader);
    tView->show();

    mLoader->startEntryAnimation();
}

void GenericView::stopLoader()
{
    if (mLoader != 0)
        mLoader->startExitAnimation();
}
