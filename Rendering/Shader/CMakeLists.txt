set(target_name RenderingShader)

file(GLOB SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

set(glslang_LIBS glslang SPIRV)

add_library(${target_name} ${JELLY_LIB_TYPE} ${SRCS})
add_dependencies(${target_name} CommonCore ${glslang_LIBS})

target_link_libraries(${target_name} PUBLIC CommonCore)
target_link_libraries(${target_name} PRIVATE ${glslang_LIBS})

target_include_directories(${target_name} PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

target_include_directories(${target_name} PUBLIC ${Vulkan_INCLUDE_DIR})

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
