#include <string.h>
#include "ht_hash.h"

#define OFFSET 0x811c9dc5
#define PRIME 0x01000193

unsigned
HT_GetHash(const char *data) {
	unsigned hash = OFFSET;
	char *d = (char *)data;

	if (!d || !*d) return 0;
	while (*d) {
		hash ^= *d++;
		hash *= PRIME;
	}
	return hash;
}

int
HT_ProbeForBucket(const HT_HashTable *t, unsigned hash, int i, int len, int set) {
	int s = i ? i - 1 : len;

	while (t[i].h != hash) {
		if (set && !t[i].d) break;
		if (i == s) return -1;
		if (++i == len) i = 0;
	}
	return i;
}

void *
HT_GetValuePtr(const char *key, const HT_HashTable *t, int len) {
	unsigned hash = HT_GetHash(key);
	int i = (int)(hash % len);
	size_t k_len = strlen(key);

	if (t[i].h == hash && memcmp(&t[i].b, key, k_len > sizeof(unsigned) ? sizeof(unsigned) : k_len) == 0)
		return (void *)&t[i].d;
	i = HT_ProbeForBucket(t, hash, i, len, 0);
	return i == -1 ? NULL : (void *)&t[i].d;
}

unsigned
HT_SetValuePtr(const char *key, void *val, size_t nbytes, HT_HashTable *t, int len) {
	unsigned hash = HT_GetHash(key);
	int i = (int)(hash % len);
	size_t k_len = strlen(key);

	if (nbytes > sizeof(void *)) return -1;
	if (t[i].h && (t[i].h != hash || memcmp(&t[i].b, key, k_len > sizeof(unsigned) ? sizeof(unsigned) : k_len) != 0))
		i = HT_ProbeForBucket(t, hash, i, len, 1);
	if (i == -1) return 0;
	t[i].h = hash;
	t[i].d = val;
	memcpy(&t[i].b, key, k_len > sizeof(unsigned) ? sizeof(unsigned) : k_len);
	return hash;
}
