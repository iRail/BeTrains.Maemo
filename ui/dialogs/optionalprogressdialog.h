//
// Configuration
//

// Include guard
#ifndef OPTIONALPROGRESSDIALOG_H
#define OPTIONALPROGRESSDIALOG_H

// Includes
#include <QWidget>
#include <QObject>
#include <QProgressDialog>
#include <QString>

namespace iRail
{
    class OptionalProgressDialog : public QObject
    {
    Q_OBJECT
        // Construction
    public:
        OptionalProgressDialog(QWidget* iParent);

        // UI slots
    public slots:
        void setEnabled(bool iEnabled);
        void setWindowTitle(const QString& iTitle);
        void show();
        void setValue(int& iValue);
        void setLabelText(const QString& iDetails);

        // UI members
    private:
        bool mEnabled;
        QWidget* mParent;
        QProgressDialog* mDialog;
        QString mTitle;
    };
}

#endif // OPTIONALPROGRESSDIALOG_H
