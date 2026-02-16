# Unordered set

An unordered set implementation which uses the bucket chaining algorithm.

## API Reference 

### Lifecycle

- `unordered_set_init(set, key_size, hash_function, key_compare_function)` - Initialize an unordered set with a certain key size (e.g., int), a hash function and a function to compare two different keys.
- `unordered_set_free(set)` - Free all memory

### Modifiers

- `unordered_set_insert(set, key)` - Insert an element into the set
- `unordered_set_erase(set, key)` - Remove an element from the set
- `unordered_set_clear(set)` - Removes all the elements from the set 

### Access 

- `unordered_set_contains(set, key)` - Check if the element exists in the set
- `unordered_set_size(set)` - Find the number of elements in the set
- `unordered_set_is_empty(set)` - Check if the set is empty
