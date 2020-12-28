cmake_minimum_required(VERSION 3.14)
project(dispersion-cost-cos)

add_executable(dispersion-cost-cos
dispersion-cost-cos.cpp
src/estimators.cpp
src/FlowGraph.cpp)

if(${USE_REMOTE_REPOSITORIES})
        add_dependencies(dispersion-cost-cos dipacus graph-flow)        
endif()


target_include_directories(dispersion-cost-cos PRIVATE
include
${DIPACUS_INCLUDE_DIRS}
${GRAPH_FLOW_INCLUDE_DIRS}
${DGTAL_INCLUDE_DIRS}
${BOOST_INCLUDE_DIRS}
)

target_link_directories(dispersion-cost-cos PRIVATE
        ${DIPACUS_LIBS_DIR}
        ${BOOST_LIBS_DIR}
        ${GRAPHFLOW_LIBS_DIR})

target_link_libraries(dispersion-cost-cos PRIVATE
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


install(TARGETS dispersion-cost-cos
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)