//
// Configuration
//

// Includes
#include "optionalprogressdialog.h"

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

OptionalProgressDialog::OptionalProgressDialog(QWidget* iParent) : mParent(iParent)
{
    mEnabled = false;
    mDialog = 0;
}


//
// UI Slots
//

void OptionalProgressDialog::setEnabled(bool iEnabled)
{
    mEnabled = iEnabled;

    if (!mEnabled)
    {
        if (mDialog != 0)
        {
            mDialog->hide();
            delete mDialog;
            mDialog = 0;
        }
    }
}

void OptionalProgressDialog::show()
{
    if (mEnabled && mDialog == 0)
    {
        mDialog = new QProgressDialog(mParent);
        if (mTitle.length() > 0)
            mDialog->setWindowTitle(mTitle);
        mDialog->setModal(true);
        mDialog->setMinimum(0);
        mDialog->setMaximum(100);
        mDialog->show();
    }
}

void OptionalProgressDialog::setWindowTitle(const QString& iTitle)
{
    mTitle = iTitle;
    if (mDialog != 0)
        mDialog->setWindowTitle(mTitle);
}

void OptionalProgressDialog::setValue(int& iValue)
{
    if (mDialog != 0)
        mDialog->setValue(iValue);
}

void OptionalProgressDialog::setLabelText(const QString& iDetails)
{
    if (mDialog != 0)
        mDialog->setLabelText(iDetails);
}
