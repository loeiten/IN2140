include ../common.mk

.PHONY: all clean test

all: $(EXEC_DIR)/node

# FIXME: Dummy until the code is written
$(EXEC_DIR)/node:
	$(call explain_build,Creating the node executable)
	mkdir -p $(dir $@)
	touch $(EXEC_DIR)/node

test:
	$(MAKE) -C $(SRC_DIR)/test test

clean:
	rm -rf $(EXEC_DIR)
