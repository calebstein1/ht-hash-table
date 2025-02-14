#ifndef HASH_TABLE_HT_HASH_H
#define HASH_TABLE_HT_HASH_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define HT_StoreRef(key, val, tbl) (HT_SetValue((key), (uintptr_t)(val), (tbl)))
#define HT_GetRef(key, type, tbl) ((type *)HT_GetValue((key), (tbl)))

typedef struct hash_table_t HT_HashTable;

HT_HashTable *HT_InitTable(struct hash_table_t **tbl);
uintptr_t HT_GetValue(const char *key, const HT_HashTable *t);
unsigned HT_SetValue(const char *key, uintptr_t val, HT_HashTable *t);
void HT_FreeTable(struct hash_table_t **tbl);

#endif /* HASH_TABLE_HT_HASH_H */
