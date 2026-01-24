# List of all library subdirectories
LIBS = vector

.PHONY: all clean test debug release help $(LIBS)

all: $(LIBS)

$(LIBS):
	@echo "Building $@..."
	@$(MAKE) -C $@

debug:
	@for lib in $(LIBS); do \
		echo "Building $$lib (debug)..."; \
		$(MAKE) -C $$lib debug || exit 1; \
	done

release:
	@for lib in $(LIBS); do \
		echo "Building $$lib (release)..."; \
		$(MAKE) -C $$lib release || exit 1; \
	done

test:
	@for lib in $(LIBS); do \
		echo "Testing $$lib..."; \
		$(MAKE) -C $$lib test || exit 1; \
	done

clean:
	@for lib in $(LIBS); do \
		echo "Cleaning $$lib..."; \
		$(MAKE) -C $$lib clean; \
	done

# Build specific library: make lib-<name>
lib-%:
	@$(MAKE) -C $*

# Test specific library: make test-<name>
test-%:
	@$(MAKE) -C $* test

# Clean specific library: make clean-<name>
clean-%:
	@$(MAKE) -C $* clean

# Debug specific library: make debug-<name>
debug-%:
	@$(MAKE) -C $* debug

help:
	@echo "stdlib Master Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  all            - Build all libraries (default)"
	@echo "  debug          - Build all libraries with debug symbols"
	@echo "  release        - Build all libraries optimized"
	@echo "  test           - Run tests for all libraries"
	@echo "  clean          - Clean all libraries"
	@echo ""
	@echo "Per-library targets:"
	@echo "  lib-<name>     - Build specific library (e.g., make lib-vector)"
	@echo "  test-<name>    - Test specific library (e.g., make test-vector)"
	@echo "  clean-<name>   - Clean specific library (e.g., make clean-vector)"
	@echo "  debug-<name>   - Debug build specific library"
	@echo ""
	@echo "Available libraries: $(LIBS)"
