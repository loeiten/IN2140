include ../common.mk

.PHONY: all clean test

all: $(EXEC_DIR)/routing_server

SOURCE_FILES := $(wildcard src/*.c)
$(info "SOURCE_FILES")
$(info ${SOURCE_FILES})
# We here use the functionality $(list:pattern=replace)
# One could also have used $(patsubst pattern,replacement,text)
# We are also using the % wildcard
OBJECT_FILES := $(SOURCES:src/%.c=$(BUILD_OBJ_DIR)/%.o)
$(info "OBJECT_FILES")
$(info ${OBJECT_FILES})

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
