##---------------------------------------------------------------------------------------------------------------------
##  FASTCOM
##---------------------------------------------------------------------------------------------------------------------
##  Copyright 2019 - Pablo Ramon Soria (a.k.a. Bardo91) 
##---------------------------------------------------------------------------------------------------------------------
##  Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
##  and associated documentation files (the "Software"), to deal in the Software without restriction, 
##  including without limitation the rights to use, copy, modify, merge, publish, distribute, 
##  sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
##  furnished to do so, subject to the following conditions:
##
##  The above copyright notice and this permission notice shall be included in all copies or substantial 
##  portions of the Software.
##
##  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
##  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
##  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES 
##  OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
##  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
##---------------------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
PROJECT(fastcom VERSION 1.2.0)


#########################################
######			General config	   ######
#########################################

option(BUILD_EXAMPLES "Compile examples" ON)
option(BUILD_TESTS "Prepare tests" OFF)

if(UNIX)
	add_definitions(-std=c++11 -pthread -lpthread)
elseif(WIN32)
	set(Boost_USE_STATIC_LIBS ON)
endif()


#########################################
######			Dependencies       ######
#########################################
set(Boost_USE_MULTITHREAD ON)

find_package(Boost REQUIRED COMPONENTS system thread regex)

find_package(OpenCV)


#########################################
######			Library	   ######
#########################################
file(GLOB_RECURSE FASTCOM_HEADERS "./include/*.h" "./include/*.inl")
file(GLOB_RECURSE FASTCOM_SOURCES "./src/*.cpp")

if(UNIX)
	add_library(${PROJECT_NAME} SHARED ${FASTCOM_SOURCES} ${FASTCOM_HEADERS})
elseif(WIN32)
	add_library(${PROJECT_NAME} STATIC ${FASTCOM_SOURCES} ${FASTCOM_HEADERS})
endif()
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if(UNIX)
	target_link_libraries(${PROJECT_NAME} LINK_PUBLIC pthread atomic)
elseif(WIN32)
	target_compile_options(${PROJECT_NAME} PUBLIC "/EHsc")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${Boost_LIBRARIES})

if(OpenCV_FOUND)
  target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${OpenCV_LIBRARIES})
  target_compile_definitions(${PROJECT_NAME} PUBLIC FASTCOM_HAS_OPENCV)
endif(OpenCV_FOUND)


# Find python latest version
find_package (Python3)
if(${Python3_FOUND})
	if(UNIX)
		set(PYTHON_BINARY python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR})
	elseif(WIN32)
		set(PYTHON_BINARY python)
	endif()
endif()

#########################################
######			Documentation	   ######
#########################################
find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_target(	doc_fastcom
						${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
						WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
						COMMENT "Generating API documentation with Doxygen" VERBATIM )
endif(DOXYGEN_FOUND)


#########################################
######			Examples	   ######
#########################################
if(${BUILD_EXAMPLES})
	add_subdirectory(examples)
endif()

#########################################
######			Test				######
#########################################
if(${BUILD_TESTS})
	enable_testing()
	add_subdirectory(tests)
endif()

#########################################
######			INSTALL 		   ######
#########################################
include(CMakePackageConfigHelpers)


if(${Python3_FOUND})
	## INSTALLATION Python Library
	add_custom_target(  install_python 
						${PYTHON_BINARY} ${CMAKE_CURRENT_SOURCE_DIR}/python/setup.py  install --user
						WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python )
endif()

## INSTALLATION C++ Library
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif() 

install(TARGETS fastcom
        EXPORT fastcomTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION ${CMAKE_INSTALL_PREFIX}
        )

write_basic_package_version_file(
  fastcomConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )


# This installs the include folder
install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX} FILES_MATCHING PATTERN "*.h")
install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX} FILES_MATCHING PATTERN "*.inl")

export(TARGETS fastcom NAMESPACE fastcom:: FILE fastcom.cmake)

# This generates fastcomTargets.cmake
install(EXPORT fastcomTargets
    FILE fastcomTargets.cmake
    NAMESPACE fastcom::
    DESTINATION lib/cmake/fastcom
    )

configure_file(fastcomConfig.cmake.in fastcomConfig.cmake @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/fastcomConfig.cmake"
        DESTINATION lib/cmake/fastcom
        )

# uninstall target 
if(NOT TARGET uninstall) 
  configure_file( 
      "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in" 
      "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" 
      IMMEDIATE @ONLY) 
 
  add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake) 
endif() 


 
