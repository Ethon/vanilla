# Base project setup.
project(vanilla)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Switch to C++11 mode.
if(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    add_definitions("-std=c++11")
endif()

# Find required libraries.
find_package(LibFFI REQUIRED)
find_package(GMP REQUIRED)
find_package(Boost REQUIRED)

# Set OS specific libraries.
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(OS_LIBS "dl")
endif()

# Set include directories.
include_directories(
    ./include
    ${LIBFFI_INCLUDE_DIRS}
    ${GMP_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)

# Set libraries.
set(LIBS
    ${OS_LIBS}
    ${LIBFFI_LIBRARIES}
    ${GMP_LIBRARIES}
    ${Boost_LIBRARIES}
)

# Set definitions.
add_definitions("${LIBFFI_DEFINITIONS}")

add_executable(vanilla
    main.cpp
    src/scanner.cpp
    src/parsing.cpp
    
    src/ast_base.cpp
    src/expression_ast.cpp
    src/statement_ast.cpp
    
    src/context.cpp
    src/native_library_cache.cpp
    
    src/object.cpp
    src/none_object.cpp
    src/int_object.cpp
    src/float_object.cpp
    src/bool_object.cpp
    src/string_object.cpp
    src/array_object.cpp
    src/function_object.cpp
    src/native_function_object.cpp
    
    #src/gen/xml.cpp
)
target_link_libraries(vanilla ${LIBS})

install(TARGETS vanilla RUNTIME DESTINATION bin)