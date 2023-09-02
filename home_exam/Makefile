# Make file for home exam
# See https://makefiletutorial.com for tutorial

# The include directive tells make to suspend reading the current makefile and
# read one or more other makefiles before continuing
include common.mk

# Declaration of targets which doesn't output a file
.PHONY: all clean test run-tests


# For all, we will simply call the sub-directories
# The first $ in $$ is an escape charater.
# So `$$dir` will expand to `$dir` instead of the variable named `dir`
COMPONENTS := assignment_files node routing_server test
all:
	for dir in $(COMPONENTS); do \
		$(MAKE) -C $$dir; \
	done


## FIXME: All could instead call all the makefiles in the subdirs
## Make the routing_server executable
## FIXME: Have the correct dependencies
#$(EXEC_DIR)/routing_server: $(LIB_DIR)/libprint.$(LIB_EXTENSION) $(EXEC_DIR)/assignment_files
#	$(MAKE) -C routing_server $(PROJECT_PATHS) $(PROJECT_CC_FLAGS) $(PROJECT_LIB_FLAGS)
#
## Make the node executable
## FIXME: Have directory specifically for node
#$(EXEC_DIR)/node: $(LIB_DIR)/libprint.$(LIB_EXTENSION) $(EXEC_DIR)/assignment_files
#	touch $(EXEC_DIR)/node
#
## Make the test executable
#test:
#	$(MAKE) -C test $(PROJECT_PATHS) $(PROJECT_CC_FLAGS) $(PROJECT_LIB_FLAGS) test
#
## Run the test executables
#run-tests:
#	$(MAKE) -C test $(PROJECT_PATHS) $(PROJECT_CC_FLAGS) $(PROJECT_LIB_FLAGS) run-tests
#
## Copy the files from assignment into the executable
#ASSIGNMENT_FILES := $(wildcard assignment_files/*.*)
#$(EXEC_DIR)/assignment_files: $(ASSIGNMENT_FILES)
#	echo "\n\033[92mCopying the assignment files\033[0m"
#	mkdir -p $(EXEC_DIR)/assignment_files
#	cp $(ASSIGNMENT_FILES) $(EXEC_DIR)/assignment_files
#
#
## Make the library
#$(LIB_DIR)/libprint.$(LIB_EXTENSION):
#	$(MAKE) -C assignment_files $(PROJECT_PATHS) $(PROJECT_CC_FLAGS) $(PROJECT_LIB_FLAGS)

clean:
	rm -rf $(EXEC_DIR) $(TEST_EXEC_DIR) $(LIB_DIR) $(BUILD_LIB_DIR) $(BUILD_OBJ_DIR) $(BUILD_TEST_DIR)
