set(target_name Common)

file(GLOB SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(${target_name} ${JELLY_LIB_TYPE} ${SRCS})
target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${target_name} PRIVATE ${PROJECT_SOURCE_DIR}/External)
