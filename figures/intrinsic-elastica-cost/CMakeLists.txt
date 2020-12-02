cmake_minimum_required(VERSION 3.14)
project(intrinsic-elastica-cost)

add_executable(intrinsic-elastica-cost 
intrinsic-elastica-cost.cpp
src/estimators.cpp
src/display.cpp
src/utils.cpp
src/inputReader.cpp
src/FlowGraph.cpp)

if(${USE_REMOTE_REPOSITORIES})
        add_dependencies(intrinsic-elastica-cost dipacus graph-flow)        
endif()


target_include_directories(intrinsic-elastica-cost PRIVATE
include
${DIPACUS_INCLUDE_DIRS}
${GRAPH_FLOW_INCLUDE_DIRS}
${DGTAL_INCLUDE_DIRS}
${BOOST_INCLUDE_DIRS}
)

target_link_directories(intrinsic-elastica-cost PRIVATE
        ${DIPACUS_LIBS_DIR}
        ${BOOST_LIBS_DIR}
        ${GRAPHFLOW_LIBS_DIR})

target_link_libraries(intrinsic-elastica-cost PRIVATE
-lgraph-flow-utils
-lgraph-flow-core
-lgraph-flow-shape-evolution
-ldipacus-derivates
-ldipacus-components
-ldipacus-base
-lemon
-lboost_filesystem
-lboost_system
-pthread
${DGTAL_LIBRARIES}
${OpenCV_LIBS}
)


install(TARGETS intrinsic-elastica-cost
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)