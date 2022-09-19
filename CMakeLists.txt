cmake_minimum_required(VERSION 3.15)
project(XPalm_standalone VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)

add_executable(XPalm_standalone
    main.cpp
    src/ModelParameters.hpp
    src/observer/PhytomerView.hpp
    src/observer/PlantView.hpp
    src/plant/phytomer/bunch.h
    src/plant/phytomer/inflo.h
    src/plant/phytomer/internode.h
    src/plant/phytomer/leaf.h
    src/plant/phytomer/male_inflo.h
    src/plant/phytomer/peduncle.h
    src/plant/phytomer/phytomer.h
    src/plant/processes/meteo.h
    src/plant/processes/reserve.h
    src/plant/processes/waterbalance.h
    src/plant/root.h
    src/plant/tree.h
    src/utils/ParametersReader.hpp
    src/utils/juliancalculator.h
    src/utils/resultparser.h
)
target_include_directories(XPalm_standalone PRIVATE
    ../artis/src
    ../ext_libs/include
    /
    src
)

target_compile_definitions(XPalm_standalone PRIVATE
    FORCE_TRACE_ENUM
)

target_link_libraries(XPalm_standalone PRIVATE
    Advapi32
    Secur32
    Shell32
    Ws2_32
)

if(CMAKE_BUILD_TYPE STREQUAL Debug AND DEFINES___contains___WITH_TRACE)
    target_link_libraries(XPalm_standalone PRIVATE
        # Remove: L///
        artisd_trace
    )
endif()

if(CMAKE_BUILD_TYPE STREQUAL Debug AND NOT DEFINES___contains___WITH_TRACE)
    target_link_libraries(XPalm_standalone PRIVATE
        # Remove: L///
        artisd
    )
endif()

if(DEFINES___contains___WITH_TRACE AND NOT CMAKE_BUILD_TYPE STREQUAL Debug)
    target_link_libraries(XPalm_standalone PRIVATE
        # Remove: L///
        artis_trace
    )
endif()

if(NOT CMAKE_BUILD_TYPE STREQUAL Debug AND NOT DEFINES___contains___WITH_TRACE)
    target_link_libraries(XPalm_standalone PRIVATE
        # Remove: L///
        artis
    )
endif()

install(TARGETS XPalm_standalone
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Consider using qt_generate_deploy_app_script() for app deployment if
# the project can use Qt 6.3. In that case rerun qmake2cmake with
# --min-qt-version=6.3.
