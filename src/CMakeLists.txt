#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.6)
#Name your project here
project(xplhallite)
set (xplhallite_VERSION_MAJOR 1)
set (xplhallite_VERSION_MINOR 1)


# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/projconfig.h.in"
  "${PROJECT_BINARY_DIR}/projconfig.h"
  )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# include_directories ("${PROJECT_SOURCE_DIR}/pthreadcc")
# add_subdirectory (pthreadcc)
# include_directories ("${PROJECT_SOURCE_DIR}/socketcc")
# add_subdirectory (socketcc)
# include_directories ("${PROJECT_SOURCE_DIR}/xPLLib")
# add_subdirectory (xPLLib)
include_directories ("${PROJECT_SOURCE_DIR}/pugixml")
add_subdirectory (pugixml)
include_directories ("${PROJECT_SOURCE_DIR}/xPLsdk/src")
add_subdirectory (xPLsdk/src)

find_package (Threads)


find_library(POCO_FOUNDATION PocoFoundation)
find_library(POCO_NET PocoNet)
find_library(POCO_XML PocoXML)
find_library(POCO_UTIL PocoUtil)

#Sends the -std=c99 flag to the gcc compiler
#add_definitions(-std=c99)
#This tells CMake to fib.c and name it fibonacci
add_executable(xplhallite Determinator.cpp XPLAction.cpp XPLCondition.cpp XPLParser.cpp XPLRuleManager.cpp LogAction.cpp TimeCondition.cpp DeterminatorEnvironment.cpp XHCPDispatcher.cpp XPLHal.cpp GlobalManager.cpp GlobalAction.cpp GlobalCondition.cpp XHCPServerConnection.cpp determinator-main.cpp)
# target_link_libraries (xplhallite pthreadcc)
# target_link_libraries (xplhallite socketcc)
# target_link_libraries (xplhallite xPLLib)
target_link_libraries (xplhallite pugixml)
target_link_libraries (xplhallite xplsdk)
target_link_libraries (xplhallite ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(xplhallite ${POCO_FOUNDATION} ${POCO_NET} ${POCO_XML} ${POCO_UTIL})
