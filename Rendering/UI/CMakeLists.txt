set(target_name RenderingUI)

file(GLOB SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_library(${target_name} ${JELLY_LIB_TYPE} ${SRCS})
add_dependencies(${target_name} Common)

target_link_libraries(${target_name} PUBLIC Common)

target_include_directories(${target_name} PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

add_library(${PROJECT_NAME}::${target_name} ALIAS ${target_name})        

set_target_properties(${target_name} PROPERTIES PUBLIC_HEADER "${HEADERS}")

export(TARGETS ${target_name} NAMESPACE ${PROJECT_NAME}:: FILE ${PROJECT_NAME}Targets.cmake)

install(
    TARGETS ${target_name}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include
)
