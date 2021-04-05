cmake_minimum_required(VERSION 3.2)
project(ring)

add_executable(ring
        ring.cpp)

if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(ring dipacus geoc)
endif()

target_include_directories(ring PRIVATE
        include
        ${BOOST_INCLUDE_DIRS}
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_directories(ring PRIVATE
        ${DIPACUS_LIBS_DIR}
        ${BOOST_LIBS_DIR})

target_link_libraries(ring
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

install(TARGETS ring
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)