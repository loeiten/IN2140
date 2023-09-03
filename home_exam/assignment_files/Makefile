include ../common.mk

.PHONY: all clean test

all: $(LIB_DIR)/libprint.$(LIB_EXTENSION) $(EXEC_DIR)/assignment_files

# Copy the files from assignment into the executable
ASSIGNMENT_FILES := $(wildcard *.*)
$(EXEC_DIR)/assignment_files: $(ASSIGNMENT_FILES)
	$(call explain_build,Copying the assingment files)
	mkdir -p $(EXEC_DIR)/assignment_files
	cp $(ASSIGNMENT_FILES) $(EXEC_DIR)/assignment_files

# Make the complete library
# NOTE: We do NOT need to include the headers
$(LIB_DIR)/libprint.$(LIB_EXTENSION): $(BUILD_LIB_DIR)/print_lib.o
	$(call explain_build,Creating the libprint library)
	mkdir -p $(dir $@)
	$(ARCHIVE) $(LIB_FLAGS) $@ $<

# Make the object file of the print_lib library
# $< is the first prerequisite
# $@ is name of target to be created
# https://stackoverflow.com/a/3220288/2786884
$(BUILD_LIB_DIR)/print_lib.o: print_lib/src/print_lib.c print_lib/include/print_lib.h
	$(call explain_build,Building object files of print_lib)
	mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $(CC_LINT_FLAGS) $(CC_SANITIZERS) -c $< -o $@

test:
	$(MAKE) -C $(SRC_DIR)/test test

clean:
	rm -rf $(LIB_DIR) $(BUILD_LIB_DIR)
