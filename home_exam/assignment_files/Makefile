# NOTE:
# This Makefile takes variables passed in from the "outside" (typically from the
# parent Makefile)

# Make the complete library
# NOTE: We do NOT need to include the headers
$(LIB_DIR)/libprint.$(LIB_EXTENSION): $(BUILD_OBJ_DIR)/print_lib.o
	mkdir -p $(dir $@)
	$(ARCHIVE) $(LIB_FLAGS) $@ $<

# Make the object file of the print_lib library
# $< is the first prerequisite
# $@ is name of target to be created
# https://stackoverflow.com/a/3220288/2786884
$(BUILD_OBJ_DIR)/print_lib.o: print_lib/src/print_lib.c print_lib/include/print_lib.h
	mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $(CC_LINT_FLAGS) $(CC_SANITIZERS) -c $< -o $@
