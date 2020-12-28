cmake_minimum_required(VERSION 3.14)
project(find-balance-radius)

add_executable(find-balance-radius 
find-balance-radius.cpp
src/energy.cpp)

if(${USE_REMOTE_REPOSITORIES})
        add_dependencies(find-balance-radius dipacus graph-flow)        
endif()


target_include_directories(find-balance-radius PRIVATE
include
${DIPACUS_INCLUDE_DIRS}
${GRAPH_FLOW_INCLUDE_DIRS}
${DGTAL_INCLUDE_DIRS}
${BOOST_INCLUDE_DIRS}
)

target_link_directories(find-balance-radius PRIVATE
        ${DIPACUS_LIBS_DIR}
        ${BOOST_LIBS_DIR}
        ${GRAPHFLOW_LIBS_DIR})

target_link_libraries(find-balance-radius PRIVATE
-lgraph-flow-utils
-lgraph-flow-core
-lgraph-flow-shape-evolution
-ldipacus-derivates
-ldipacus-components
-ldipacus-base
-lboost_filesystem
-lboost_system
-pthread
${DGTAL_LIBRARIES}
${OpenCV_LIBS}
)


install(TARGETS find-balance-radius
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)