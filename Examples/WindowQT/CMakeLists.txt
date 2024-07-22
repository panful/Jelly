set(target_name example_WindowQT)

find_package(Qt6 COMPONENTS Widgets)

add_executable(${target_name} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(${target_name} PRIVATE RenderingPlatform Qt6::Widgets)
add_dependencies(${target_name} RenderingPlatform)
