//
// Configuration
//

// Include guard
#ifndef GLOBAL_H
#define GLOBAL_H

// Includes
#include <QWidget>

namespace iRail
{
    enum iRailRoles
    {
        ConnectionRequestRole = Qt::UserRole + 1,
        StationRole,
        ConnectionPOIRole,
        ConnectionRole,
        VehicleRole,
        VehicleStopRole
    };
}

#endif // CLI_H
