HEADERS += bsenseprobe.h \
    bsensegui.h \
    update.h \
    bsense.h

SOURCES += main.cpp \
    bsenseprobe.cpp \
    bsensegui.cpp \
    update.cpp

RESOURCES += bsense.qrc

FORMS += \
    postcodedialog.ui \
    emaildialog.ui

OTHER_FILES += \
    images/bsense-green.png \
    images/bsense-green.ico \
    images/bsense-green.icns \
    bsense.rc

CONFIG += embed_manifest_exe

ICON = images/bsense-green.icns

RC_FILE = bsense.rc

# QT modules to 'turn on':
QT += xml \
    svg \
    network

# Additional parameters for Linux only:
unix:!macx {
    # !!IMPORTANT!! !!IMPORTANT!! !!IMPORTANT!!
    # To build the project as a shared binary, comment out the following line:
    QTPLUGIN += qsvg                # imports statically the qsvg plugin
}

# Additional parameters for Mac only:
macx {
    LIBS+= -desetRunningad_strip              # "dead code stripping" makes binaries much smaller
    QMAKE_INFO_PLIST = Info.plist   # copies the attached info.plist inside bsense.app

    # !!IMPORTANT!! !!IMPORTANT!! !!IMPORTANT!!
    # To build the project as a shared binary, comment out the following line:
    QTPLUGIN += qsvg                # imports statically the qsvg plugin
}

# Additional parameters for Windows only:
win32 {
}

# Unless we're in debug mode, turn off the console output:
!debug {
    DEFINES+=QT_NO_DEBUG_OUTPUT
}
DEFINES+=QT_NO_DEBUG_OUTPUT

# Install (still needed?):
target.path = .
sources.files = $$SOURCES \
    $$HEADERS \
    $$RESOURCES \
    $$FORMS \
    bsense.pro \
    resources \
    images
sources.path = .
INSTALLS += target \
    sources
