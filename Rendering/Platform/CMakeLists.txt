set(target_name RenderingPlatform)

if(USE_GLFW_WINDOW)
    set(SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/GLFWHelper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/InteractorGLFW.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/WindowGLFW.cpp
    )

    set(HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/GLFWHelper.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/InteractorGLFW.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/WindowGLFW.h
    )

    set(DEPENDENCIES glfw)

    set(LIBRARIES glfw)
endif(USE_GLFW_WINDOW)

if(USE_QT_WINDOW)
    find_package(Qt6 COMPONENTS Widgets)

    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC OFF)
    set(CMAKE_AUTOUIC OFF)

    set(SRCS ${SRCS}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/WindowQT.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/InteractorQT.cpp
    )

    set(HEADERS ${HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include/WindowQT.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/InteractorQT.h
    )

    set(LIBRARIES ${LIBRARIES} Qt6::Widgets)
endif(USE_QT_WINDOW)

add_library(${target_name} ${JELLY_LIB_TYPE} ${SRCS})
add_dependencies(${target_name} RenderingInteraction ${DEPENDENCIES})

target_link_libraries(${target_name} PUBLIC RenderingInteraction)
target_link_libraries(${target_name} PRIVATE ${LIBRARIES})

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
