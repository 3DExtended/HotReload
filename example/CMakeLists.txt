cmake_minimum_required(VERSION 3.1)
project(Example)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin/")

file(GLOB_RECURSE SOURCE_FILES "*.cc" "*.cpp")

add_executable(Example ${SOURCE_FILES})

TARGET_INCLUDE_DIRECTORIES(
	Example PRIVATE
	"."
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/"
)

target_link_libraries(
	Example
    PUBLIC
	hotReload
    fileWatcher
)




