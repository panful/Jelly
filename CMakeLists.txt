cmake_minimum_required (VERSION 3.12)

project("Jelly" VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /Zc:__cplusplus /utf-8 /EHsc")
endif(MSVC)
    
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

option(BUILD_UNIT_TESTING "build the unit test" ON)
option(BUILD_SHARED_LIBRARIES "build shared libraries(dlls)" ON)

if(BUILD_SHARED_LIBRARIES)
    set(JELLY_LIB_TYPE SHARED)
    add_definitions(-DJELLY_SHARED_LIBRARY)
else()
    set(JELLY_LIB_TYPE STATIC)
endif(BUILD_SHARED_LIBRARIES)

add_subdirectory(Common)

if(BUILD_UNIT_TESTING)
    enable_testing()
    add_subdirectory(External/googletest)
    add_subdirectory(Testing)
endif(BUILD_UNIT_TESTING)
