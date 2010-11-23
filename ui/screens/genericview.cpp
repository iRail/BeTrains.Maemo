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
    qDebug() << "+ " << __PRETTY_FUNCTION__;

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
    qDebug() << "~ " << __PRETTY_FUNCTION__;

}

void GenericView::showUI()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    this->show();
}


//
// UI events
//

void GenericView::_deleteLoader()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;
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
    qDebug() << "+ " << __PRETTY_FUNCTION__;

    QMaemo5InformationBox::information(this, tr("Error: ") % iError, QMaemo5InformationBox::DefaultTimeout);
}

void GenericView::startLoader()
{
    qDebug() << "+ " << __PRETTY_FUNCTION__;
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
