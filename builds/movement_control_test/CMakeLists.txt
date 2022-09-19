cmake_minimum_required(VERSION 3.12.4)

if(NOT CMAKE_VERSION VERSION_LESS 3.0)
    cmake_policy(SET CMP0048 NEW)
endif()

project(PubSub)

# Find requirements
#if(NOT fastcdr_FOUND)
#    find_package(fastcdr REQUIRED)
#endif()

#if(NOT fastrtps_FOUND)
#    find_package(fastrtps REQUIRED)
#endif()

# Set C++11
include(CheckCXXCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG OR
        CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    check_cxx_compiler_flag(-std=c++11 SUPPORTS_CXX11)
    if(SUPPORTS_CXX11)
        add_compile_options(-std=c++11)
    else()
        message(FATAL_ERROR "Compiler doesn't support C++11")
    endif()
endif()

# include the HelloWorld Topic files
message(STATUS "Configuring publisher/subscriber example...")
file(GLOB Message_SOURCES_CXX "../../messages/*/*.cxx")
set(move_command_helper_SOURCE_EXE 
               ../../src/common/xmlParser/tinyxml2/tinyxml2.cpp
               ../../src/base/Worker.cpp
               ../../src/base/Node.cpp
               ../../src/mains/MoveCommandHelperMain.cpp 
               ../../src/nodes/HelperNodes/MoveCommandHelper/MoveCommandHelper.cpp)

set(movement_control_SOURCE_EXE 
               ../../src/mains/MovementControlMain.cpp 
               ../../src/base/Worker.cpp
               ../../src/base/Node.cpp
               ../../src/common/xmlParser/tinyxml2/tinyxml2.cpp
               ../../src/common/HardwareInterface/SerialInterface.cpp
               ../../src/nodes/ModeController/ModeController.cpp
               ../../src/nodes/MovementGenerator/MovementGenerator.cpp
               ../../src/nodes/MecanumMotorController/MecanumMotorController.cpp)


# add all source files needed to build the publisher executable, and links the executable and the library together
add_executable(MoveCommandHelper ${move_command_helper_SOURCE_EXE} ${Message_SOURCES_CXX})
target_link_libraries(MoveCommandHelper fastrtps fastcdr ncurses)

add_executable(MovementControl ${movement_control_SOURCE_EXE} ${Message_SOURCES_CXX})
target_link_libraries(MovementControl fastrtps fastcdr ncurses)




# add all sources and links for subscriber executable, same as above
##target_link_libraries(DDSSubscriber fastrtps fastcdr)