cmake_minimum_required(VERSION 3.16)

project(socketui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick QuickControls2 WebSockets REQUIRED)

qt_add_executable(appsocketui
    src/main.cpp
    src/messagemodel.cpp
    src/rootstate.cpp
    src/websocketclient.cpp
)

qt_add_qml_module(appsocketui
    URI socketui
    VERSION 1.0
    QML_FILES
    qml/main.qml
)

set_target_properties(appsocketui PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER blq.me
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(appsocketui
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(appsocketui
    PRIVATE Qt6::Quick  Qt6::QuickControls2 Qt6::WebSockets)
