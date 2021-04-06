project(export-seg-mask)

add_executable(export-seg-mask export-seg-mask.cpp)

if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(export-seg-mask graph-flow)
endif()

target_include_directories(export-seg-mask PRIVATE
${CMAKE_SOURCE_DIR}/modules/contour-correction/include
${CMAKE_SOURCE_DIR}/modules/io/seed/include
${CMAKE_SOURCE_DIR}/modules/utils/include
${MAGLAC_POKER_INCLUDE_DIRS}
${GRAPHFLOW_INCLUDE_DIRS}
${DGTAL_INCLUDE_DIRS})

target_link_directories(export-seg-mask PRIVATE
        ${GRAPHFLOW_LIBS_DIR}
        ${BOOST_LIBS_DIR})

target_link_libraries(export-seg-mask
-lgraph-flow-utils
-lgraph-flow-core
-lgraph-flow-io
-lgraph-flow-contour-correction
${DGTAL_LIBRARIES}
${OpenCV_LIBS})

install(TARGETS export-seg-mask
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)