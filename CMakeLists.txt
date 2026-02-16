cmake_minimum_required(VERSION 3.30)

project (bit_array C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON) 

add_library(bit_array STATIC src/bit_array.c src/bit_array.h)

# Test executables 
add_executable(basic_test tests/basic_test.c)
target_include_directories(basic_test PRIVATE src)
target_link_libraries(basic_test PRIVATE bit_array)

add_executable(12_year_span tests/12_year_span.c)
target_include_directories(12_year_span PRIVATE src)
target_link_libraries(12_year_span PRIVATE bit_array)

add_executable(intersection tests/intersection.c)
target_include_directories(intersection PRIVATE src)
target_link_libraries(intersection PRIVATE bit_array)
