cmake_minimum_required(VERSION 3.1)
project(HotReload)

file(GLOB_RECURSE SOURCE_FILES "src/*.cc" "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "include/*.hh" "include/*.h")

if(MSVC)
    set(HOTRELOAD_LINK_TYPE STATIC)
else()
    set(HOTRELOAD_LINK_TYPE SHARED)
endif()

add_library(hotReload ${HOTRELOAD_LINK_TYPE} ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(hotReload PUBLIC
    include/
)

add_subdirectory("extern")
target_include_directories(hotReload PUBLIC
    extern/simplefilewatcher/include/
)
target_link_libraries(hotReload PUBLIC fileWatcher)

add_subdirectory("example")
