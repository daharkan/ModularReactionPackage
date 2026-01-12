TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
QT += core serialport concurrent
QT -= gui

SOURCES += \
        src/BusboardSerialManager.cpp \
        src/common/Arc.cpp \
        src/BusboardSim.cpp \
        src/BusboardV1.cpp \
        src/common/Cell.cpp \
        src/IBusboard.cpp \
        src/common/CellVisuals.cpp \
        src/common/CellVisualsHistory.cpp \
        src/common/FlowStatus.cpp \
        src/common/RedisDBManager.cpp \
        src/ServiceRunner.cpp \
        src/common/User.cpp \
        src/common/Experiment.cpp \
        src/common/Profile.cpp \
        src/common/RPMArc.cpp \
        src/common/TempArc.cpp \
        src/main.cpp

HEADERS += \
    src/BusboardSerialManager.h \
    src/common/Arc.h \
    src/BusboardSim.h \
    src/BusboardV1.h \
    src/common/Cell.h \
    src/IBusboard.h \
    src/common/CellVisuals.h \
    src/common/CellVisualsHistory.h \
    src/common/FlowStatus.h \
    src/common/RedisDBManager.h \
    src/ServiceRunner.h \
    src/common/User.h \
    src/common/Experiment.h \
    src/common/Profile.h \
    src/common/RPMArc.h \
    src/common/TempArc.h \
    src/common/rapidjson/allocators.h \
    src/common/rapidjson/cursorstreamwrapper.h \
    src/common/rapidjson/document.h \
    src/common/rapidjson/encodedstream.h \
    src/common/rapidjson/encodings.h \
    src/common/rapidjson/error/en.h \
    src/common/rapidjson/error/error.h \
    src/common/rapidjson/filereadstream.h \
    src/common/rapidjson/filewritestream.h \
    src/common/rapidjson/fwd.h \
    src/common/rapidjson/internal/biginteger.h \
    src/common/rapidjson/internal/clzll.h \
    src/common/rapidjson/internal/diyfp.h \
    src/common/rapidjson/internal/dtoa.h \
    src/common/rapidjson/internal/ieee754.h \
    src/common/rapidjson/internal/itoa.h \
    src/common/rapidjson/internal/meta.h \
    src/common/rapidjson/internal/pow10.h \
    src/common/rapidjson/internal/regex.h \
    src/common/rapidjson/internal/stack.h \
    src/common/rapidjson/internal/strfunc.h \
    src/common/rapidjson/internal/strtod.h \
    src/common/rapidjson/internal/swap.h \
    src/common/rapidjson/istreamwrapper.h \
    src/common/rapidjson/memorybuffer.h \
    src/common/rapidjson/memorystream.h \
    src/common/rapidjson/msinttypes/inttypes.h \
    src/common/rapidjson/msinttypes/stdint.h \
    src/common/rapidjson/ostreamwrapper.h \
    src/common/rapidjson/pointer.h \
    src/common/rapidjson/prettywriter.h \
    src/common/rapidjson/rapidjson.h \
    src/common/rapidjson/reader.h \
    src/common/rapidjson/schema.h \
    src/common/rapidjson/stream.h \
    src/common/rapidjson/stringbuffer.h \
    src/common/rapidjson/uri.h \
    src/common/rapidjson/writer.h



LIBS += -ltacopie -lcpp_redis -L/usr/local/lib
