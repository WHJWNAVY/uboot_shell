cmake_minimum_required (VERSION 3.3.2)

project (USHELL C)

set(EXECUTABLE_NAME ushell)
set(CMAKE_BUILD_TYPE Release)

SET(CMAKE_C_FLAGS "-m32")

find_package(PkgConfig REQUIRED)
#pkg_check_modules(LIB_PTHREAD REQUIRED pthread)

file(GLOB USHELL_SRCS       ./source/*.c)
                                         
add_executable(${EXECUTABLE_NAME} ${USHELL_SRCS})

target_link_libraries(${EXECUTABLE_NAME} pthread curses)

target_include_directories(${EXECUTABLE_NAME} PUBLIC ${PROJECT_SOURCE_DIR}
                                          ${PROJECT_SOURCE_DIR}/include)

install(TARGETS ${EXECUTABLE_NAME}
        RUNTIME DESTINATION bin)