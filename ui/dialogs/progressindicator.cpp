//
// Configuration
//

// Includes
#include "progressindicator.h"
#include <QApplication>

// Namespaces
using namespace iRail;


//
// Construction and destruction
//

ProgressIndicator::ProgressIndicator(const QString &iTitle, QWidget *iParent) : QProgressDialog(iParent)
{
    // Dialog properties
    this->setModal(true);
    this->setWindowTitle(iTitle);
    this->show();
    this->setMinimum(0);
    setSubsteps(1);
}

void ProgressIndicator::setSubsteps(int iSteps)
{
    mSubsteps = iSteps;
    mCurrentSubstep = 0;
    this->setMaximum(mSubsteps*100);
}

void ProgressIndicator::next(const QString& iStepDescription)
{
    if (mCurrentSubstep+1 <= mSubsteps)
    {
        mCurrentSubstep++;
        this->setLabelText(iStepDescription);
        this->setValue(100*(mCurrentSubstep-1));
        QApplication::processEvents();  // This makes sure we get drawn (and not immediatly blocked
                                        // by some sort of I/O called after us).
    }
}

void ProgressIndicator::done()
{
    this->setValue(mSubsteps*100);
    this->hide();
}


//
// Public slots
//

void ProgressIndicator::setSubProgress(int progress)
{
    if (progress > 100)
        progress = 100;
    this->setValue(100*mCurrentSubstep + progress);
}

void ProgressIndicator::setSubProgress(int progress, int totalSteps)
{
    if (progress >= totalSteps && totalSteps > 0)
    {
        int smallprogress = 100*mCurrentSubstep + progress/totalSteps * 100;
        this->setValue(smallprogress);
    }
}
