TARGET = Gui
TEMPLATE = lib
CONFIG += static debug_and_release c++17 qtquickcompiler

QT = core gui widgets
versionAtLeast(QT_VERSION, 6) {
    DEFINES += QT6
    QT += opengl openglwidgets
}

DEFINES += GL_SILENCE_DEPRECATION

INCLUDEPATH += \
    ../DataModel

HEADERS += \
    GLWidgets/Camera.h \
    GLWidgets/GLWidget.h \
    GLWidgets/MainWidget.h \
    GLWidgets/Scene.h \
    Renderers/AbstractRenderer.h \
    Renderers/Common/MultipleLightsRenderer.h \
    Renderers/MeshRenderer.h \
    Renderers/PathRenderer.h \
    Renderers/PlaneRenderer.h \
    Renderers/UnorderedTransparency/DualDepthPeelingRenderer.h \
    Renderers/UnorderedTransparency/TransparencyRenderer.h

SOURCES += \
    GLWidgets/Camera.cpp \
    GLWidgets/GLWidget.cpp \
    GLWidgets/MainWidget.cpp \
    GLWidgets/Scene.cpp \
    Renderers/AbstractRenderer.cpp \
    Renderers/Common/MultipleLightsRenderer.cpp \
    Renderers/MeshRenderer.cpp \
    Renderers/PathRenderer.cpp \
    Renderers/PlaneRenderer.cpp \
    Renderers/UnorderedTransparency/DualDepthPeelingRenderer.cpp \
    Renderers/UnorderedTransparency/TransparencyRenderer.cpp

build_pass:CONFIG(debug, debug|release) {
    DEFINES += \
        AUTO_SHADER \
        PROJECT_DIR=$$_PRO_FILE_PWD_
    HEADERS += Renderers/Debug/AutoShaderReloader.h
    SOURCES += Renderers/Debug/AutoShaderReloader.cpp
}

RESOURCES += \
    Shaders/ShaderResources.qrc
