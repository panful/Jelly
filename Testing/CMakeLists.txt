set(target_name "Testing")

file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_executable(${target_name} ${SRCS})
add_dependencies(${target_name} CommonCore CommonData RenderingCore RenderingUI RenderingPlatform RenderingShader GTest::gtest GTest::gmock)

target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${target_name} PRIVATE CommonCore CommonData RenderingCore RenderingUI RenderingPlatform RenderingShader)
target_link_libraries(${target_name} PRIVATE GTest::gtest GTest::gmock)

target_compile_definitions(${target_name} PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/TestData")

add_test(NAME JellyUnitTest COMMAND ${target_name})
