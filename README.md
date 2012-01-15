# About

BeTrains is a set of mobile applications providing easy access to public transport data. This specific project contains the BeTrains application for the Maemo 5 platform, found on the Nokia N900 mobile computer. The application is written in C++, using the Qt library to make it a bit more fun. The project has been split in three distinct parts, with the one you're looking at now providing the user interface as well as some platform dependant quirks (e.g. the build system). For the application logic (the controller, in MVC terminology), have a look at the *BeTrains.Qt* project. Another component responsible for the actual data (cfr. the MVC model) is to be found in the *libqtrail* project.

# iRail

Not only does BeTrains use the iRail API to get the necessary data, it is also strongly related with it. For more information, check the iRail wiki: http://project.irail.be/wiki/BeTrains

# Installation

Have a look at the _INSTALL_ file.

#Translations

The application can be easily be translated into other languages. Here's how we handle those translations.

## Files and Folders

### BeTrains.Maemo.pro

The main project file takes care of generating `.qm` files from the source `.ts` files. It is mostly automated.

### i18n.qrc

This resource file must be updated whenever a new translation is added. Please note that files must end with `.qm`, not `.ts`.

	<qresource>
	    <file>i18n/de.qm</file>
	    <file>i18n/nl.qm</file>
	</qresource>

### i18n/

This directory contains all translations. Filenames must adhere to this format:

    xx_YY.ts or xx.ts

#### Source file

`i18n/en.ts` is a treated in a special way. It is used as the source for all other translations. Whenever a string in the code is change this file must be updated to reflect those changes. Usually, this can be accomplished by running `lupdate`, after which the file can get pushed to Transifex (see below).

## Syncing with Transifex

We are using http://transifex.net as a frontend for translating the client, use the "transifex client":http://help.transifex.net/features/client/index.html to fetch new translations from transifex.

### Pushing new source strings

1. `lupdate -no-obsolete -pro BeTrains.Maemo.pro -ts i18n/en.ts`
2. Fill in plural forms (use Qt Linquist, or manually edit `numerus="yes"` entries)
3. `tx push -s`

### Fetching new translations

1. `tx pull -a`
2. Update `i18n.qrc`
3. `git add` new translations from `i18n/`
