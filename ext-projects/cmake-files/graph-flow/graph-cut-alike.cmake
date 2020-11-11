ExternalProject_Add(graph-flow
        GIT_REPOSITORY https://github.com/danoan/graph-flow.git
        GIT_TAG v0.2.3-dev
        SOURCE_DIR ${EXTPROJECTS_SOURCE_DIR}/graph-flow
        CMAKE_ARGS
        -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
        -DCMAKE_BUILD_TYPE=Release
        -DCMAKE_INSTALL_PREFIX=${EXTPROJECTS_BUILD_DIR}
        -DCREATE_SHARED_LIBRARIES=ON
        -DUSE_REMOTE_REPOSITORIES=OFF
        -DDIPACUS_INCLUDE_DIRS=${EXTPROJECTS_BUILD_DIR}/include
        -DDIPACUS_LIBS_DIR=${EXTPROJECTS_BUILD_DIR}/lib
        -DGCURVE_INCLUDE_DIRS=${EXTPROJECTS_BUILD_DIR}/include
        -DGCURVE_LIBS_DIR=${EXTPROJECTS_BUILD_DIR}/lib
        -DGEOC_INCLUDE_DIRS=${EXTPROJECTS_BUILD_DIR}/include
        -DBTOOLS_INCLUDE_DIRS=${EXTPROJECTS_BUILD_DIR}/include
        -DBTOOLS_LIBS_DIRS=${EXTPROJECTS_BUILD_DIR}/lib
        -DMAGLAC_INCLUDE_DIRS=${EXTPROJECTS_BUILD_DIR}/include
        -DLEMON_INCLUDE_DIRS=${CMAKE_PREFIX_PATH}/include
        -DLEMON_LIBS_DIRS=${CMAKE_PREFIX_PATH}/lib
        -DBOOST_INCLUDE_DIRS=${BOOST_INCLUDE_DIRS}
        -DBOOST_LIBS_DIR=${BOOST_LIBS_DIR})

add_dependencies(graph-flow dipacus geoc gcurve magLac btools)