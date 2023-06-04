cmake_minimum_required(VERSION 3.16)

project(QtOsgViewer LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)

set(INSTALL_DIR "QtOsgViewer")
set(SRC_DIR "src")
set(SRC_GUI_DIR  ${SRC_DIR}/gui)
set(INCLUDE_DIR "include")
set(INCLUDE_GUI_DIR ${INCLUDE_DIR}/gui)

option(OpenSceneGraph_DEBUG on)

file(GLOB GUI_HEADERS ${INCLUDE_GUI_DIR}/*.h)
file(GLOB GUI_SRCS ${SRC_GUI_DIR}/*.cpp)

#Qt
find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL OpenGLWidgets Widgets)
qt_standard_project_setup()

#OpenSceneGraph
find_package(OpenSceneGraph REQUIRED osg osgDB OpenThreads osgGA osgViewer osgUtil)

add_library(Gui SHARED
    ${GUI_HEADERS}
    ${GUI_SRCS}
)

target_include_directories(Gui PRIVATE
    ${INCLUDE_GUI_DIR}
    ${OSG_INCLUDE_DIR}
)

target_link_libraries(Gui PRIVATE
    #Qt6
    Qt6::Core
    Qt6::Gui
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    Qt6::Widgets

    #OSG
    ${OSG_LIBRARIES} 
    ${OSGVIEWER_LIBRARIES} 
    ${OSGUTIL_LIBRARIES} 
    ${OSGDB_LIBRARIES} 
    ${OSGGA_LIBRARIES} 
    ${OPENTHREADS_LIBRARIES}
)

add_executable(${PROJECT_NAME}
   ${SRC_DIR}/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${INCLUDE_GUI_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    #Qt6
    Qt6::Widgets

    #GUI
    Gui
)



install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "${INSTALL_DIR}"
    BUNDLE DESTINATION "${INSTALL_DIR}"
    LIBRARY DESTINATION "${INSTALL_DIR}"
)
