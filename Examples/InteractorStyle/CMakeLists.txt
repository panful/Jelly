set(target_name example_interactorStyle)
add_executable(${target_name} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(${target_name} PRIVATE RenderingPlatform RenderingInteraction)
add_dependencies(${target_name} RenderingPlatform RenderingInteraction)
