cmake_minimum_required(VERSION 3.10)

project(piloop)

set(CMAKE_CXX_STANDARD 11)

# option(DEV "development using keyboards" OFF)
# option(REL "release using gpio-pin connected buttons" ON)
set(MODE "REL")

SET (CMAKE_CXX_FLAGS "-D  ${MODE} -D K6") 

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pedantic -Wall -Wextra -Wno-error")

set (INCLUDE_DIRECTORIES
    ./include
    ./external
    /usr/include
    /usr/include/jsoncpp/
    /usr/lib/x86_64-linux-gnu/
    )

set(SOURCE_FILES
    src/audioserver.cpp  
    src/handshake.cpp  
    src/main.cpp  
    src/config.cpp
    src/menu.cpp
    src/piloop.cpp
    src/interface.cpp
    src/session.cpp
    src/effects.cpp
    src/monitor.cpp
    src/mixer.cpp
    src/metronome.cpp
    src/channel.cpp
    src/looper.cpp
)

if("${MODE}" STREQUAL "DEV")
    list(APPEND SOURCE_FILES "src/keyboard.cpp")
elseif("${MODE}" STREQUAL "REL")
    list(APPEND SOURCE_FILES "src/gpio/buttons.cpp")
else()
    message(FATAL_ERROR "MODE variable is set with wrong value. Set as REL for release and DEV for dev")
endif()


include(CMakePrintHelpers)
cmake_print_variables(INCLUDE_DIRECTORIES)
set(LIBRARIES
    jack
    jackserver
    jacknet
    pthread
    jsoncpp
    wiringPi
)

add_executable(piloop ${SOURCE_FILES})

target_include_directories(piloop PRIVATE ${INCLUDE_DIRECTORIES})

target_link_libraries(piloop ${LIBRARIES})
