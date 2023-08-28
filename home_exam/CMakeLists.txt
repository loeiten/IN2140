# With Makefile
# https://stackoverflow.com/a/50204961/2786884
# https://stackoverflow.com/a/44505200/2786884
# With a static library
add_custom_command(OUTPUT home_exam_all
                  COMMAND make all
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(make_home_exam_all ALL DEPENDS home_exam_all)
# This command will run all tests as a single ctest test
# NOTE: The -s makes the command silent
add_test(NAME home_exam_run_tests COMMAND make -s run-tests WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
