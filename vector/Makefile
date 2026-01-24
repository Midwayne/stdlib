CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -O2 -DNDEBUG

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
TEST_DIR = tests

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_BINS = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%,$(TEST_SRCS))

LIB_NAME = libvector.a

.PHONY: all clean debug release test dirs

all: dirs release

dirs:
	@mkdir -p $(BUILD_DIR) $(SRC_DIR) $(INC_DIR) $(TEST_DIR)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: dirs $(BUILD_DIR)/$(LIB_NAME)

release: CFLAGS += $(RELEASE_FLAGS)
release: dirs $(BUILD_DIR)/$(LIB_NAME)

$(BUILD_DIR)/$(LIB_NAME): $(OBJS)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

test: debug $(TEST_BINS)
	@for test in $(TEST_BINS); do \
		echo "Running $$test..."; \
		$$test || exit 1; \
	done

$(BUILD_DIR)/%: $(TEST_DIR)/%.c $(BUILD_DIR)/$(LIB_NAME)
	$(CC) $(CFLAGS) -I$(INC_DIR) $< -L$(BUILD_DIR) -lvector -o $@

clean:
	rm -rf $(BUILD_DIR)

help:
	@echo "Available targets:"
	@echo "  all      - Build release version (default)"
	@echo "  debug    - Build with debug symbols"
	@echo "  release  - Build optimized version"
	@echo "  test     - Build and run tests"
	@echo "  clean    - Remove build artifacts"
	@echo "  dirs     - Create project directories"
	@echo "  help     - Show this message"
