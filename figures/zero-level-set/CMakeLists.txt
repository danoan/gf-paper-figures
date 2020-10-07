cmake_minimum_required(VERSION 3.2)
project(zero-level-set)

add_executable(zero-level-set
        zero-level-set.cpp)

if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(zero-level-set dipacus geoc)
endif()

target_include_directories(zero-level-set PRIVATE
        include
        ${BOOST_INCLUDE_DIRS}
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_directories(zero-level-set PRIVATE
        ${DIPACUS_LIBS_DIR}
        ${BOOST_LIBS_DIR})

target_link_libraries(zero-level-set
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

install(TARGETS zero-level-set
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)