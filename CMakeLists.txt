cmake_minimum_required(VERSION 3.0.0)
project(webscapper_cpp VERSION 0.1.0)

include(CTest)
enable_testing()
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


find_package(CURL REQUIRED)
find_package(LibXml2 REQUIRED)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE ${CURL_LIBRARIES})
target_include_directories(main PRIVATE ${CURL_INCLUDE_DIRS})

target_include_directories(main PRIVATE ${LIBXML2_INCLUDE_DIR})
target_link_libraries(main PRIVATE ${LIBXML2_LIBRARIES})