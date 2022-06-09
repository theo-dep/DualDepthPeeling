TARGET = DataModel
TEMPLATE = lib
CONFIG += static debug_and_release c++17

QT = core

HEADERS += \
    Geom/Plane.h \
    Geom/Point.h \
    Geom/Vec4.h \
    Geom/Vector.h \
    Mesh/MeshModel.h

SOURCES += \
    Geom/Plane.cpp \
    Geom/Point.cpp \
    Geom/Vec4.cpp \
    Geom/Vector.cpp \
    Mesh/MeshModel.cpp

OTHER_FILES += \
    Geom/Plane.inl.cpp \
    Geom/Point.inl.cpp \
    Geom/Vec4.inl.cpp \
    Geom/Vector.inl.cpp

DEFINES += GL_SILENCE_DEPRECATION
