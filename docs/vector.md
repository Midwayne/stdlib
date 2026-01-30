# Vector

A dynamic array implementation

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
