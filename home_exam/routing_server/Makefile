include ../common.mk

.PHONY: all clean test

all: $(EXEC_DIR)/routing_server

ROUTING_SERVER_SOURCE_FILES := $(wildcard $(CUR_DIR)routing_server/src/*.c)
ROUTING_SERVER_SRC := $(word 1, $(dir $(ROUTING_SERVER_SOURCE_FILES)))
# NOTE:
# Substitution references
# $(var:pattern=replacement)
# are equivalent to
# $(patsubst %suffix,%replacement,$(var))
# Here we cannot use the former, and must use the latter
OBJECT_FILES := $(patsubst $(ROUTING_SERVER_SRC)%.c,$(BUILD_OBJ_DIR)/%.o,$(ROUTING_SERVER_SOURCE_FILES))

# Build the final executable
$(EXEC_DIR)/routing_server: $(OBJECT_FILES)
	$(call explain_build,Building routing_server)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) $^ -o $@

# FIXME: include h
# Building the object files
$(BUILD_OBJ_DIR)/%.o: $(ROUTING_SERVER_SRC)/%.c
	$(call explain_build,Building the object files for routing_server)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) -c $< -o $@

test:
	$(MAKE) -C $(SRC_DIR)/test $(PROJECT_PATHS) $(PROJECT_CC_FLAGS) $(PROJECT_LIB_FLAGS) test

clean:
	rm -rf $(EXEC_DIR) $(BUILD_OBJ_DIR)
