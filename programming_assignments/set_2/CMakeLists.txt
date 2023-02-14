set(source_files
    s2_assignment_1.c
    s2_assignment_2.c
)

# Create executables
make_executable(SOURCE ${source_files})

# Test
# assignment_1
do_test(NAME s2_assignment_1 NO 1 ARGS "0" "0" EXPECTED "0\n")
do_test(NAME s2_assignment_1 NO 2 ARGS "123" "456" EXPECTED "579\n")
do_test(NAME s2_assignment_1 NO 3 ARGS "0" "-0" EXPECTED "0\n")
do_test(NAME s2_assignment_1 NO 4 ARGS "123" "-456" EXPECTED "-333\n")

# assignment_2
do_test(NAME s2_assignment_2 NO 1 ARGS "" EXPECTED "\\[0\\]: 2\n\\[1\\]: 1\n\\[2\\]: -1\n\\[3\\]: 10\n\\[4\\]: 22\n\\[5\\]: 21\n")
