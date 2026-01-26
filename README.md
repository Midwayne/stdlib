# stdlib

A C standard library implementation featuring common data structures.

## Data Structures

- **Vector** - Dynamic array with automatic resizing

## Building

```bash
# Build release version
make

# Build with debug symbols
make debug

# Clean build artifacts
make clean
```

## Testing

Tests use the [Check](https://libcheck.github.io/check/) unit testing framework.

### Prerequisites

```bash
# macOS
brew install check

# Debian/Ubuntu
sudo apt install check
```

### Running Tests

```bash
make test
```

## Editor Support

To enable LSP features (autocomplete, diagnostics) in your editor:

```bash
make compile_commands
```

This generates `compile_commands.json` for clangd/ccls. Restart your editor after running this command.

## Usage

```c
#include "vector.h"

int main(void) {
    Vector v;
    vector_init(&v, sizeof(int));

    // Add elements
    vector_push_back(&v, &(int){42});
    vector_push_back(&v, &(int){100});

    // Access elements
    int *first = vector_get(&v, 0);  // 42
    int *last = vector_back(&v);     // 100

    // Cleanup
    vector_free(&v);
    return 0;
}
```

## API Reference

### Lifecycle
- `vector_init(v, element_size)` - Initialize with default capacity
- `vector_reserve(v, capacity)` - Reserve memory for capacity elements
- `vector_free(v)` - Free all memory

### Access
- `vector_get(v, index)` - Get element at index
- `vector_front(v)` - Get first element
- `vector_back(v)` - Get last element

### Modifiers
- `vector_set(v, index, element)` - Set element at index
- `vector_push_back(v, element)` - Add element to end
- `vector_pop_back(v)` - Remove last element
- `vector_insert(v, index, element)` - Insert at index
- `vector_erase(v, index)` - Remove at index
- `vector_clear(v)` - Remove all elements
- `vector_shrink_to_fit(v)` - Reduce capacity to size

### Status
- `vector_size(v)` - Get number of elements
- `vector_is_empty(v)` - Check if empty
