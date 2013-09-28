project(vanilla)
cmake_minimum_required(VERSION 2.8)

add_definitions("-std=c++11")

include_directories(./include)
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
target_link_libraries(vanilla ffi dl gmp)

install(TARGETS vanilla RUNTIME DESTINATION bin)