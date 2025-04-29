cmake_minimum_required(VERSION 3.16)
project(VirtualPiano LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set as Windows GUI application
set(CMAKE_WIN32_EXECUTABLE ON)

# Find Qt packages
find_package(Qt6 REQUIRED COMPONENTS 
    Core
    Gui
    Widgets
    Multimedia
)

# Add executable
add_executable(VirtualPiano WIN32
    mainPiano.cpp
    Piano.cpp
    Piano.hpp
    Pianokey.cpp
    Pianokey.hpp
    Song.cpp
    Song.hpp
    SongPlayer.cpp
    SongPlayer.hpp
)

# Link Qt libraries
target_link_libraries(VirtualPiano PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Multimedia
)

# Include directories
target_include_directories(VirtualPiano PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
