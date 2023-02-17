set(source_files
    s2_assignment_1.c
    s2_assignment_2.c
    s2_assignment_3.c
    s2_assignment_4.c
    s2_assignment_5.c
    s2_assignment_6.c
)

set(link_with_utils
    s2_assignment_5
)

set(link_with_person
    s2_assignment_5
    s2_assignment_6
)

# Create executables
make_executable(SOURCE ${source_files})

# Link with libraries
link_executable(TARGETS ${link_with_utils} LIB utils)
link_executable(TARGETS ${link_with_person} LIB person)

# Test
# assignment_1
do_test(NAME s2_assignment_1 NO 1 ARGS "0" "0" EXPECTED "0\n")
do_test(NAME s2_assignment_1 NO 2 ARGS "123" "456" EXPECTED "579\n")
do_test(NAME s2_assignment_1 NO 3 ARGS "0" "-0" EXPECTED "0\n")
do_test(NAME s2_assignment_1 NO 4 ARGS "123" "-456" EXPECTED "-333\n")

# assignment_2
do_test(NAME s2_assignment_2 NO 1 ARGS "" EXPECTED "\\[0\\]: 2\n\\[1\\]: 1\n\\[2\\]: -1\n\\[3\\]: 10\n\\[4\\]: 22\n\\[5\\]: 21\n")

# assignment_3
# Move files
file(GLOB text_files "s2_assignment_3*.txt")
foreach(text_file ${text_files})
    get_filename_component(cur_file ${text_file} NAME)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/${cur_file}
         DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach(text_file ${text_files})
# Run tests
do_test(NAME s2_assignment_3 NO 1 ARGS "is" "s2_assignment_3_0.txt" EXPECTED "1/4\n")
do_test(NAME s2_assignment_3 NO 2 ARGS "this" "s2_assignment_3_0.txt" EXPECTED "0/4\n")
do_test(NAME s2_assignment_3 NO 3 ARGS "is" "s2_assignment_3_1.txt" EXPECTED "1/4\n")
do_test(NAME s2_assignment_3 NO 4 ARGS "is" "s2_assignment_3_2.txt" EXPECTED "1/4\n")
do_test(NAME s2_assignment_3 NO 5 ARGS "is" "s2_assignment_3_3.txt" EXPECTED "2/12\n")
do_test(NAME s2_assignment_3 NO 6 ARGS "mauris" "s2_assignment_3_4.txt" EXPECTED "2/482\n")
do_test(NAME s2_assignment_3 NO 7 ARGS "Mauris" "s2_assignment_3_4.txt" EXPECTED "1/482\n")
do_test(NAME s2_assignment_3 NO 8 ARGS "dolor" "s2_assignment_3_4.txt" EXPECTED "5/482\n")
do_test(NAME s2_assignment_3 NO 9 ARGS "do" "do_not_exist.txt" EXPECTED "do_not_exist.txt not found\n")

# assignment_4
do_test(NAME s2_assignment_4 NO 1 ARGS "my_name" "30" "my_file.txt" EXPECTED "Hello my_name, you are 30 years old.\n")

# assignment_5
do_test(NAME s2_assignment_5 NO 1 ARGS "Bob" "5" EXPECTED "Name: Bob, age: 5\n")
do_test(NAME s2_assignment_5 NO 2 ARGS "Bob" "5" "Alice" "6" EXPECTED "Name: Bob, age: 5\nName: Alice, age: 6\n")
do_test(NAME s2_assignment_5 NO 3 ARGS "null" "-1" EXPECTED "Failed to capture the age from '-1'\n")
do_test(NAME s2_assignment_5 NO 4 ARGS "Bob" "5" "Alice" EXPECTED "Name: Bob, age: 5\nNot enough arguments given\n")

# assignment_6
# Move files
file(GLOB text_files "s2_assignment_6*.txt")
file(GLOB csv_files "s2_assignment_6*.csv")
set(files ${text_files} ${csv_files})
foreach(file ${files})
    get_filename_component(cur_file ${file} NAME)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/${cur_file}
         DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach(file ${files})
# Run test
do_test(NAME s2_assignment_6 NO 1 ARGS "s2_assignment_6.csv" EXPECTED "Name: Alice, age: 25\nName: Bob, age: 26\nName: Carol, age: 30\nName: Dan, age: 28\nName: Eve, age: 17\nName: Frank, age: 50\n")
do_test(NAME s2_assignment_6 NO 2 ARGS "s2_assignment_6.csv" "s2_assignment_6_remove.txt" "s2_assignment_6_add.txt" EXPECTED "Name: Bob, age: 26\nName: Carol, age: 30\nName: Erin, age: 28\nName: Vanna, age: 20\n")
