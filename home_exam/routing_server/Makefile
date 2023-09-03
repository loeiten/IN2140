include ../common.mk

.PHONY: all clean test

all: $(EXEC_DIR)/routing_server

SOURCE_FILES := $(wildcard src/*.c)
# NOTE:
# Substitution references
# $(var:pattern=replacement)
# are equivalent to
# $(patsubst %suffix,%replacement,$(var))
# Here we cannot use the former, and must use the latter
OBJECT_FILES := $(patsubst src/%.c,$(BUILD_OBJ_DIR)/%.o,$(SOURCE_FILES))

# Build the final executable
$(EXEC_DIR)/routing_server: $(OBJECT_FILES)
	$(call explain_build,Building routing_server)
	$(CC) $(CCFLAGS) $(CC_LINT_FLAGS) $(CC_SANITIZERS) $^ -o $@

# Building the object files
$(BUILD_OBJ_DIR)/%.o: src/%.c
	$(call explain_build,Building the object files for routing_server)
	$(CC) $(CCFLAGS) $(CC_LINT_FLAGS) $(CC_SANITIZERS) -c $< -o $@

test:
	$(MAKE) -C $(SRC_DIR)/test $(PROJECT_PATHS) $(PROJECT_CC_FLAGS) $(PROJECT_LIB_FLAGS) test

clean:
	rm -rf $(EXEC_DIR) $(BUILD_OBJ_DIR)
