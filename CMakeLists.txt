cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
PROJECT(fastcom VERSION 0.1)

add_definitions(-std=c++11 -pthread -lpthread)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREAD ON)

find_package(Boost REQUIRED COMPONENTS system thread regex)

find_package(OpenCV)


file(GLOB_RECURSE FASTCOM_HEADERS "./include/*.h" "./include/*.inl")
file(GLOB_RECURSE FASTCOM_SOURCES "./src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${FASTCOM_SOURCES} ${FASTCOM_HEADERS})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)

target_include_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC pthread)

if(Boost_FOUND)
  target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})
ENDIF(Boost_FOUND)

if(OpenCV_FOUND)
  target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${OpenCV_LIBRARIES})
  target_compile_definitions(${PROJECT_NAME} PUBLIC FASTCOM_HAS_OPENCV)
endif(OpenCV_FOUND)


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

