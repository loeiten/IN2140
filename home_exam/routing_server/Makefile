# NOTE:
# This Makefile takes variables passed in from the "outside" (typically from the
# parent Makefile)

.PHONY: all clean test

all: $(EXEC_DIR)/routing_server

SOURCE_FILES := $(wildcard src/*.c)
# We here use the functionality $(list:pattern=replace)
# One could also have used $(patsubst pattern,replacement,text)
# We are also using the % wildcard
OBJECT_FILES := $(SOURCES:%.c=$(BUILD_OBJ_DIR)/%.o)

# Build the final executable
$(EXEC_DIR)/routing_server: $(OBJECT_FILES)
	echo "\n\033[92mBuilding routing_server\033[0m"
	$(CC) $(CCFLAGS) $(CC_LINT_FLAGS) $(CC_SANITIZERS) $^ -o $@

# Building the object files
$(BUILD_OBJ_DIR)/%.o: src/%.c
	echo "\n\033[92mBuilding the object files for routing_server\033[0m"
	$(CC) $(CCFLAGS) $(CC_LINT_FLAGS) $(CC_SANITIZERS) -c $< -o $@

test:
	$(MAKE) -C $(SRC_DIR)/test $(PROJECT_PATHS) $(PROJECT_CC_FLAGS) $(PROJECT_LIB_FLAGS) test

clean:
	rm -rf $(EXEC_DIR) $(BUILD_OBJ_DIR)
