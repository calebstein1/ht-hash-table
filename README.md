# HT Hash Table

This is a tiny open-addressed hash table in about 200 SLOC.

## Usage

HT Hash operates on HT_HashTable objects.

```c
#include "ht_hash.h"

HT_HashTable my_table = HT_NewTable();

HT_SetValue("My Key", 12, my_table);
printf("%ld\n", HT_GetValue("My Key", my_table));
HT_DeleteKey("My Key", my_table);

HT_FreeTable(my_table);
```

You can also use the `HT_StoreRef` and `HT_GetRef` macros to store and get pointers to larger structs in your tables.

## Installation

Just drop `ht_hash.c` into your projects sources and include `ht_hash.h` and you're good to go!
HT Hash is written in C99 with no extensions so it should work almost everywhere.
Specifically, it depends on `errno.h`, `stdarg.h`, `stdlib.h`, `stdint.h`, `stdio.h`, and `string.h`.
