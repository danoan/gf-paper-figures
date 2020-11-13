cmake_minimum_required(VERSION 3.14)
project(graph-flow-paper-figures)

set(CMAKE_CXX_STANDARD 14)
set(EXTPROJECTS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/ext-projects/source)

if (${CMAKE_BUILD_TYPE} STREQUAL Release)
    set(EXTPROJECTS_BUILD_DIR ${CMAKE_SOURCE_DIR}/ext-projects/cmake-build-release)
else()
    set(EXTPROJECTS_BUILD_DIR ${CMAKE_SOURCE_DIR}/ext-projects/cmake-build-debug)
endif ()

set(DIPACUS_INCLUDE_DIRS ${EXTPROJECTS_BUILD_DIR}/include CACHE PATH "DIPaCUS include location path.")
set(DIPACUS_LIBS_DIR ${EXTPROJECTS_BUILD_DIR}/lib CACHE PATH "DIPaCUS libraries location path.")

set(BTOOLS_INCLUDE_DIRS ${EXTPROJECTS_BUILD_DIR}/include CACHE PATH "BTools include location path.")
set(BTOOLS_LIBS_DIR ${EXTPROJECTS_BUILD_DIR}/lib CACHE PATH "BTools libraries location path.")

set(GCURVE_INCLUDE_DIRS ${EXTPROJECTS_BUILD_DIR}/include CACHE PATH "GCurve include location path.")
set(GCURVE_LIBS_DIR ${EXTPROJECTS_BUILD_DIR}/lib CACHE PATH "GCurve libraries location path.")


find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

add_subdirectory(ext-projects)
add_subdirectory(figures/zero-level-set)
add_subdirectory(figures/chan-vese-alike)
add_subdirectory(figures/ring)
add_subdirectory(figures/dynamic-radius/limit-radius)
add_subdirectory(figures/dynamic-radius/length-ratio-decrease)
add_subdirectory(figures/need-for-squared-curvature)