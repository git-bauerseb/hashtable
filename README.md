# hashtable

Implementation of a hashtable using chaining for hash collision in C.

## Features

### Generic key/value storage
Keys and values are not required to have a predetermined size. This adds a little overhead to the
implementation but gives you much freedom in what 

### Custom hash function
In case you encounter frequent hash collisions or simply want to experiment, you can specify a custom hash function:

```c
#include "hashtable.h"

// Don't use this as hash function!
size_t my_hash(void* key, size_t key_size) {
    return 1;
}

int main() {
    t_hash_table* ht = ht_new_c_hash(5, &my_hash);
    ht_insert(ht, "HE", "abc", 2, 3);
}
```