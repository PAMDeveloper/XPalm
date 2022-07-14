NAME = XPalm
QT       += core gui charts widgets

TEMPLATE = app
CONFIG += static

CONFIG += c++11 precompile_header
#pour la version test main()
#CONFIG += console

##### EXEC OPTIONS #####
#DEFINES += UNSAFE_RUN
DEFINES += FORCE_TRACE_ENUM
DEFINES += WITH_TRACE
########################

##### OPTIMIZATION #####
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
#########################


##### DEBUG OUTPUT #####
#QMAKE_CXXFLAGS += -save-temps #preprocessed headers
#QMAKE_CXXFLAGS += -P #preprocessed headers
#QMAKE_CFLAGS_RELEASE += -E
########################

##### PATH #####
MDL_SRC = ../src
ARTIS_SRC = ../../artis/src
DEST = ../bin
THIRD_PARTY = ../ext_libs
ARTIS_LIB = ../libs
################
msvc: {
    COMPILER = 'msvc'_$$(VisualStudioVersion)
    QMAKE_CXXFLAGS_RELEASE += -Ox
}
gcc: {
    COMPILER = 'gcc'_$$system($$QMAKE_CXX " -dumpversion")
    QMAKE_CXXFLAGS_RELEASE += -Ofast
}

contains(QT_ARCH, i386) { ARCHI = 'x86' }
else { ARCHI = 'x64' }

CONFIG(static, static|): {
    LINK = static
} else {
    LINK = shared
}


CONFIG(debug, debug|release) {
    contains(DEFINES, WITH_TRACE) {
        TARGET = $${NAME}d_trace
        LIBS += -L$$ARTIS_LIB/$$COMPILER/$$ARCHI/$$LINK -lartisd_trace
    } else {
        TARGET = $${NAME}d
        LIBS += -L$$ARTIS_LIB/$$COMPILER/$$ARCHI/$$LINK -lartisd
    }
} else {
    contains(DEFINES, WITH_TRACE) {
        TARGET = $${NAME}_trace
        LIBS += -L$$ARTIS_LIB/$$COMPILER/$$ARCHI/$$LINK -lartis_trace
    } else {
        TARGET = $${NAME}
        LIBS += -L$$ARTIS_LIB/$$COMPILER/$$ARCHI/$$LINK -lartis
    }
}

equals(TEMPLATE,lib) {
    DESTDIR = $$DEST/$$COMPILER/$$ARCHI/$$LINK
} else {
    DESTDIR = $$DEST/$$COMPILER/$$ARCHI/$$LINK
}

LIBS += -L$$THIRD_PARTY/$$COMPILER/$$ARCHI/$$LINK
LIBS += -lSecur32 -lWs2_32 -lShell32 -lAdvapi32

INCLUDEPATH +=  $$THIRD_PARTY/include \
                $$MDL_SRC \
                ./  \
                $$ARTIS_SRC  \

message(building $$TEMPLATE $$TARGET $$ARCHI $$COMPILER $$LINK to: $$DESTDIR)


PRECOMPILED_HEADER  = $$MDL_SRC/defines.hpp

HEADERS += \
    mainwindow.h \
    meteodatamodel.h \
    parametersdatamodel.h \
    view.h \
    callout.h \
    tracemodel.h \
    \
    $$MDL_SRC/ModelParameters.hpp \
    $$MDL_SRC/utils/ParametersReader.hpp \
    $$MDL_SRC/utils/resultparser.h \
    $$MDL_SRC/utils/juliancalculator.h\
    $$MDL_SRC/observer/PlantView.hpp \
    $$MDL_SRC/plant/processes/reserve.h \
    $$MDL_SRC/plant/root.h \
    $$MDL_SRC/plant/phytomer/inflo.h \
    $$MDL_SRC/plant/phytomer/bunch.h \
    $$MDL_SRC/plant/phytomer/internode.h \
    $$MDL_SRC/plant/phytomer/leaf.h \
    $$MDL_SRC/plant/phytomer/male_inflo.h \
    $$MDL_SRC/plant/phytomer/peduncle.h \
    $$MDL_SRC/plant/phytomer/phytomer.h \
    $$MDL_SRC/plant/processes/meteo.h \
    $$MDL_SRC/plant/processes/waterbalance.h \
    $$MDL_SRC/plant/tree.h \
    $$MDL_SRC/observer/PhytomerView.hpp

SOURCES += \
    mainwindow.cpp \
    meteodatamodel.cpp \
    parametersdatamodel.cpp \
    view.cpp \
    callout.cpp \
    tracemodel.cpp \
    main.cpp \
#    $$SRC/../artis_lite/simpletrace.cpp \

FORMS += \
    mainwindow.ui
