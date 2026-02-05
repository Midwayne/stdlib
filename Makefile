CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -O2 -DNDEBUG

# Check framework flags
# Try pkg-config first, fall back to Homebrew paths
CHECK_PREFIX := $(shell brew --prefix check 2>/dev/null)
ifdef CHECK_PREFIX
    CHECK_CFLAGS ?= -I$(CHECK_PREFIX)/include
    CHECK_LIBS ?= -L$(CHECK_PREFIX)/lib -lcheck -pthread
else
    CHECK_CFLAGS = $(shell pkg-config --cflags check)
    CHECK_LIBS = $(shell pkg-config --libs check)
endif

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
TEST_DIR = tests

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Test files
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/test_%.o,$(TEST_SRCS))
TEST_BIN = $(BUILD_DIR)/test_runner

LIB_NAME = libstdlib.a

.PHONY: all clean debug release test dirs help compile_commands

all: dirs format release

dirs:
	@mkdir -p $(BUILD_DIR) $(SRC_DIR) $(INC_DIR) $(TEST_DIR)

# Format source files before building
format:
	@command -v clang-format >/dev/null 2>&1 || { echo >&2 "clang-format is not installed. Skipping formatting."; exit 0; }
	@echo "Formatting C sources with clang-format..."
	@find $(SRC_DIR) $(INC_DIR) $(TEST_DIR) -type f \( -name "*.c" -o -name "*.h" \) -print0 | xargs -0 clang-format -i || true

debug: CFLAGS += $(DEBUG_FLAGS)
debug: dirs $(BUILD_DIR)/$(LIB_NAME)

release: CFLAGS += $(RELEASE_FLAGS)
release: dirs $(BUILD_DIR)/$(LIB_NAME)

# Build static library
$(BUILD_DIR)/$(LIB_NAME): $(OBJS)
	ar rcs $@ $^

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Build and run tests
test: CFLAGS += $(DEBUG_FLAGS)
test: dirs $(TEST_BIN)
	@echo "Running tests..."
	@./$(TEST_BIN)

# Compile test files (relax -pedantic for Check's GNU extensions)
$(BUILD_DIR)/test_%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) $(CHECK_CFLAGS) -Wno-gnu-zero-variadic-macro-arguments -I$(INC_DIR) -c $< -o $@

# Link test runner
$(TEST_BIN): $(TEST_OBJS) $(OBJS)
	$(CC) $(CFLAGS) $^ $(CHECK_LIBS) -o $@

clean:
	rm -rf $(BUILD_DIR)

# Generate compile_commands.json for LSP support
compile_commands:
	@echo '[' > compile_commands.json
	@first=1; \
	for src in $(SRCS); do \
		[ $$first -eq 0 ] && echo ',' >> compile_commands.json; \
		first=0; \
		echo '  {"directory": "$(CURDIR)", "file": "'$$src'", "command": "$(CC) $(CFLAGS) -I$(INC_DIR) -c '$$src'"}' >> compile_commands.json; \
	done; \
	for src in $(TEST_SRCS); do \
		echo ',' >> compile_commands.json; \
		echo '  {"directory": "$(CURDIR)", "file": "'$$src'", "command": "$(CC) $(CFLAGS) $(CHECK_CFLAGS) -Wno-gnu-zero-variadic-macro-arguments -I$(INC_DIR) -c '$$src'"}' >> compile_commands.json; \
	done
	@echo ']' >> compile_commands.json
	@echo "Generated compile_commands.json"

help:
	@echo "stdlib Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  all      - Build release version (default)"
	@echo "  debug    - Build with debug symbols"
	@echo "  release  - Build optimized version"
	@echo "  test     - Build and run tests (requires Check framework)"
	@echo "  clean    - Remove build artifacts"
	@echo "  dirs     - Create project directories"
	@echo "  help     - Show this message"
	@echo ""
	@echo "Prerequisites:"
	@echo "  brew install check    (macOS)"
	@echo "  apt install check     (Debian/Ubuntu)"
	@echo ""
	@echo "Editor support:"
	@echo "  make compile_commands - Generate compile_commands.json for LSP"
