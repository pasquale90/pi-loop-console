cmake_minimum_required(VERSION 3.10)

project(piloop)

set(CMAKE_CXX_STANDARD 11)

set(flag_variables 
    PILOOP_MODE
    AUDIO_DEVICE
    SAMPLE_RATE
    BUFFER_SIZE
    KEYBOARD_DEVICE
    I2C_BUS_ADDR 
)

message("PILOOP Configuration FLAGS:")
foreach(flag ${flag_variables})
    message(${flag} : $ENV{${flag}})
    set(${flag} $ENV{${flag}})
endforeach()

SET (USER_DEFINED_FLAGS 
"-D ${PILOOP_MODE} \
-D ${AUDIO_DEVICE} \
-D SAMPLE_RATE=${SAMPLE_RATE} \
-D BUFFER_SIZE=${BUFFER_SIZE}"
) 

set(CMAKE_CXX_FLAGS "-g -pedantic -Wall -Wextra -Wno-error ${USER_DEFINED_FLAGS}")

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

set(LIBRARIES
    jack
    jackserver
    jacknet
    pthread
    jsoncpp
)

if("${PILOOP_MODE}" STREQUAL "PC")
    list(APPEND INCLUDE_DIRECTORIES "./include/computer")
    list(APPEND SOURCE_FILES "src/computer.cpp" "src/computer/keyboard.cpp" "src/computer/screen.cpp")
    list(APPEND LIBRARIES evdev)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D KEYBOARD_DEVICE=${KEYBOARD_DEVICE}")
elseif("${PILOOP_MODE}" STREQUAL "RPI")
    list(APPEND INCLUDE_DIRECTORIES "./include/gpio")
    list(APPEND SOURCE_FILES "src/gpio/buttons.cpp"  "src/gpio/leds.cpp" "src/gpio/potentiometers.cpp" )
    list(APPEND LIBRARIES wiringPi)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D I2C_BUS_ADDR=${I2C_BUS_ADDR}")
else()
    message(FATAL_ERROR "MODE variable is set with wrong value. Set as RPI for using the raspberry pi and PC for using the computer instead.")
endif()



include(CMakePrintHelpers)
cmake_print_variables(INCLUDE_DIRECTORIES)
cmake_print_variables(SOURCE_FILES)
cmake_print_variables(LIBRARIES)
cmake_print_variables(USER_DEFINED_FLAGS)
cmake_print_variables(CMAKE_CXX_FLAGS)

add_executable(piloop ${SOURCE_FILES})

target_include_directories(piloop PRIVATE ${INCLUDE_DIRECTORIES})

target_link_libraries(piloop ${LIBRARIES})
