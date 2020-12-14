cmake_minimum_required(VERSION 3.14)
project(graph-flow-paper-figures)

set(CMAKE_CXX_STANDARD 17)
set(EXTPROJECTS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/ext-projects/source)

if (${CMAKE_BUILD_TYPE} STREQUAL Release)
    set(EXTPROJECTS_BUILD_DIR ${CMAKE_SOURCE_DIR}/ext-projects/cmake-build-release)
else()
    set(EXTPROJECTS_BUILD_DIR ${CMAKE_SOURCE_DIR}/ext-projects/cmake-build-debug)
endif ()

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib:${EXTPROJECTS_BUILD_DIR}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(DIPACUS_INCLUDE_DIRS ${EXTPROJECTS_BUILD_DIR}/include CACHE PATH "DIPaCUS include location path.")
set(DIPACUS_LIBS_DIR ${EXTPROJECTS_BUILD_DIR}/lib CACHE PATH "DIPaCUS libraries location path.")

set(GEOC_INCLUDE_DIRS ${EXTPROJECTS_BUILD_DIR}/include CACHE PATH "GEOC include location path.")
set(MAGLAC_INCLUDE_DIRS ${EXTPROJECTS_BUILD_DIR}/include CACHE PATH "MagLac include location path.")

set(GRAPHFLOW_INCLUDE_DIRS ${EXTPROJECTS_BUILD_DIR}/include CACHE PATH "GraphFlow include location path.")
set(GRAPHFLOW_LIBS_DIR ${EXTPROJECTS_BUILD_DIR}/lib CACHE PATH "GraphFlow libraries location path.")


find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

add_subdirectory(ext-projects)
add_subdirectory(figures/zero-level-set)
add_subdirectory(figures/chan-vese-alike)
add_subdirectory(figures/ring)
add_subdirectory(figures/dynamic-radius/limit-radius)
add_subdirectory(figures/dynamic-radius/length-ratio-decrease)
add_subdirectory(figures/need-for-squared-curvature)
add_subdirectory(figures/threshold-dynamics)
add_subdirectory(figures/dynamic-radius/find-balance-radius)
add_subdirectory(figures/intrinsic-elastica-cost)
add_subdirectory(figures/metric-validation)
add_subdirectory(figures/dispersion-cost)