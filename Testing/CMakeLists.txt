if(WIN32 AND MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /IGNORE:4099")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd /IGNORE:4099")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:msvcrtd")
endif()

set(target_name "Testing")

file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(${target_name} ${SRCS})
add_dependencies(${target_name} CommonCore GTest::gtest GTest::gmock)

target_link_libraries(${target_name} PRIVATE CommonCore)
target_link_libraries(${target_name} PRIVATE GTest::gtest GTest::gmock)

add_test(NAME JellyUnitTest COMMAND ${target_name})
