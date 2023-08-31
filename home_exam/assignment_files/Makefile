# NOTE:
# This Makefile takes variables passed in from the "outside" (typically from the
# parent Makefile)

.PHONY: all clean test

all: $(LIB_DIR)/libprint.$(LIB_EXTENSION)

# Make the complete library
# NOTE: We do NOT need to include the headers
$(LIB_DIR)/libprint.$(LIB_EXTENSION): $(BUILD_LIB_DIR)/print_lib.o
	echo "\n\033[92mCreating the libprint library\033[0m"
	mkdir -p $(dir $@)
	$(ARCHIVE) $(LIB_FLAGS) $@ $<

# Make the object file of the print_lib library
# $< is the first prerequisite
# $@ is name of target to be created
# https://stackoverflow.com/a/3220288/2786884
$(BUILD_LIB_DIR)/print_lib.o: print_lib/src/print_lib.c print_lib/include/print_lib.h
	echo "\n\033[92mBuilding object files of print_lib\033[0m"
	mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $(CC_LINT_FLAGS) $(CC_SANITIZERS) -c $< -o $@

test:
	$(MAKE) -C $(SRC_DIR)/test $(PROJECT_PATHS) $(PROJECT_CC_FLAGS) $(PROJECT_LIB_FLAGS) test

clean:
	rm -rf $(LIB_DIR) $(BUILD_LIB_DIR)
