TARGET = dronehunter

LIBS += ../Brixtuff/Urho3D/lib/libUrho3D.a \
    -lpthread \
    -ldl \
    -lGL

DEFINES += URHO3D_COMPILE_QT

QMAKE_CXXFLAGS += -std=c++1y

INCLUDEPATH += \
    ../Brixtuff/Urho3D/include \
    ../Brixtuff/Urho3D/include/Urho3D/ThirdParty \

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    luckey.h \
    sceneobject.h \
    mastercontrol.h \
    inputmaster.h \
    spawnmaster.h \
    player.h \
    controllable.h \
    rock.h \
    character.h \
    dhcam.h \
    building.h \
    drone.h \
    predator.h \
    reaper.h \
    tr3b.h

SOURCES += \
    luckey.cpp \
    sceneobject.cpp \
    mastercontrol.cpp \
    inputmaster.cpp \
    spawnmaster.cpp \
    player.cpp \
    controllable.cpp \
    rock.cpp \
    character.cpp \
    dhcam.cpp \
    building.cpp \
    drone.cpp \
    predator.cpp \
    reaper.cpp \
    tr3b.cpp
