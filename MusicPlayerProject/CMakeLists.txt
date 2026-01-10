cmake_minimum_required(VERSION 3.10)

project(MusicPlayerProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directory
include_directories(include)

# Source files
set(SOURCES
    src/MusicLibrary.cpp
    src/PlaybackQueue.cpp
    src/PlaybackHistory.cpp
    src/MusicPlayer.cpp
    src/SmartPlaylist.cpp
    main.cpp
)

# Create executable
add_executable(music_player ${SOURCES})
