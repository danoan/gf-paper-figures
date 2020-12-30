cmake_minimum_required(VERSION 3.14)
project(ideal-cost)

add_executable(ideal-cost 
ideal-cost.cpp
src/estimators.cpp
src/FlowGraph.cpp)

if(${USE_REMOTE_REPOSITORIES})
        add_dependencies(ideal-cost dipacus graph-flow)        
endif()


target_include_directories(ideal-cost PRIVATE
include
${DIPACUS_INCLUDE_DIRS}
${GRAPH_FLOW_INCLUDE_DIRS}
${DGTAL_INCLUDE_DIRS}
${BOOST_INCLUDE_DIRS}
)

target_link_directories(ideal-cost PRIVATE
        ${DIPACUS_LIBS_DIR}
        ${BOOST_LIBS_DIR}
        ${GRAPHFLOW_LIBS_DIR})

target_link_libraries(ideal-cost PRIVATE
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


install(TARGETS ideal-cost
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)