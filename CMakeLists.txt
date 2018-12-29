cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
PROJECT(fastcom LANGUAGES CXX VERSION 0.1)

add_definitions(-std=c++11 -pthread -lpthread)

SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE(Boost REQUIRED COMPONENTS system thread regex)
find_package(OpenCV REQUIRED)
IF(Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
  LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF(Boost_FOUND)

SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY} ${OpenCV_LIBRARIES} pthread)

file(GLOB_RECURSE FASTCOM_HEADERS "./include/*.h" "./include/*.inl")
file(GLOB_RECURSE FASTCOM_SOURCES "./include/*.cpp")

add_library(${PROJECT_NAME} SHARED ${FASTCOM_SOURCES} ${FASTCOM_HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${USED_LIBS})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)

# EXAMPLES

add_executable(example_sender "examples/sender.cpp")
target_include_directories(example_sender PRIVATE "./include")
target_link_libraries(example_sender LINK_PUBLIC ${USED_LIBS})


add_executable(example_receiver "examples/receiver.cpp")
target_include_directories(example_receiver PRIVATE "./include")
target_link_libraries(example_receiver LINK_PUBLIC ${USED_LIBS})

