include(../plugins-global.pri)

HEADERS     = src/*.h
SOURCES     = src/*.cpp
TRANSLATIONS = operator_fr.ts
TRANSLATIONS += operator_nl.ts
TRANSLATIONS += operator_de.ts
TRANSLATIONS += operator_uk.ts

TARGET      = $$qtLibraryTarget(operatorplugin)

RESOURCES = res.qrc
