# About

BeTrains is a set of mobile applications providing easy access to public transport data. This specific project contains the BeTrains application for the Maemo 5 platform, found on the Nokia N900 mobile computer. The application is written in C++, using the Qt library to make it a bit more fun. The project has been split in three distinct parts, with the one you're looking at now providing the user interface as well as some platform dependant quirks (e.g. the build system). For the application logic (the controller, in MVC terminology), have a look at the *BeTrains.Qt* project. Another component responsible for the actual data (cfr. the MVC model) is to be found in the *libqtrail* project.

# iRail

Not only does BeTrains use the iRail API to get the necessary data, it is also strongly related with it. For more information, check the iRail wiki: http://project.irail.be/wiki/BeTrains

# Installation

Have a look at the _INSTALL_ file.

