set(source_files
    assignment_1.c
    assignment_2.c
    assignment_3.c
    assignment_4.c
)

set(link_with_utils
    assignment_1
    assignment_2
)

# Create executables
make_executable(SOURCE ${source_files})

# Link with libraries
link_executable(TARGETS ${link_with_utils} LIB utils)

# Test
# assignment_1
do_test(NAME assignment_1 NO 1 ARGS "Hello World" EXPECTED "Input: Hello World\n")
do_test(NAME assignment_1 NO 2 ARGS "" EXPECTED "Usage: ./assignment_1 STR\n")
do_test(NAME assignment_1 NO 3 ARGS "0" EXPECTED "Number: 0\n")
do_test(NAME assignment_1 NO 4 ARGS "-1" EXPECTED "Input: -1\n")
do_test(NAME assignment_1 NO 5 ARGS "987" EXPECTED "Number: 987\n")
do_test(NAME assignment_1 NO 6 ARGS "987ab" EXPECTED "Input: 987ab\n")

# assignment_2
do_test(NAME assignment_2 NO 1 ARGS "Hello World" "0" EXPECTED "Position 0 in 'Hello World' is H\n")
do_test(NAME assignment_2 NO 2 ARGS "Hello World" "1" EXPECTED "Position 1 in 'Hello World' is e\n")
do_test(NAME assignment_2 NO 3 ARGS "Hello World" "5" EXPECTED "Position 5 in 'Hello World' is  \n")
do_test(NAME assignment_2 NO 4 ARGS "Hello World" "10" EXPECTED "Position 10 in 'Hello World' is d\n")
do_test(NAME assignment_2 NO 5 ARGS "Hello World" "11" EXPECTED "Error when finding position 11 of 'Hello World' with length 11\n")
do_test(NAME assignment_2 NO 6 ARGS "Hello World" "-1" EXPECTED "Could not capture the position from -1\n")
do_test(NAME assignment_2 NO 7 ARGS "Hello World" "a" EXPECTED "Could not capture the position from a\n")

# assignment_3
do_test(NAME assignment_3 NO 1 ARGS "" EXPECTED "Overflow detected when using short\n20000 \\+ 20000 = -25536\n9223372036854775806 \\+ 1 = 9223372036854775807\n")

# assignment_4
do_test(NAME assignment_4 NO 1 ARGS "foo" "foo" EXPECTED "Strings are the same\n")
do_test(NAME assignment_4 NO 3 ARGS "foo" "fo" EXPECTED "Strings are NOT the same\n")
do_test(NAME assignment_4 NO 4 ARGS "foo" "fo " EXPECTED "Strings are NOT the same\n")
do_test(NAME assignment_4 NO 5 ARGS "foo" "bar" EXPECTED "Strings are NOT the same\n")
do_test(NAME assignment_4 NO 6 ARGS "Hello World" "Hello World" EXPECTED "Strings are the same\n")
do_test(NAME assignment_4 NO 7 ARGS "Hello World" "Hello Worl" EXPECTED "Strings are NOT the same\n")
do_test(NAME assignment_4 NO 8 ARGS "Hello World" "hello world" EXPECTED "Strings are NOT the same\n")
