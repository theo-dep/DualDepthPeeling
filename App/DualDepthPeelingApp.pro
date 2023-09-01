TARGET = DualDepthPeelingApp
TEMPLATE = app

QT += core gui widgets
versionAtLeast(QT_VERSION, 6) {
    QT += opengl openglwidgets
}

CONFIG += debug_and_release c++17 qtquickcompiler big_resources

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    ../DataModel \
    ../Gui

SOURCES += \
    main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

build_pass:CONFIG(debug, debug|release):CONFIGURATION = debug
else:build_pass:CONFIG(release, debug|release):CONFIGURATION = release

LIBS += \
    -L$$OUT_PWD/../Gui -L$$OUT_PWD/../Gui/$${CONFIGURATION} -lGui \
    -L$$OUT_PWD/../DataModel -L$$OUT_PWD/../DataModel/$${CONFIGURATION} -lDataModel

RESOURCES += \
    Resources/Resources.qrc
