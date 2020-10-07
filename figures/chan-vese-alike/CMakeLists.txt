cmake_minimum_required(VERSION 3.2)
project(highlight-contours)

add_executable(highlight-contour
        highlight-contour.cpp)

if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(highlight-contour dipacus geoc)
endif()

target_include_directories(highlight-contour PRIVATE
        include
        ${BOOST_INCLUDE_DIRS}
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_directories(highlight-contour PRIVATE
        ${DIPACUS_LIBS_DIR}
        ${BOOST_LIBS_DIR})

target_link_libraries(highlight-contour
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

install(TARGETS highlight-contour
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)