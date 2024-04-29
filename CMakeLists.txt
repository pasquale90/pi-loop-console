cmake_minimum_required(VERSION 3.10)

project(piloop)

set(CMAKE_CXX_STANDARD 11)

SET (CMAKE_CXX_FLAGS "-D DEV")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pedantic -Wall -Wextra -Wno-error")

set (INCLUDE_DIRECTORIES
    ./include
    /usr/include
    /usr/include/jsoncpp/
    /usr/lib/x86_64-linux-gnu/
    )
    
set(SOURCE_FILES
    # src/audiotrack.cpp  
    # src/channel.cpp  
    # src/handshake.cpp  
    src/main.cpp  
    src/config.cpp
    src/menu.cpp
    src/interface.cpp
    src/keyboard.cpp
    # src/miditrack.cpp  
    # src/route.cpp  
    # src/server.cpp  
    # src/session.cpp  
    # src/track.cpp
)

include(CMakePrintHelpers)
cmake_print_variables(INCLUDE_DIRECTORIES)
set(LIBRARIES
    jack
    pthread
    jsoncpp
    evdev
)

add_executable(piloop ${SOURCE_FILES})

target_include_directories(piloop PRIVATE ${INCLUDE_DIRECTORIES})

target_link_libraries(piloop ${LIBRARIES})