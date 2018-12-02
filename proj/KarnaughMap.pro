#-------------------------------------------------
#
# Project created by QtCreator 2017-08-10T02:54:15
#
#-------------------------------------------------

QT       += core gui widgets

TEMPLATE = app

#---------------------------------------------------------------------------------------------------
# Versión
#---------------------------------------------------------------------------------------------------
DEFINES += MAJOR_VERSION=1
DEFINES += MINOR_VERSION=0

#---------------------------------------------------------------------------------------------------
# Target
#---------------------------------------------------------------------------------------------------
CONFIG(debug, debug|release) {
TARGET = KarnaughMap_d
} else {
TARGET = KarnaughMap
}

#---------------------------------------------------------------------------------------------------
# Directorio de destino
#---------------------------------------------------------------------------------------------------
DESTDIR = ../bin

#---------------------------------------------------------------------------------------------------
# Archivos de código
#---------------------------------------------------------------------------------------------------
SOURCES += \
			src/main.cpp\
			src/MainWindow.cpp \
			src/TruthTableWidget.cpp \
			src/KarnaughMap.cpp \
			src/KarnaughMapWidget.cpp \
			src/TruthTableModel.cpp \
			src/BooleanFunction.cpp \
			src/KarnaughMapModel.cpp \
			src/KarnaughMapDelegate.cpp \
			src/SolutionWidget.cpp \
    src/SolutionItemDelegate.cpp \
    src/Languages.cpp \
    src/AboutDialog.cpp \
    src/gray.c \
    src/ConfigForm.cpp

HEADERS  += \
			src/MainWindow.h \
			src/TruthTableWidget.h \
			src/KarnaughMap.h \
			src/KarnaughMapWidget.h \
			src/TruthTableModel.h \
			src/BooleanFunction.h \
			src/KarnaughMapModel.h \
			src/KarnaughMapDelegate.h \
			src/SolutionWidget.h \
    src/SolutionItemDelegate.h \
    src/Languages.h \
    src/AboutDialog.h \
    src/gray.h \
    src/ConfigForm.h

FORMS    += \
			src/MainWindow.ui \
    src/AboutDialog.ui \
    src/ConfigForm.ui


#---------------------------------------------------------------------------------------------------
# Archivos de recursos
#---------------------------------------------------------------------------------------------------

RESOURCES += res/resource.qrc

win32:RC_FILE = res/resource_win32.rc

#---------------------------------------------------------------------------------------------------
# Archivos de traducción
#---------------------------------------------------------------------------------------------------

TRANSLATIONS = \
                translate\kmap_en.ts \
                translate\kmap_hr.ts \
                translate\kmap_es.ts \
                translate\kmap_it.ts \
                translate\kmap_pt.ts \
                translate\kmap_de.ts
