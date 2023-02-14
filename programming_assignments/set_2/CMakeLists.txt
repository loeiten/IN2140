set(source_files
    s2_assignment_1.c
    s2_assignment_2.c
    s2_assignment_3.c
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

# assignment_3
file(GLOB text_files "s2_assignment_3*.txt")
foreach(text_file ${text_files})
    get_filename_component(cur_file ${text_file} NAME)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/${cur_file}
         DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach(text_file ${text_files})
do_test(NAME s2_assignment_3 NO 1 ARGS "is" "s2_assignment_3_0.txt" EXPECTED "1/4\n")
do_test(NAME s2_assignment_3 NO 2 ARGS "this" "s2_assignment_3_0.txt" EXPECTED "0/4\n")
do_test(NAME s2_assignment_3 NO 3 ARGS "is" "s2_assignment_3_1.txt" EXPECTED "1/4\n")
do_test(NAME s2_assignment_3 NO 4 ARGS "is" "s2_assignment_3_2.txt" EXPECTED "1/4\n")
do_test(NAME s2_assignment_3 NO 5 ARGS "is" "s2_assignment_3_3.txt" EXPECTED "2/12\n")
do_test(NAME s2_assignment_3 NO 6 ARGS "mauris" "s2_assignment_3_4.txt" EXPECTED "2/482\n")
do_test(NAME s2_assignment_3 NO 7 ARGS "Mauris" "s2_assignment_3_4.txt" EXPECTED "1/482\n")
do_test(NAME s2_assignment_3 NO 8 ARGS "dolor" "s2_assignment_3_4.txt" EXPECTED "5/482\n")
do_test(NAME s2_assignment_3 NO 9 ARGS "do" "do_not_exist.txt" EXPECTED "do_not_exist.txt not found\n")
