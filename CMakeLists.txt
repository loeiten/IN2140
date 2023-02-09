# Build with
# rm -rf build
# mkdir build
# cd build
# cmake -G Ninja ..  # Creates make files
# cmake --build . -j 4 # Calls make using 4 threads
# ctest -V  # Calls the tests

# See also
# https://www.jetbrains.com/help/clion/quick-cmake-tutorial.html
# http://www.aosabook.org/en/cmake.html

cmake_minimum_required(VERSION 3.15)

# Set the project name
# Set the version of the project
project(IN2140 VERSION 1.0)

# Specify the C standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

# Turn on compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Specify verbose output
# You can also get the commands called by
# cmake --build . -v
# set(CMAKE_VERBOSE_MAKEFILE on)

# Set options
option(IWYU "Build with Include What You Use" ON)
# Sanitizers, see https://github.com/google/sanitizers for details
option(ADDRESS_SANITIZE "Use address sanitizer" ON)
option(UNDEFINED_SANITIZE "Use undefined behavior sanitizer" ON)
option(MEMORY_SANITIZE "Use memory sanitizer" OFF)
option(THREAD_SANITIZE "Use thread sanitizer" OFF)

if(IWYU)
   set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "include-what-you-use;-Xiwyu;--error;-Xiwyu;--mapping_file=${CMAKE_CURRENT_LIST_DIR}/.linters/iwyu.imp")
endif()

# include(CTest) automatically calls enable_testing()
# enable_testing()
include(CTest)

# Define a function to check the output of a test
function(do_test name target input expected)
   add_test(NAME ${name} COMMAND sh -c "echo ${input} | ./${target}")
   # The output must match this regex for the test to pass
   # For other properties check
   # https://cmake.org/cmake/help/latest/manual/cmake-properties.7.html#test-properties
   set_tests_properties(${name}
   PROPERTIES PASS_REGULAR_EXPRESSION "^${expected}$"
   )
endfunction(do_named_test)

# Enable sanitizers
if(ADDRESS_SANITIZE)
   add_compile_options(-fsanitize=address)
   add_link_options(-fsanitize=address)
endif()
if(UNDEFINED_SANITIZE)
   add_compile_options(-fsanitize=undefined)
   add_link_options(-fsanitize=undefined)
endif()
if(MEMORY_SANITIZE)
   add_compile_options(-fsanitize=memory)
   add_link_options(-fsanitize=memory)
endif()
if(THREAD_SANITIZE)
   add_compile_options(-fsanitize=thread)
   add_link_options(-fsanitize=thread)
endif()

add_subdirectory(programming_assignments)