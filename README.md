# stdlib

A C standard library implementation featuring common data structures.
This was created as a fun project while learning C. Some implementations may not be optimal.

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

## Documentation

For detailed API documentation, see the [docs/](docs/) directory.
