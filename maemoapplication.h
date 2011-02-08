//
// Configuration
//

// Include guard
#ifndef MAEMOAPPLICATION_H
#define MAEMOAPPLICATION_H

// Includes
#include "application.h"

namespace iRail
{
    class MaemoApplication : public Application
    {
    public:
        // Construction and destruction
        MaemoApplication(int & argc, char ** argv);
    };
}

#endif // MAEMOAPPLICATION_H
