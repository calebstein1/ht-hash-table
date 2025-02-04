#include <stdio.h>

#include <arm_acle.h>

typedef struct {
    void *d;
    unsigned h;
} HT_HashTable;

unsigned
HT_GetHash(const char *data) {
	char *d = (char *)data;
	unsigned int crc = 0xffffffff;

	while(*d) crc = __crc32cb(crc, *d++);

	return crc ^ 0xffffffff;
}

int
HT_ProbeForBucket(const HT_HashTable *t, unsigned hash, unsigned i, size_t len) {
	unsigned s = i ? i - 1 : len - 1;

	while (t[i].h != hash) {
		if (++i == len) i = 0;
		if (i == s) return -1;
	}

	return (int)i;
}

void *
HT_GetValue(const char *key, const HT_HashTable *t, size_t len) {
	unsigned hash = HT_GetHash(key);
	int i = (int)(hash % len);

	if (t[i].h == hash) return t[i].d;

	i = HT_ProbeForBucket(t, hash, i, len);

	return i == -1 ? NULL : t[i].d;
}

int
main(void) {

	return 0;
}
