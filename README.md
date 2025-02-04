# HT Hash Table

This is a very simple, pure C89 implementation of a stack-allocated, fixed-size, open-addressed hash table supporting non-floating point values and pointers.

## Usage

HT Hash operates on stack-allocated, fixed-sized arrays of HT_HashTable structures.
These HT_HashTables are operated on using the macros available in `ht_hash.h`.

```c
#include "ht_hash.h"

HT_HashTable my_table[16];
int n;
int *n_p;

HT_ZeroTable(my_table); /* Table needs to be zeroed before use */

HT_SetValue("My Key", int, 54, my_table); /* Sets a value of a specified non-floating point type to the key's bucket */
if (HT_CheckKeyExists("My Key", my_table)) { /* True if a value is associated with the supplied key, false if not */
        n = HT_GetValue("My_Key", int, my_table); /* Gets the value of a specified type associated with the key */
        n_p = HT_GetRef("My_Key", my_table); /* Gets a pointer to the value associated with the key */
}
```

## Installation

Just drop `ht_hash.c` into your projects sources and include `ht_hash.h` and you're good to go!
HT Hash is written in C89 with no extensions so it should work everywhere.