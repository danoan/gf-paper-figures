add_executable(hausdorff-distance hausdorff-distance.cpp)
add_dependencies(hausdorff-distance
dipacus)

target_include_directories(hausdorff-distance
PUBLIC
  ${DIPACUS_INCLUDE_DIRS}
  ${BOOST_INCLUDE_DIRS}
  ${DGTAL_INCLUDE_DIRS}
)

target_link_directories(hausdorff-distance
PUBLIC
  ${BOOST_LIBS_DIR}
  ${DIPACUS_LIBS_DIR}
)


target_link_libraries(hausdorff-distance
PUBLIC
  -ldipacus-base
  -ldipacus-components
  -lboost_filesystem
  -lboost_system
  -lpthread
  ${DGTAL_LIBRARIES}
  ${OpenCV_LIBS}
)

install(TARGETS hausdorff-distance
DESTINATION ${CMAKE_INSTAlL_PREFIX/bin}
)
