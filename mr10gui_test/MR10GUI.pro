QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
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
    ../experimentmanagerservice/src/common/TempArc.cpp \
    ../experimentmanagerservice/src/common/User.cpp \
    main.cpp \
    uiMainWindow.cpp

HEADERS += \
    ../experimentmanagerservice/src/common/Arc.h \
    ../experimentmanagerservice/src/common/Cell.h \
    ../experimentmanagerservice/src/common/CellTarget.h \
    ../experimentmanagerservice/src/common/CellVisuals.h \
    ../experimentmanagerservice/src/common/CellVisualsHistory.h \
    ../experimentmanagerservice/src/common/Experiment.h \
    ../experimentmanagerservice/src/common/Profile.h \
    ../experimentmanagerservice/src/common/RPMArc.h \
    ../experimentmanagerservice/src/common/RedisDBManager.h \
    ../experimentmanagerservice/src/common/TempArc.h \
    ../experimentmanagerservice/src/common/User.h \
    ../experimentmanagerservice/src/common/rapidjson/allocators.h \
    ../experimentmanagerservice/src/common/rapidjson/cursorstreamwrapper.h \
    ../experimentmanagerservice/src/common/rapidjson/document.h \
    ../experimentmanagerservice/src/common/rapidjson/encodedstream.h \
    ../experimentmanagerservice/src/common/rapidjson/encodings.h \
    ../experimentmanagerservice/src/common/rapidjson/error/en.h \
    ../experimentmanagerservice/src/common/rapidjson/error/error.h \
    ../experimentmanagerservice/src/common/rapidjson/filereadstream.h \
    ../experimentmanagerservice/src/common/rapidjson/filewritestream.h \
    ../experimentmanagerservice/src/common/FlowStatus.h \
    ../experimentmanagerservice/src/common/rapidjson/fwd.h \
    ../experimentmanagerservice/src/common/rapidjson/internal/biginteger.h \
    ../experimentmanagerservice/src/common/rapidjson/internal/clzll.h \
    ../experimentmanagerservice/src/common/rapidjson/internal/diyfp.h \
    ../experimentmanagerservice/src/common/rapidjson/internal/dtoa.h \
    ../experimentmanagerservice/src/common/rapidjson/internal/ieee754.h \
    ../experimentmanagerservice/src/common/rapidjson/internal/itoa.h \
    ../experimentmanagerservice/src/common/rapidjson/internal/meta.h \
    ../experimentmanagerservice/src/common/rapidjson/internal/pow10.h \
    ../experimentmanagerservice/src/common/rapidjson/internal/regex.h \
    ../experimentmanagerservice/src/common/rapidjson/internal/stack.h \
    ../experimentmanagerservice/src/common/rapidjson/internal/strfunc.h \
    ../experimentmanagerservice/src/common/rapidjson/internal/strtod.h \
    ../experimentmanagerservice/src/common/rapidjson/internal/swap.h \
    ../experimentmanagerservice/src/common/rapidjson/istreamwrapper.h \
    ../experimentmanagerservice/src/common/rapidjson/memorybuffer.h \
    ../experimentmanagerservice/src/common/rapidjson/memorystream.h \
    ../experimentmanagerservice/src/common/rapidjson/msinttypes/inttypes.h \
    ../experimentmanagerservice/src/common/rapidjson/msinttypes/stdint.h \
    ../experimentmanagerservice/src/common/rapidjson/ostreamwrapper.h \
    ../experimentmanagerservice/src/common/rapidjson/pointer.h \
    ../experimentmanagerservice/src/common/rapidjson/prettywriter.h \
    ../experimentmanagerservice/src/common/rapidjson/rapidjson.h \
    ../experimentmanagerservice/src/common/rapidjson/reader.h \
    ../experimentmanagerservice/src/common/rapidjson/schema.h \
    ../experimentmanagerservice/src/common/rapidjson/stream.h \
    ../experimentmanagerservice/src/common/rapidjson/stringbuffer.h \
    ../experimentmanagerservice/src/common/rapidjson/uri.h \
    ../experimentmanagerservice/src/common/rapidjson/writer.h \
    uiMainWindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -ltacopie -lcpp_redis -L/usr/local/lib
