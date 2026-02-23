cmake_minimum_required(VERSION 3.30)

project (bit_array C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON) 

add_library(bit_array STATIC src/bit_array.c src/bit_array.h)

# Test library 
add_library(test_utils INTERFACE tests/test_utils.h) 
target_include_directories(test_utils INTERFACE tests/)

# for pedantic behaviors
# target_compile_options(bit_array PUBLIC -Wall -Wextra -Wpedantic) 

# Add test functions 
function(add_test_exe name)
  add_executable(${name} tests/${name}.c)
  target_include_directories(${name} PRIVATE src/)
  target_link_libraries(${name} PRIVATE bit_array test_utils)
endfunction() 

add_test_exe(test_inputs)
add_test_exe(test_lifecycle)
add_test_exe(test_operations)
add_test_exe(test_queries)
add_test_exe(test_setters)
