TEMPLATE = lib
VERSION = 1.0
TARGET = ddwarf-qtfiles

QT += qml

CONFIG += c++11
INCLUDEPATH +=

macx: QMAKE_SONAME_PREFIX = @executable_path/../Frameworks

isEmpty(PREFIX) {
    PREFIX = /usr/local/
}

unix:!android {
    target.path = $$(PREFIX)

    INSTALLS += target
}

HEADERS += \
    FileProviderList.h \
    FileProvider.h


SOURCES += \
    FileProviderList.cpp \
    FileProvider.cpp


OTHER_FILES += \
    LICENSE \
    README.md

include(../QtPvsStudio/pvs-studio.pri)
