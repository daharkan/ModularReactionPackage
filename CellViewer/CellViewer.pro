QT       += core gui serialport concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ExperimentRunner.cpp \
    OnlyCellSerialManager.cpp \
    main.cpp \
    ../experimentmanagerservice/src/common/Arc.cpp \
    ../experimentmanagerservice/src/common/Cell.cpp \
    ../experimentmanagerservice/src/common/CellTarget.cpp \
    ../experimentmanagerservice/src/common/CellVisuals.cpp \
    ../experimentmanagerservice/src/common/CellVisualsHistory.cpp \
    ../experimentmanagerservice/src/common/Experiment.cpp \
    ../experimentmanagerservice/src/common/FlowStatus.cpp \
    ../experimentmanagerservice/src/common/Profile.cpp \
    ../experimentmanagerservice/src/common/RPMArc.cpp \
    ../experimentmanagerservice/src/common/RedisDBManager.cpp \
    ../experimentmanagerservice/src/common/User.cpp \
    ../experimentmanagerservice/src/common/TempArc.cpp \
    uiCellGraph.cpp \
    uiCellWidget.cpp \
    uiMainWindow.cpp

HEADERS += \
    ../experimentmanagerservice/src/common/Cell.h \
    ../experimentmanagerservice/src/common/CellTarget.h \
    ../experimentmanagerservice/src/common/CellVisuals.h \
    ../experimentmanagerservice/src/common/CellVisualsHistory.h \
    ../experimentmanagerservice/src/common/Experiment.h \
    ../experimentmanagerservice/src/common/FlowStatus.h \
    ../experimentmanagerservice/src/common/Profile.h \
    ../experimentmanagerservice/src/common/RPMArc.h \
    ../experimentmanagerservice/src/common/RedisDBManager.h \
    ../experimentmanagerservice/src/common/User.h \
    ../experimentmanagerservice/src/common/TempArc.h \
    ExperimentRunner.h \
    OnlyCellSerialManager.h \
    uiCellGraph.h \
    uiCellWidget.h \
    uiMainWindow.h

FORMS += \
    uiCellGraph.ui \
    uiCellWidget.ui \
    uiMainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH += ../experimentmanagerservice/src/common/
INCLUDEPATH += $$(DEV_EXTERNALS_PATH)/qwt-6.2.0/src
DEPENDPATH +=  $$(DEV_EXTERNALS_PATH)/qwt-6.2.0/lib/
LIBS += -L$$(DEV_EXTERNALS_PATH)/qwt-6.2.0/lib/
LIBS += -ltacopie -lcpp_redis -lqwt -L/usr/local/lib
