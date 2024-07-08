set(target_name RenderingCore)

file(GLOB SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_library(${target_name} ${JELLY_LIB_TYPE} ${SRCS})

target_link_libraries(${target_name} PUBLIC CommonCore CommonData RenderingShader ${Vulkan_LIBRARIES})

target_include_directories(${target_name} PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

target_include_directories(${target_name} PRIVATE ${PROJECT_SOURCE_DIR}/External)
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
