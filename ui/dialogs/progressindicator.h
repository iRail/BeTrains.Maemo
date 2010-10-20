//
// Configuration
//

// Include guard
#ifndef PROGRESSINDICATOR_H
#define PROGRESSINDICATOR_H

// Includes
#include <QProgressDialog>

namespace iRail
{
    class ProgressIndicator : public QProgressDialog    // private inheritance doesn't work, as we need QObject publicly (and multiply inheriting QProgressDialog and QDialog doesn't work either)
    {
    Q_OBJECT
    public:
        ProgressIndicator(const QString& iTitle, QWidget *iParent);

        void setSubsteps(int iSubsteps);
        void next(const QString& iStepDescription);
        void done();

    public slots:
        void setSubProgress(int progress);
        void setSubProgress(int progress, int totalSteps);

    private:
        unsigned int mSubsteps, mCurrentSubstep;
    };
}

#endif // PROGRESSINDICATOR_H
