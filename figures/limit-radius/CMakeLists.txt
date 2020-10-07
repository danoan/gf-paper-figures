cmake_minimum_required(VERSION 3.2)
project(generate-disk)

add_executable(generate-disk
        generate-disk.cpp)

if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(generate-disk dipacus geoc)
endif()

target_include_directories(generate-disk PRIVATE
        include
        ${BOOST_INCLUDE_DIRS}
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_directories(generate-disk PRIVATE
        ${DIPACUS_LIBS_DIR}
        ${BOOST_LIBS_DIR})

target_link_libraries(generate-disk
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

install(TARGETS generate-disk
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)