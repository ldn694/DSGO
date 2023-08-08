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
	src/Color.cpp
	include/Color.h
	src/Game.cpp
	include/Game.h
	src/BackButton.cpp
	include/BackButton.h
	src/Box.cpp
	include/Box.h
	src/MultipleChoice.cpp
	include/MultipleChoice.h
	src/MyShape.cpp
	include/MyShape.h
	src/ReadFromFile.cpp
	include/ReadFromFile.h
	src/TriangleButton.cpp
	include/TriangleButton.h
	src/TypingBox.cpp
	include/TypingBox.h
	src/IngameSettings.cpp
	include/IngameSettings.h
	src/MediaControl.cpp
	include/MediaControl.h
	src/PlayButton.cpp
	include/PlayButton.h
	src/Scrubber.cpp
	include/Scrubber.h
	src/Switch.cpp
	include/Switch.h
	src/SlideBar.cpp
	include/SlideBar.h
	src/CodeBox.cpp
	include/CodeBox.h
	src/CodeVisualizer.cpp
	include/CodeVisualizer.h
	src/Stage.cpp
	include/Stage.h
	src/HashCell.cpp
	include/HashCell.h
	src/HashTable.cpp
	include/HashTable.h
	src/HashTableStage.cpp
	include/HashTableStage.h
	src/AVLNode.cpp
	include/AVLNode.h
	src/AVLGraph.cpp
	include/AVLGraph.h
	src/AVLStage.cpp
	include/AVLStage.h
	src/HeapGraph.cpp
	include/HeapGraph.h
	src/HeapStage.cpp
	include/HeapStage.h
	src/BTreeNode.cpp
	include/BTreeNode.h
	src/BTreeGroup.cpp
	include/BTreeGroup.h
	src/BTreeGraph.cpp
	include/BTreeGraph.h
	src/BTreeStage.cpp
	include/BTreeStage.h
	)

target_link_libraries(DSGO PRIVATE sfml-graphics)
target_compile_features(DSGO PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET DSGO POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:DSGO> $<TARGET_FILE_DIR:DSGO> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS DSGO)
