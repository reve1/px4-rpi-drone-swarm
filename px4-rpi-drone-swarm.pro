QT -= gui
QT += bluetooth
QT += serialport
QT += network
QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        bluetooth/bluetoothdiscovery.cpp \
        bluetooth/chat.cpp \
        bluetooth/chatclient.cpp \
        bluetooth/chatserver.cpp \
        lan/tcpserver.cpp \
        main.cpp \
        model/model.cpp \
        streams/filewrite.cpp \
        streams/timermessage.cpp \
        streams/timerrssi.cpp \
        #vehicle/vehicle.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    bluetooth/bluetoothdiscovery.h \
    bluetooth/chat.h \
    bluetooth/chatclient.h \
    bluetooth/chatserver.h \
    lan/tcpserver.h \
    mavsdk/include/mavsdk/connection_result.h \
    mavsdk/include/mavsdk/mavsdk.h \
    mavsdk/include/mavsdk/plugin_base.h \
    mavsdk/include/mavsdk/plugins/action/action.h \
    mavsdk/include/mavsdk/plugins/calibration/calibration.h \
    mavsdk/include/mavsdk/plugins/camera/camera.h \
    mavsdk/include/mavsdk/plugins/follow_me/follow_me.h \
    mavsdk/include/mavsdk/plugins/geofence/geofence.h \
    mavsdk/include/mavsdk/plugins/gimbal/gimbal.h \
    mavsdk/include/mavsdk/plugins/info/info.h \
    mavsdk/include/mavsdk/plugins/log_files/log_files.h \
    mavsdk/include/mavsdk/plugins/mission/mission.h \
    mavsdk/include/mavsdk/plugins/mission/mission_item.h \
    mavsdk/include/mavsdk/plugins/mission_raw/mission_raw.h \
    mavsdk/include/mavsdk/plugins/offboard/offboard.h \
    mavsdk/include/mavsdk/plugins/param/param.h \
    mavsdk/include/mavsdk/plugins/telemetry/telemetry.h \
    mavsdk/include/mavsdk/system.h \
    model/model.h \
    streams/filewrite.h \
    streams/timermessage.h \
    streams/timerrssi.h \
    #vehicle/vehicle.h


unix:!android: LIBS += -L$$PWD/mavsdk/lib/ -lmavsdk
unix:!android: LIBS += -L$$PWD/mavsdk/lib/ -lmavsdk_action
unix:!android: LIBS += -L$$PWD/mavsdk/lib/ -lmavsdk_telemetry
unix:!android: LIBS += -L$$PWD/mavsdk/lib/ -lmavsdk_offboard

unix:!android: INCLUDEPATH += $$PWD/mavsdk/include
unix:!android: DEPENDPATH += $$PWD/mavsdk/include
