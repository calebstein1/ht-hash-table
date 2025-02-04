#ifndef HASH_TABLE_HT_HASH_H
#define HASH_TABLE_HT_HASH_H

#define HT_ZeroTable(t) memset((t), 0, sizeof((t)))
#define HT_CheckValueExists(k, t) (HT_GetValuePtr((k), (t), (sizeof((t)) / sizeof(*(t)))) != NULL)
#define HT_GetRef(k, t) HT_GetValuePtr((k), (t), (sizeof((t)) / sizeof(*(t))))
#define HT_GetValue(k, type, t) *(type *)HT_GetValuePtr((k), (t), (sizeof((t)) / sizeof(*(t))))
#define HT_SetValue(k, type, v, t) HT_SetValuePtr((k), (void *)(v), sizeof(type), (t), (sizeof((t)) / sizeof(*(t))))

typedef struct ht_hashtable_t HT_HashTable;

void *HT_GetValuePtr(const char *key, const HT_HashTable *t, int len);
unsigned HT_SetValuePtr(const char *key, void *val, size_t nbytes, HT_HashTable *t, int len);

#endif /* HASH_TABLE_HT_HASH_H */
