#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.6)
#Name your project here
project(xplhallite)
set (xplhallite_VERSION_MAJOR 1)
set (xplhallite_VERSION_MINOR 2)

#we can ask do this to try to statically link
SET(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
add_definitions(-DPOCO_STATIC)


# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/projconfig.h.in"
  "${PROJECT_BINARY_DIR}/projconfig.h"
  )

include_directories ("${PROJECT_SOURCE_DIR}")

include_directories("${PROJECT_BINARY_DIR}")


include_directories ("${PROJECT_SOURCE_DIR}/pugixml")
add_subdirectory (pugixml)
include_directories ("${PROJECT_SOURCE_DIR}/xPLsdk/src")
add_subdirectory (xPLsdk/src)

add_subdirectory(conditions)
add_subdirectory(actions)

find_package (Threads)

add_definitions(-D_DEBUG)

find_library(POCO_FOUNDATION PocoFoundation)
find_library(POCO_NET PocoNet)
find_library(POCO_XML PocoXML)
find_library(POCO_UTIL PocoUtil)

#add_definitions(-std=c99)
add_executable(xplhallite Determinator.cpp  XPLParser.cpp XPLRuleManager.cpp  DeterminatorEnvironment.cpp XHCPDispatcher.cpp XPLHal.cpp GlobalManager.cpp XHCPServerConnection.cpp determinator-main.cpp)

#the old XML library, in-tree
target_link_libraries (xplhallite pugixml)
#our XPL library
target_link_libraries (xplhallite xplsdk)
#internal subdirs
target_link_libraries(xplhallite conditions)
target_link_libraries(xplhallite actions)

target_link_libraries (xplhallite ${CMAKE_THREAD_LIBS_INIT})
#we use POCO for just about everything.
target_link_libraries(xplhallite ${POCO_FOUNDATION} ${POCO_NET} ${POCO_XML} ${POCO_UTIL})



# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)



