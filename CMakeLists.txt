cmake_minimum_required (VERSION 3.12)

project("Jelly" VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /Zc:__cplusplus /utf-8 /EHsc /wd4251")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fexceptions")
endif(MSVC)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

option(BUILD_UNIT_TESTING "build the unit test" ON)
option(BUILD_EXAMPLES "build the examples" ON)
option(BUILD_SHARED_LIBRARIES "build shared libraries(dlls)" ON)
option(USE_GLFW_WINDOW "display using glfw dindow" ON)

find_package(Vulkan REQUIRED)

if(BUILD_SHARED_LIBRARIES)
    set(JELLY_LIB_TYPE SHARED)
    add_definitions(-DJELLY_SHARED_LIBRARY)
else()
    set(JELLY_LIB_TYPE STATIC)
endif(BUILD_SHARED_LIBRARIES)

add_subdirectory(Common)
add_subdirectory(Rendering)

if(USE_GLFW_WINDOW)
    add_subdirectory(External/glfw)
endif(USE_GLFW_WINDOW)

if(BUILD_UNIT_TESTING)
    enable_testing()
    set(INSTALL_GTEST OFF)
    add_subdirectory(External/googletest)
    add_subdirectory(Testing)
endif(BUILD_UNIT_TESTING)

if(BUILD_EXAMPLES)
    add_subdirectory(Examples)
endif(BUILD_EXAMPLES)

install(
    EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME}
)

set(INCLUDE_DIR include)
set(LIBRARIES_DIR lib)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${PROJECT_BINARY_DIR}/lib/cmake/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION 0.0.1
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
    ${PROJECT_BINARY_DIR}/lib/cmake/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
    PATH_VARS INCLUDE_DIR LIBRARIES_DIR
    INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}
)

install(
    FILES 
        ${PROJECT_BINARY_DIR}/lib/cmake/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake 
        ${PROJECT_BINARY_DIR}/lib/cmake/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION lib/cmake/${PROJECT_NAME}
)
