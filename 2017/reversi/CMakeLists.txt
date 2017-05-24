cmake_minimum_required(VERSION 3.7)
project(reversi)

find_package (Threads)
set(CMAKE_CXX_STANDARD 14)
add_definitions(-DLOCAL)

if(MSVC)
    # Force to always compile with W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    # Update if necessary
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

set(SOURCE_FILES MyClient.cpp Client.h SimpleStrategy.h StrategyNMoves.h Strategy.h)
add_executable(reversi ${SOURCE_FILES})
target_link_libraries(reversi ${CMAKE_THREAD_LIBS_INIT})