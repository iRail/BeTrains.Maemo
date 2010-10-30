//
// Configuration
//

// Include guard
#ifndef GLOBAL_H
#define GLOBAL_H

// Includes
#include <QList>

namespace iRail
{
    enum iRailRoles
    {
        ConnectionRequestRole = Qt::UserRole + 1,
        StationRole,
        ConnectionRole
    };
}

#endif // CLI_H
