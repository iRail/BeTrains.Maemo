//
// Configuration
//

// Includes
#include "connectionview.h"
#include "ui/auxiliary/delegates/connectionrequestdelegate.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFont>
#include <QStringBuilder>
#include <QtMaemo5/QMaemo5InformationBox>
#include "ui/global.h"
#include "ui/auxiliary/delegates/connectiondelegate.h"

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

ConnectionView::ConnectionView(QWidget* iParent) : QWidget(iParent)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    if (iParent != 0)
    {
        setWindowFlags(windowFlags() | Qt::Window);
        setAttribute(Qt::WA_Maemo5StackedWindow);
    }    

    // Initialisation
    init_ui();
    init_children();

    this->hide();
}

ConnectionView::~ConnectionView()
{
    qDebug() << "~ " << Q_FUNC_INFO;

}

void ConnectionView::showUI(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    _showConnectionResult(iConnectionRequest);
    this->show();
}

void ConnectionView::load(const QList<ConnectionPointer>& iConnections)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    populateModel(iConnections);
}


//
// UI events
//

void ConnectionView::_showConnectionResult(ConnectionRequestPointer iConnectionRequest)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    emit downloadConnections(iConnectionRequest);
}

void ConnectionView::_showConnectionResult(const QMap<QString, StationPointer>& iStations, const QList<ConnectionPointer>& iConnections)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mStations = iStations;  // TODO: also in load?

    // Show the results
    load(iConnections);
}

void ConnectionView::activated(QModelIndex iIndex)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    ConnectionPointer tConnection = iIndex.data(ConnectionRole).value<ConnectionPointer>();
    emit launchVehicle(tConnection);
}


//
// Controller actions
//

void ConnectionView::setStations(QMap<QString, StationPointer>* iStations)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    _showConnectionResult(*iStations, *tConnections);
    delete iStations;
    delete tConnections;
}

void ConnectionView::setConnections(QList<ConnectionPointer>* iConnections)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    tConnections = iConnections;
    emit downloadStations();
}

void ConnectionView::showError(const QString &iError)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    QMaemo5InformationBox::information(this, tr("Error: ") % iError, QMaemo5InformationBox::DefaultTimeout);
}//
// Initialisation
//


void ConnectionView::init_ui()
{
    qDebug() << "+ " << Q_FUNC_INFO;

    // Window settings
    this->setWindowTitle(QString(tr("Connections")));

    // Main layout
    QVBoxLayout *mUILayout = new QVBoxLayout(this);
    mUILayout->setAlignment(Qt::AlignTop);

    // Populate the history list model
    mModel = new QStandardItemModel(0, 1);

    // Create the history listview
    QListView *tView = new QListView();
    tView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tView->setModel(mModel);
    tView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tView->setSelectionMode(QAbstractItemView::SingleSelection);
    tView->setItemDelegate(new ConnectionDelegate(mStations));
    connect(tView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(activated(QModelIndex)));
    mUILayout->addWidget(tView);
}

void ConnectionView::init_children()
{
    qDebug() << "+ " << Q_FUNC_INFO;

}

//
// Auxiliary
//

void ConnectionView::populateModel(const QList<ConnectionPointer>& iConnections)
{
    qDebug() << "+ " << Q_FUNC_INFO;

    mModel->clear();
    if (iConnections.size() > 0)
    {
        for (int i = 0; i < iConnections.size(); i++)
        {
            ConnectionPointer tConnection = iConnections.at(i);
            QStandardItem *tItem = new QStandardItem();
            tItem->setData(QVariant::fromValue(tConnection), ConnectionRole);
            mModel->appendRow(tItem);
        }
    }
    else
    {
        QStandardItem *tDummy = new QStandardItem(tr("No connections found."));
        tDummy->setEditable(false);
        tDummy->setSelectable(false);
        mModel->appendRow(tDummy);
    }
}
