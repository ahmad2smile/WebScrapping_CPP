cmake_minimum_required(VERSION 3.0.0)
project(webscapper_cpp VERSION 0.1.0)

include(CTest)
enable_testing()
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


find_package(CURL REQUIRED)

add_executable(webscapper_cpp main.cpp)

target_link_libraries(webscapper_cpp PRIVATE ${CURL_LIBRARIES})
target_include_directories(webscapper_cpp PRIVATE ${CURL_INCLUDE_DIRS})