qt_add_library(utils STATIC
    ansi_color_text.cpp
)
target_link_libraries(utils PRIVATE Qt6::Widgets)
target_include_directories(utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
