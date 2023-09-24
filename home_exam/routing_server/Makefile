include ../common.mk

.PHONY: all clean test

all: $(EXEC_DIR)/routing_server

ROUTING_SERVER_SOURCE_FILES := $(wildcard $(CUR_DIR)routing_server/src/*.c)
ROUTING_SERVER_SRC := $(word 1, $(dir $(ROUTING_SERVER_SOURCE_FILES)))
# Remove trailing slash
ROUTING_SERVER_SRC := $(patsubst %/,%,$(ROUTING_SERVER_SRC))
# NOTE:
# Substitution references
# $(var:pattern=replacement)
# are equivalent to
# $(patsubst %suffix,%replacement,$(var))
# Here we cannot use the former, and must use the latter
OBJECT_FILES := $(patsubst $(ROUTING_SERVER_SRC)%.c,$(BUILD_OBJ_DIR)%.o,$(ROUTING_SERVER_SOURCE_FILES))

# Build the final executable
# NOTE: We also have an dependency on dynamic_memory.o
$(EXEC_DIR)/routing_server: $(OBJECT_FILES) $(LIB_DIR)/libprint.$(LIB_EXTENSION) $(BUILD_OBJ_DIR)/dynamic_memory.o
	$(call explain_build,Building routing_server)
	mkdir -p $(EXEC_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) $^ -o $@

# Building the object files
# NOTE: The only reason why we add an include path is because we have pretended
#       that print_lib.h comes from an external library
#       Since we use statements like
#       #include "print_lib/include/print_lib.h", and since print_lib doesn't
#       reside in "this" directory, we need to make sure that the compiler can
#       find it
$(BUILD_OBJ_DIR)%.o: $(ROUTING_SERVER_SRC)%.c
	$(call explain_build,Building the object files for routing_server)
	mkdir -p $(BUILD_OBJ_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) -I$(ASSIGNMENT_FILES_SRC) -c $< -o $@

test:
	$(MAKE) -C $(SRC_DIR)/test test

clean:
	rm -rf $(EXEC_DIR) $(BUILD_OBJ_DIR)
