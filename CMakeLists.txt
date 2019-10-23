cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
PROJECT(fastcom VERSION 1.0)

if(UNIX)
	add_definitions(-std=c++11 -pthread -lpthread)
elseif(WIN32)
	set(Boost_USE_STATIC_LIBS ON)
endif()

set(Boost_USE_MULTITHREAD ON)

find_package(Boost REQUIRED COMPONENTS system thread regex)

find_package(OpenCV)

file(GLOB_RECURSE FASTCOM_HEADERS "./include/*.h" "./include/*.inl")
file(GLOB_RECURSE FASTCOM_SOURCES "./src/*.cpp")

if(UNIX)
	add_library(${PROJECT_NAME} SHARED ${FASTCOM_SOURCES} ${FASTCOM_HEADERS})
elseif(WIN32)
	add_library(${PROJECT_NAME} STATIC ${FASTCOM_SOURCES} ${FASTCOM_HEADERS})
endif()
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)

target_include_directories(${PROJECT_NAME} PRIVATE "./include")
if(UNIX)
	target_link_libraries(${PROJECT_NAME} LINK_PUBLIC pthread atomic)
elseif(WIN32)
	SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES CMAKE_CXX_FLAGS  "/EHsc")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${Boost_LIBRARIES})

if(OpenCV_FOUND)
  target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${OpenCV_LIBRARIES})
  target_compile_definitions(${PROJECT_NAME} PUBLIC FASTCOM_HAS_OPENCV)
endif(OpenCV_FOUND)


# Find python latest version
find_package (Python3 COMPONENTS Interpreter Development REQUIRED)
set(PYTHON_BINARY python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR})

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

add_executable(example_sender "examples/sender.cpp")
target_include_directories(example_sender PRIVATE "./include")
target_link_libraries(example_sender LINK_PUBLIC fastcom)

add_executable(example_receiver "examples/receiver.cpp")
target_include_directories(example_receiver PRIVATE "./include")
target_link_libraries(example_receiver LINK_PUBLIC fastcom)


add_executable(example_sender_image "examples/sender_image.cpp")
target_include_directories(example_sender_image PRIVATE "./include")
target_link_libraries(example_sender_image LINK_PUBLIC fastcom)

add_executable(example_receiver_image "examples/receiver_image.cpp")
target_include_directories(example_receiver_image PRIVATE "./include")
target_link_libraries(example_receiver_image LINK_PUBLIC fastcom)

add_executable(example_destruction "examples/destruction.cpp")
target_include_directories(example_destruction PRIVATE "./include")
target_link_libraries(example_destruction LINK_PUBLIC fastcom)


#########################################
######			INSTALL 		   ######
#########################################
include(CMakePackageConfigHelpers)

## INSTALLATION Python Library
add_custom_target(  install_python 
                    ${PYTHON_BINARY} ${CMAKE_CURRENT_SOURCE_DIR}/python/setup.py  install --user
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python )

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


 
