project(correction-report)

add_executable(correction-report correction-report.cpp)
if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(correction-report graph-flow)
endif()

target_include_directories(correction-report PRIVATE
  ${CMAKE_SOURCE_DIR}/modules/utils/include
  ${CMAKE_SOURCE_DIR}/modules/shape-evolution/include
  ${CMAKE_SOURCE_DIR}/modules/core/include
  ${MAGLAC_POKER_INCLUDE_DIRS}
  ${GRAPHFLOW_INCLUDE_DIRS}
  ${DGTAL_INCLUDE_DIRS}
)

target_link_directories(correction-report PRIVATE
        ${GRAPHFLOW_LIBS_DIR}
        ${BOOST_LIBS_DIR})

target_link_libraries(correction-report
  -lgraph-flow-utils
  -lgraph-flow-core
  -lgraph-flow-shape-evolution
  -lpthread
  ${DGTAL_LIBRARIES}
)

install(TARGETS correction-report
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
