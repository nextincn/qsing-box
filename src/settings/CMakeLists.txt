qt_add_library(settings STATIC
    privilege_manager.cpp
    settings_manager.cpp
    task_scheduler.cpp
)
target_link_libraries(settings PRIVATE Qt6::Core)
target_include_directories(settings INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
