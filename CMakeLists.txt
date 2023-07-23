cmake_minimum_required(VERSION 3.21)
project(DSGO LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

include_directories(include)

add_executable(DSGO 
	src/main.cpp
	src/Template.cpp
	include/Template.h
	src/Game.cpp
	include/Game.h)

target_link_libraries(DSGO PRIVATE sfml-graphics)
target_compile_features(DSGO PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET DSGO POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:DSGO> $<TARGET_FILE_DIR:DSGO> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS DSGO)
