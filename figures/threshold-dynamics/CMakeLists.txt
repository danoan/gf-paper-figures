cmake_minimum_required(VERSION 3.14)
project(threshold-dynamics)

add_executable(threshold-dynamics 
threshold-dynamics.cpp)

add_dependencies(threshold-dynamics dipacus)

target_include_directories(threshold-dynamics PRIVATE
include
${DIPACUS_INCLUDE_DIRS}
${GRAPH_FLOW_INCLUDE_DIRS}
${DGTAL_INCLUDE_DIRS}
${BOOST_INCLUDE_DIRS}
)

target_link_directories(threshold-dynamics PRIVATE
        ${DIPACUS_LIBS_DIR}
        ${BOOST_LIBS_DIR}
        ${GRAPHFLOW_LIBS_DIR})

target_link_libraries(threshold-dynamics PRIVATE
-lgraph-flow-utils
-ldipacus-derivates
-ldipacus-components
-ldipacus-base
-lboost_filesystem
-lboost_system
${DGTAL_LIBRARIES}
${OpenCV_LIBS}
)


install(TARGETS threshold-dynamics
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)