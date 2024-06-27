set(target_name "Testing")

file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(${target_name} ${SRCS})
add_dependencies(${target_name} CommonCore GTest::gtest GTest::gmock)

target_link_libraries(${target_name} PRIVATE CommonCore RenderingShader)
target_link_libraries(${target_name} PRIVATE GTest::gtest GTest::gmock)

add_test(NAME JellyUnitTest COMMAND ${target_name})
