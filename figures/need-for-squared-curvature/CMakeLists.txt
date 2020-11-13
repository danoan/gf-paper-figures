cmake_minimum_required(VERSION 3.2)
project(need-for-squared-curvature)

add_executable(need-for-squared-curvature
        need-for-squared-curvature.cpp
        src/RoundedRect.cpp
        src/energy.cpp)

if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(need-for-squared-curvature dipacus geoc)
endif()

target_include_directories(need-for-squared-curvature PRIVATE
        include
        ${BOOST_INCLUDE_DIRS}
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_directories(need-for-squared-curvature PRIVATE
        ${DIPACUS_LIBS_DIR}
        ${BOOST_LIBS_DIR})

target_link_libraries(need-for-squared-curvature
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

add_executable(best-segmentation
        best-segmentation.cpp
        src/RoundedRect.cpp
        src/energy.cpp)

if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(best-segmentation dipacus geoc)
endif()

target_include_directories(best-segmentation PRIVATE
        include
        ${BOOST_INCLUDE_DIRS}
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_directories(best-segmentation PRIVATE
        ${DIPACUS_LIBS_DIR}
        ${BOOST_LIBS_DIR})

target_link_libraries(best-segmentation
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

install(TARGETS need-for-squared-curvature best-segmentation
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)