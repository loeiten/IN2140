include ../common.mk

.PHONY: all clean test

UTILS_SOURCE_FILES := $(wildcard $(CUR_DIR)utils/src/*.c)
UTILS_FILES_SRC := $(word 1, $(dir $(UTILS_SOURCE_FILES)))
# Remove trailing slash
UTILS_FILES_SRC := $(patsubst %/,%,$(UTILS_FILES_SRC))

# NOTE:
# Substitution references
# $(var:pattern=replacement)
# are equivalent to
# $(patsubst %suffix,%replacement,$(var))
# Here we cannot use the former, and must use the latter
OBJECT_FILES := $(patsubst $(UTILS_FILES_SRC)%.c,$(BUILD_OBJ_DIR)%.o,$(UTILS_SOURCE_FILES))

all: $(OBJECT_FILES)

# Building the object files
$(BUILD_OBJ_DIR)%.o: $(UTILS_FILES_SRC)%.c
	$(call explain_build,Building the object files for utils)
	mkdir -p $(BUILD_OBJ_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) -c $< -o $@

test:
	$(MAKE) -C $(SRC_DIR)/test test

clean:
	rm -rf $(BUILD_OBJ_DIR)
