include ../common.mk

.PHONY: all clean test

all: $(EXEC_DIR)/node

NODE_SOURCE_FILES := $(wildcard $(CUR_DIR)node/src/*.c)
NODE_SRC := $(word 1, $(dir $(NODE_SOURCE_FILES)))
# Remove trailing slash
NODE_SRC := $(patsubst %/,%,$(NODE_SRC))
# NOTE:
# Substitution references
# $(var:pattern=replacement)
# are equivalent to
# $(patsubst %suffix,%replacement,$(var))
# Here we cannot use the former, and must use the latter
OBJECT_FILES := $(patsubst $(NODE_SRC)%.c,$(BUILD_OBJ_DIR)%.o,$(NODE_SOURCE_FILES))

# Build the final executable
$(EXEC_DIR)/node: $(OBJECT_FILES) $(BUILD_OBJ_DIR)/common.o $(BUILD_OBJ_DIR)/dynamic_memory.o
	$(call explain_build,Building node)
	mkdir -p $(EXEC_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) $^ -o $@

# Building the object files
$(BUILD_OBJ_DIR)%.o: $(NODE_SRC)%.c
	$(call explain_build,Building the object files for node)
	mkdir -p $(BUILD_OBJ_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) -c $< -o $@

test:
	$(MAKE) -C $(SRC_DIR)/test test

clean:
	rm -rf $(EXEC_DIR) $(BUILD_OBJ_DIR)
