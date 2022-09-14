TARGET = DualDepthPeelingApp
TEMPLATE = app

QT = core gui widgets

CONFIG += debug_and_release c++17 qtquickcompiler

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

win32-msvc {
    # MSVC compiler is out of heap space, even with /Zm option
    # TODO: decimate the dragon.obj to reduce its size
    PWD_WIN = $${PWD}
    DESTDIR_WIN = $${OUT_PWD}
    PWD_WIN ~= s,/,\\,g
    DESTDIR_WIN ~= s,/,\\,g

    copyfiles.commands = $$quote(cmd /c xcopy /S /I /Y $${PWD_WIN}\\Resources\\dragon.obj $${DESTDIR_WIN})

    QMAKE_EXTRA_TARGETS += copyfiles
    POST_TARGETDEPS += copyfiles
} else {
    RESOURCES += \
        Resources/Resources.qrc
}
