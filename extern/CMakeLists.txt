cmake_minimum_required(VERSION 3.1)
project(FileWatcher)

file(GLOB_RECURSE SOURCE_FILES "simplefilewatcher/source/*.cc" "simplefilewatcher/source/*.cpp")

file(GLOB_RECURSE HEADER_FILES "simplefilewatcher/include/*.hh" "simplefilewatcher/include/*.h")

if(MSVC)
    set(FILEWATCHER_LINK_TYPE STATIC)
else()
    set(FILEWATCHER_LINK_TYPE SHARED)
endif()

add_library(fileWatcher ${FILEWATCHER_LINK_TYPE} ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(fileWatcher PUBLIC
    simplefilewatcher/include/
)
