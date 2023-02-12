set(source_files
    s1_assignment_1.c
    s1_assignment_2.c
    s1_assignment_3.c
    s1_assignment_4.c
    s1_assignment_5.c
    s1_assignment_6.c
)

set(link_with_utils
    s1_assignment_1
    s1_assignment_2
)

# Create executables
make_executable(SOURCE ${source_files})

# Link with libraries
link_executable(TARGETS ${link_with_utils} LIB utils)

# Test
# assignment_1
do_test(NAME s1_assignment_1 NO 1 ARGS "Hello World" EXPECTED "Input: Hello World\n")
do_test(NAME s1_assignment_1 NO 2 ARGS "" EXPECTED "Usage: ./s1_assignment_1 STR\n")
do_test(NAME s1_assignment_1 NO 3 ARGS "0" EXPECTED "Number: 0\n")
do_test(NAME s1_assignment_1 NO 4 ARGS "-1" EXPECTED "Input: -1\n")
do_test(NAME s1_assignment_1 NO 5 ARGS "987" EXPECTED "Number: 987\n")
do_test(NAME s1_assignment_1 NO 6 ARGS "987ab" EXPECTED "Input: 987ab\n")

# assignment_2
do_test(NAME s1_assignment_2 NO 1 ARGS "Hello World" "0" EXPECTED "Position 0 in 'Hello World' is H\n")
do_test(NAME s1_assignment_2 NO 2 ARGS "Hello World" "1" EXPECTED "Position 1 in 'Hello World' is e\n")
do_test(NAME s1_assignment_2 NO 3 ARGS "Hello World" "5" EXPECTED "Position 5 in 'Hello World' is  \n")
do_test(NAME s1_assignment_2 NO 4 ARGS "Hello World" "10" EXPECTED "Position 10 in 'Hello World' is d\n")
do_test(NAME s1_assignment_2 NO 5 ARGS "Hello World" "11" EXPECTED "Error when finding position 11 of 'Hello World' with length 11\n")
do_test(NAME s1_assignment_2 NO 6 ARGS "Hello World" "-1" EXPECTED "Could not capture the position from -1\n")
do_test(NAME s1_assignment_2 NO 7 ARGS "Hello World" "a" EXPECTED "Could not capture the position from a\n")

# assignment_3
do_test(NAME s1_assignment_3 NO 1 ARGS "" EXPECTED "WARNING: Overflow detected when using short\n20000 \\+ 20000 = -25536\n9223372036854775806 \\+ 1 = 9223372036854775807\n")

# assignment_4
do_test(NAME s1_assignment_4 NO 1 ARGS "foo" "foo" EXPECTED "Strings are the same\n")
do_test(NAME s1_assignment_4 NO 3 ARGS "foo" "fo" EXPECTED "Strings are NOT the same\n")
do_test(NAME s1_assignment_4 NO 4 ARGS "foo" "fo " EXPECTED "Strings are NOT the same\n")
do_test(NAME s1_assignment_4 NO 5 ARGS "foo" "bar" EXPECTED "Strings are NOT the same\n")
do_test(NAME s1_assignment_4 NO 6 ARGS "Hello World" "Hello World" EXPECTED "Strings are the same\n")
do_test(NAME s1_assignment_4 NO 7 ARGS "Hello World" "Hello Worl" EXPECTED "Strings are NOT the same\n")
do_test(NAME s1_assignment_4 NO 8 ARGS "Hello World" "hello world" EXPECTED "Strings are NOT the same\n")

# assignment_5
do_test(NAME s1_assignment_5 NO 1 ARGS "0" EXPECTED "0\n")
do_test(NAME s1_assignment_5 NO 2 ARGS "1" EXPECTED "1\n")
do_test(NAME s1_assignment_5 NO 3 ARGS "2" EXPECTED "2\n")
do_test(NAME s1_assignment_5 NO 4 ARGS "9" EXPECTED "9\n")
do_test(NAME s1_assignment_5 NO 5 ARGS "a" EXPECTED "10\n")
do_test(NAME s1_assignment_5 NO 6 ARGS "b" EXPECTED "11\n")
do_test(NAME s1_assignment_5 NO 7 ARGS "c" EXPECTED "12\n")
do_test(NAME s1_assignment_5 NO 8 ARGS "f" EXPECTED "15\n")
do_test(NAME s1_assignment_5 NO 9 ARGS "A" EXPECTED "10\n")
do_test(NAME s1_assignment_5 NO 10 ARGS "B" EXPECTED "11\n")
do_test(NAME s1_assignment_5 NO 11 ARGS "C" EXPECTED "12\n")
do_test(NAME s1_assignment_5 NO 12 ARGS "F" EXPECTED "15\n")
do_test(NAME s1_assignment_5 NO 13 ARGS "g" EXPECTED "ERROR: 'g' is not a valid hex-value\n-1\n")
do_test(NAME s1_assignment_5 NO 14 ARGS "G" EXPECTED "ERROR: 'G' is not a valid hex-value\n-1\n")

# assignment_6
do_test(NAME s1_assignment_6 NO 1 ARGS "" EXPECTED "strcmpx\\(\"Abc\", \"Ab\"\\) gives -1\nstrcmpx\\(\"Abc\", \"Abc\"\\) gives 0\nstrcmpx\\(\"Abc\", \"Abcd\"\\) gives 1\nstrcmpx\\(\"Abd\", \"Abc\"\\) gives 1\nstrcmpx\\(\"Abc\", \"Abd\"\\) gives -1\nstrcmpx\\(\"abc\", \"Abc\"\\) gives 1\nstrcmpx\\(\"Abc\", \"abc\"\\) gives -1\n")
