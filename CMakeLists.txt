cmake_minimum_required(VERSION 3.21)

project(qsing-box VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools)
qt_standard_project_setup()

add_subdirectory(src)

set(qsing-box_sources
    src/about_dialog.cpp
    src/about_dialog.ui
    src/main.cpp
    src/main_window.cpp
    src/main_window.ui
    src/settings_dialog.cpp
    src/settings_dialog.ui
    src/tray_icon.cpp
)

qt_add_executable(qsing-box
    ${qsing-box_sources}
    resources/app.rc
)

target_include_directories(qsing-box PRIVATE
    include
)

target_link_libraries(qsing-box PRIVATE
    Qt6::Widgets
    config
    proxy
    settings
    utils
)

set_target_properties(qsing-box PROPERTIES
    WIN32_EXECUTABLE ON
)

set(qsing-box_resources
    "resources/images/app.ico"
    "resources/images/app_enable_proxy.ico"
    "resources/images/status_enabled.png"
    "resources/images/status_disabled.png"
    "resources/images/about.png"
    "resources/images/add-new.png"
    "resources/images/delete.png"
    "resources/images/edit.png"
    "resources/images/import.png"
    "resources/images/save.png"
    "resources/images/setting.png"
    "resources/images/start.png"
    "resources/images/stop.png"
    "resources/images/switch.png"
    "resources/images/qt_logo.png"
    "resources/texts/gpl-3.0.txt"
    "resources/xml/task.xml"
)

qt_add_resources(qsing-box res
    PREFIX "/"
    BASE "resources"
    FILES
        ${qsing-box_resources}
)

qt_add_translations(qsing-box
    TS_FILES
        languages/qsing-box_zh_CN.ts
        languages/qsing-box_en_US.ts
    LUPDATE_OPTIONS
        -no-obsolete
)

string(TIMESTAMP BUILD_TIME "%Y.%m.%d %H:%M")
add_definitions(-DPROJECT_VERSION="${PROJECT_VERSION}")
add_definitions(-DBUILD_TIME="${BUILD_TIME}")
add_definitions(-DCOMPILER_ID="${CMAKE_CXX_COMPILER_ID}")
add_definitions(-DCOMPILER_VERSION="${CMAKE_CXX_COMPILER_VERSION}")
