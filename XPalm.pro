QT       += core gui charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 precompile_header
TEMPLATE = app
CONFIG += static
#CONFIG += console

#DEFINES += UNSAFE_RUN
#DEFINES += WITH_TRACE FORCE_TRACE_ENUM
#DEFINES += WITH_TRACE

QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
##MSVC OPTIM FLAGS
QMAKE_CXXFLAGS_RELEASE += -Ox
##MINGW OPTIM FLAGS
#QMAKE_CXXFLAGS_RELEASE += -Ofast

## DEBUG OUTPUT
#QMAKE_CXXFLAGS += -save-temps #preprocessed headers
#QMAKE_CXXFLAGS += -P #preprocessed headers
#QMAKE_CFLAGS_RELEASE += -E

NAME = XPalm

SRC_ = src/
R_SRC_ = src/
ARTIS_SRC_ = ../artis/src
3P_LIBS_ = ../ext_libs
#LIBS_ = ../libs


ARCHI = x64
COMPILER = msvc14
#COMPILER = mingw-4.9.3
LIBS += -lSecur32 -lWs2_32 -lShell32 -lAdvapi32
LIBS += -L$$3P_LIBS_/$$COMPILER/$$ARCHI/static
#LIBS += -L$$3P_LIBS_/$$COMPILER/$$ARCHI/static -llibpq
DEPENDPATH += -L$$3P_LIBS_/$$COMPILER/$$ARCHI/static

DEPENDPATH += $$ARTIS_SRC_

#CONFIG(debug, debug|release) {
#    TARGET = $${NAME}d
#    LIBS += -L$$LIBS_/$$COMPILER/$$ARCHI/static -lartisd
#} else {
#    TARGET = $${NAME}
#    LIBS += -L$$LIBS_/$$COMPILER/$$ARCHI/static -lartis
#}

#LINK = static
equals(TEMPLATE,lib) {
    message(building LIB)
    DESTDIR = $$LIBS_/$$COMPILER/$$ARCHI/$$LINK
} else {
    message(building APP)
    DESTDIR = ../bin/$$COMPILER/$$ARCHI
}

INCLUDEPATH +=  $$3P_LIBS_/include \
                $$SRC_ \
                $$R_SRC_ \
                $$ARTIS_SRC_


message($$TARGET - $$TEMPLATE - $$ARCHI - $$LINK - $$COMPILER)
message(to: $$DESTDIR)

PRECOMPILED_HEADER  = $$R_SRC_/defines.hpp

HEADERS += \
    $$SRC_/qtapp/mainwindow.h \
    $$SRC_/qtapp/meteodatamodel.h \
    $$SRC_/qtapp/parametersdatamodel.h \
    $$SRC_/qtapp/view.h \
    $$SRC_/qtapp/callout.h \
    $$SRC_/qtapp/tracemodel.h \
    ../artis/src/artis/builder/Builder.hpp \
    ../artis/src/artis/builder/ModelFactory.hpp \
    ../artis/src/artis/context/Context.hpp \
    ../artis/src/artis/context/State.hpp \
    ../artis/src/artis/context/StateValues.hpp \
    ../artis/src/artis/context/Value.hpp \
    ../artis/src/artis/kernel/AbstractAtomicModel.hpp \
    ../artis/src/artis/kernel/AbstractCoupledModel.hpp \
    ../artis/src/artis/kernel/AbstractModel.hpp \
    ../artis/src/artis/kernel/Any.hpp \
    ../artis/src/artis/kernel/Externals.hpp \
    ../artis/src/artis/kernel/Internals.hpp \
    ../artis/src/artis/kernel/Macro.hpp \
    ../artis/src/artis/kernel/Node.hpp \
    ../artis/src/artis/kernel/Simulator.hpp \
    ../artis/src/artis/kernel/States.hpp \
    ../artis/src/artis/lib.hpp \
    ../artis/src/artis/observer/Observer.hpp \
    ../artis/src/artis/observer/Output.hpp \
    ../artis/src/artis/observer/View.hpp \
    ../artis/src/artis/utils/DateTime.hpp \
    ../artis/src/artis/utils/DoubleTime.hpp \
    ../artis/src/artis/utils/Exception.hpp \
    ../artis/src/artis/utils/ParametersReader.hpp \
    ../artis/src/artis/utils/Singleton.hpp \
    ../artis/src/artis/utils/Time.hpp \
    ../artis/src/artis/utils/Trace.hpp \
    \
    $$R_SRC_/ModelParameters.hpp \
    $$R_SRC_/utils/ParametersReader.hpp \
    $$R_SRC_/utils/resultparser.h \
    $$R_SRC_/utils/juliancalculator.h\
    \
    $$R_SRC_/observer/PlantView.hpp \
    $$R_SRC_/plant/processes/reserve.h \
    $$R_SRC_/plant/root.h \
    $$R_SRC_/plant/phytomer/inflo.h \
    $$R_SRC_/plant/phytomer/bunch.h \
    $$R_SRC_/plant/phytomer/internode.h \
    $$R_SRC_/plant/phytomer/leaf.h \
    $$R_SRC_/plant/phytomer/male_inflo.h \
    $$R_SRC_/plant/phytomer/peduncle.h \
    $$R_SRC_/plant/phytomer/phytomer.h \
    $$R_SRC_/plant/processes/meteo.h \
    $$R_SRC_/plant/processes/waterbalance.h \
    $$R_SRC_/plant/tree.h \
    $$R_SRC_/sampleatomic.h

SOURCES += \
    $$SRC_/qtapp/mainwindow.cpp \
    $$SRC_/qtapp/meteodatamodel.cpp \
    $$SRC_/qtapp/parametersdatamodel.cpp \
    $$SRC_/qtapp/view.cpp \
    $$SRC_/qtapp/callout.cpp \
    $$SRC_/qtapp/tracemodel.cpp \
    $$SRC_/artis_lite/simpletrace.cpp \
    $$SRC_/main.cpp \
    ../artis/src/artis/builder/Builder.cpp \
    ../artis/src/artis/kernel/Any.cpp \
    ../artis/src/artis/kernel/Simulator.cpp \
    ../artis/src/artis/lib.cpp \
    ../artis/src/artis/observer/Observer.cpp \
    ../artis/src/artis/observer/Output.cpp \
    ../artis/src/artis/utils/DateTime.cpp \
    ../artis/src/artis/utils/ParametersReader.cpp \
    ../artis/src/artis/utils/Trace.cpp

FORMS += \
    $$SRC_/qtapp/mainwindow.ui

