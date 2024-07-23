set(target_name example_QTProject)

find_package(Qt6 COMPONENTS Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC OFF)

add_executable(${target_name} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/MyWindow.cpp)
target_link_libraries(${target_name} PRIVATE RenderingPlatform Qt6::Widgets)
add_dependencies(${target_name} RenderingPlatform)
