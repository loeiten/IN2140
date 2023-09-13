# Make file for home exam
# See https://makefiletutorial.com for tutorial

# Comment: A way to make compile_commands.json file with make:
# 		   Note that this is usually not needed as we have included the whole
#		   working directory in c_cpp_properties.json
# 	make --always-make --dry-run \
# 	| grep -wE 'clang|g\+\+' \
# 	| grep -w '\-c' \
# 	| jq -nR "[inputs|{directory:\"$(realpath ${PWD}/../build)\", command:., file: match(\" [^ ]+$\").string[1:]}]" \
# 	>> ../build/compile_commands.json && \
# 	sed -i -e 's/\]\[/,/' ../build/compile_commands.json && \
# 	cat ../build/compile_commands.json | jq > ../build/compile_commands.json.tmp && \
#   mv ../build/compile_commands.json.tmp ../build/compile_commands.json

# The include directive tells make to suspend reading the current makefile and
# read one or more other makefiles before continuing
include common.mk

# Declaration of targets which doesn't output a file
.PHONY: all clean run-tests test

# For all, we will simply call the sub-directories
# The first $ in $$ is an escape charater.
# So `$$dir` will expand to `$dir` instead of the variable named `dir`
# NOTE: When you call $(MAKE) -C you are creating a new process
COMPONENTS := assignment_files node routing_server test
all:
	for dir in $(COMPONENTS); do \
		$(MAKE) -C $$dir; \
	done

# Run the test executables
# NOTE: This depends on the target all
#       This is in order for test/Makefile to be aware of the targets in the
#       other directories
#       An alternative to this was to make test/Makefile aware of the targets
#       through include
test: all
	$(MAKE) -C test test

clean:
	rm -rf $(EXEC_DIR) $(TEST_EXEC_DIR) $(LIB_DIR) $(BUILD_LIB_DIR) $(BUILD_OBJ_DIR) $(BUILD_TEST_DIR)
