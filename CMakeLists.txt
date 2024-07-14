cmake_minimum_required(VERSION 3.10)

project(piloop)

set(CMAKE_CXX_STANDARD 11)


#manual parameters - set those manually
# option(PC "uses the computer based interface" OFF)
# option(RPI "uses gpio-pin based interface of the raspberry pi" ON)
set(MODE "PC")
set(AUDIO_INTERFACE "PCH") #K6/PCH/CODEC

SET (CMAKE_CXX_FLAGS "-D  ${MODE} -D ${AUDIO_INTERFACE}") 

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

include(CMakePrintHelpers)
cmake_print_variables(INCLUDE_DIRECTORIES)
set(LIBRARIES
    jack
    jackserver
    jacknet
    pthread
    jsoncpp
)

if("${MODE}" STREQUAL "PC")
    list(APPEND INCLUDE_DIRECTORIES "./include/computer")
    list(APPEND SOURCE_FILES "src/computer.cpp" "src/computer/keyboard.cpp" "src/computer/screen.cpp")
    list(APPEND LIBRARIES evdev)
elseif("${MODE}" STREQUAL "RPI")
    list(APPEND INCLUDE_DIRECTORIES "./include/gpio")
    list(APPEND SOURCE_FILES "src/gpio/buttons.cpp"  "src/gpio/leds.cpp" "src/gpio/potentiometers.cpp" )
    list(APPEND LIBRARIES wiringPi)
else()
    message(FATAL_ERROR "MODE variable is set with wrong value. Set as RPI for using the raspberry pi and PC for using the computer instead.")
endif()

add_executable(piloop ${SOURCE_FILES})

target_include_directories(piloop PRIVATE ${INCLUDE_DIRECTORIES})

target_link_libraries(piloop ${LIBRARIES})
