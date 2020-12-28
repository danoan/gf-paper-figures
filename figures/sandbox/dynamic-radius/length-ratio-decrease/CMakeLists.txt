cmake_minimum_required(VERSION 3.2)
project(length-ratio-decrease)

add_executable(length-ratio-decrease
        length-ratio-decrease.cpp)

if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(length-ratio-decrease dipacus geoc)
endif()

target_include_directories(length-ratio-decrease PRIVATE
        include
        ${BOOST_INCLUDE_DIRS}
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_directories(length-ratio-decrease PRIVATE
        ${DIPACUS_LIBS_DIR}
        ${BOOST_LIBS_DIR})


target_link_libraries(length-ratio-decrease
        -ldipacus-derivates
        -ldipacus-base
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

install(TARGETS length-ratio-decrease
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)