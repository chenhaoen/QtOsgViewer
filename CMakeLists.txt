cmake_minimum_required(VERSION 3.16)

project(QtOsgViewer LANGUAGES CXX)

if(NOT DEFINED INSTALL_DIR)
    set(INSTALL_DIR "QtOsgViewer")
endif()

set(SRC_DIR "src")
set(INCLUDE_DIR "include")

option(OpenSceneGraph_DEBUG on)

#Qt
find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL OpenGLWidgets Widgets)
qt_standard_project_setup()

#OpenSceneGraph
find_package(OpenSceneGraph REQUIRED osg osgDB OpenThreads osgGA osgViewer osgUtil)

add_executable(QtOsgViewer
   ${SRC_DIR}/main.cpp
)

INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})

LINK_DIRECTORIES(${OSG_LIB_DIR})

target_link_libraries(QtOsgViewer PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    Qt6::Widgets
    ${OSG_LIBRARIES} 
    ${OSGVIEWER_LIBRARIES} 
    ${OSGUTIL_LIBRARIES} 
    ${OSGDB_LIBRARIES} 
    ${OSGGA_LIBRARIES} 
    ${OPENTHREADS_LIBRARIES}
)

install(TARGETS QtOsgViewer
    RUNTIME DESTINATION "${INSTALL_DIR}"
    BUNDLE DESTINATION "${INSTALL_DIR}"
    LIBRARY DESTINATION "${INSTALL_DIR}"
)
