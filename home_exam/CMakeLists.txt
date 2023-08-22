# With Makefile
# https://stackoverflow.com/a/50204961/2786884
# https://stackoverflow.com/a/44505200/2786884
# With a static library
add_custom_command(OUTPUT home_exam_all
                  COMMAND make
                  DEPENDS s3_assignment_6.c my_math
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
# FIXME: Add tests as they are created here
add_custom_target(test_foo ALL DEPENDS home_exam_all)
do_test(NAME test_foo NO 1 ARGS EXPECTED "Success\n")
