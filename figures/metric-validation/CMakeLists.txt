cmake_minimum_required(VERSION 3.14)
project(metric-validation)

add_executable(metric-validation
metric-validation.cpp
src/estimators.cpp
src/FlowGraph.cpp)

if(${USE_REMOTE_REPOSITORIES})
        add_dependencies(metric-validation dipacus graph-flow)        
endif()


target_include_directories(metric-validation PRIVATE
include
${DIPACUS_INCLUDE_DIRS}
${GRAPH_FLOW_INCLUDE_DIRS}
${DGTAL_INCLUDE_DIRS}
${BOOST_INCLUDE_DIRS}
)

target_link_directories(metric-validation PRIVATE
        ${DIPACUS_LIBS_DIR}
        ${BOOST_LIBS_DIR}
        ${GRAPHFLOW_LIBS_DIR})

target_link_libraries(metric-validation PRIVATE
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


install(TARGETS metric-validation
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)