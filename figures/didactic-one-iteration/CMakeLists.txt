project(didactic-one-iteration)

add_executable(didactic-one-iteration
didactic-one-iteration.cpp)

add_dependencies(didactic-one-iteration 
graph-flow)

target_include_directories(didactic-one-iteration
PUBLIC
  ${GRAPHFLOW_INCLUDE_DIRS}
  ${DGTAL_INCLUDE_DIRS}
  ${BOOST_INCLUDE_DIRS}
  ${MAGLAC_POKER_INCLUDE_DIRS}
)

target_link_directories(didactic-one-iteration
PUBLIC
  ${GRAPHFLOW_LIBS_DIR}
  ${BOOST_LIBS_DIR}
)

target_link_libraries(didactic-one-iteration
PUBLIC
  -lgraph-flow-shape-evolution
  -lgraph-flow-core
  -lgraph-flow-utils
  -lboost_filesystem
  -lboost_system
  -lpthread
  ${DGTAL_LIBRARIES}
  ${OpenCV_LIBS}
)

install(TARGETS didactic-one-iteration
DESTINATION ${CMAKE_INSTAlL_PREFIX/bin}
)