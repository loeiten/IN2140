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
set(CMAKE_BUILD_TYPE "Debug")

# Specify verbose output
# You can also get the commands called by
# cmake --build . -v
# set(CMAKE_VERBOSE_MAKEFILE on)

# Set options
option(IWYU "Build with Include What You Use" OFF)  # Prefer pre-commit
# Sanitizers, see https://github.com/google/sanitizers for details
option(ASAN "Use address sanitizer" ON)
option(UBSAN "Use undefined behavior sanitizer" ON)
option(MSAN "Use memory sanitizer" OFF)
option(TSAN "Use thread sanitizer" OFF)

if(IWYU)
   set(CMAKE_C_INCLUDE_WHAT_YOU_USE "include-what-you-use;-Xiwyu;--error;-Xiwyu;--mapping_file=${CMAKE_CURRENT_LIST_DIR}/.linters/iwyu.imp")
endif(IWYU)

# include(CTest) automatically calls enable_testing()
# enable_testing()
include(CTest)

# Make executables
function(make_executable)
   # For details of function parameters, see
   # https://stackoverflow.com/questions/23327687/how-to-write-a-cmake-function-with-more-than-one-parameter-groups
   cmake_parse_arguments(
       EXE_ARGS  # Prefix of output variables
       ""        # List of names of the boolean arguments (only defined ones will be true)
       ""        # List of names of mono-valued arguments
       "SOURCE"  # List of names of multi-valued arguments (output variables are lists)
       ${ARGN}   # Arguments of the function to parse, here we take the all original ones
   )
   # NOTE: If unparsed arguments remains here, they can be found in the
   #       variable EXE_ARGS_UNPARSED_ARGUMENTS
   foreach(file ${EXE_ARGS_SOURCE})
      get_filename_component(file_stem ${file} NAME_WE)
      add_executable(${file_stem} ${file})
  endforeach(file ${EXE_ARGS_SOURCE})
endfunction(make_executable)

# Link executables
function(link_executable)
   # For details of function parameters, see
   # https://stackoverflow.com/questions/23327687/how-to-write-a-cmake-function-with-more-than-one-parameter-groups
   cmake_parse_arguments(
       LINK_ARGS # Prefix of output variables
       ""        # List of names of the boolean arguments (only defined ones will be true)
       "LIB"     # List of names of mono-valued arguments
       "TARGETS" # List of names of multi-valued arguments (output variables are lists)
       ${ARGN}   # Arguments of the function to parse, here we take the all original ones
   )
   # NOTE: If unparsed arguments remains here, they can be found in the
   #       variable LINK_ARGS_UNPARSED_ARGUMENTS
   if(NOT LINK_ARGS_LIB)
       message(FATAL_ERROR "You must provide a lib to link with")
   endif(NOT LINK_ARGS_LIB)

   # Notice that linking happens after adding the executable
   # https://stackoverflow.com/a/39600062/2786884
   # Difference PUBLIC, PRIVATE and INTERFACE:
   # https://leimao.github.io/blog/CMake-Public-Private-Interface/
   # https://kubasejdak.com/modern-cmake-is-like-inheritance
   # NOTE: The location of LINK_ARGS_LIB is inferred
   foreach(executable ${LINK_ARGS_TARGETS})
      target_link_libraries(${executable} PRIVATE ${LINK_ARGS_LIB})
   endforeach(executable ${LINK_ARGS_TARGETS})
endfunction(link_executable)

# Define a function to check the output of a test
function(do_test)
   # For details of function parameters, see
   # https://stackoverflow.com/questions/23327687/how-to-write-a-cmake-function-with-more-than-one-parameter-groups
   cmake_parse_arguments(
       TEST_ARGS            # Prefix of output variables
       ""                   # List of names of the boolean arguments (only defined ones will be true)
       "NAME;NO;EXPECTED"   # List of names of mono-valued arguments
       "ARGS"               # List of names of multi-valued arguments (output variables are lists)
       ${ARGN}              # Arguments of the function to parse, here we take the all original ones
   )
   # NOTE: If unparsed arguments remains here, they can be found in the
   #       variable TEST_ARGS_UNPARSED_ARGUMENTS

   # Error handling
   if(NOT TEST_ARGS_NAME)
       message(FATAL_ERROR "You must provide a name to the executable")
   endif(NOT TEST_ARGS_NAME)
   if(NOT TEST_ARGS_NO)
       message(FATAL_ERROR "You must provide a test number")
   endif(NOT TEST_ARGS_NO)
   if(NOT TEST_ARGS_EXPECTED)
       message(FATAL_ERROR "You must provide an expected outcome")
   endif(NOT TEST_ARGS_EXPECTED)

   # Find path relative to this
   file(RELATIVE_PATH rel_path ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_LIST_DIR})
   string(REPLACE "/" "_" rel_path ${rel_path})
   set(full_name ${rel_path}_${TEST_ARGS_NAME}_${TEST_ARGS_NO})
   add_test(NAME ${full_name} COMMAND ./${TEST_ARGS_NAME} ${TEST_ARGS_ARGS})
   # The output must match this regex for the test to pass
   # For other properties check
   # https://cmake.org/cmake/help/latest/manual/cmake-properties.7.html#test-properties
   set_tests_properties(${full_name}
   PROPERTIES PASS_REGULAR_EXPRESSION "^${TEST_ARGS_EXPECTED}$"
   )
endfunction(do_test)

# Enable sanitizers
if(ASAN)
   add_compile_options(-fsanitize=address)
   add_link_options(-fsanitize=address)
endif(ASAN)
if(UBSAN)
   add_compile_options(-fsanitize=undefined)
   add_link_options(-fsanitize=undefined)
endif(UBSAN)
if(MSAN)
   add_compile_options(-fsanitize=memory)
   add_link_options(-fsanitize=memory)
endif(MSAN)
if(TSAN)
   add_compile_options(-fsanitize=thread)
   add_link_options(-fsanitize=thread)
endif(TSAN)

add_subdirectory(utils)
add_subdirectory(programming_assignments)
add_subdirectory(os_theory_assignments)
