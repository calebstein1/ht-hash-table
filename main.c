#include <stdio.h>
#include <string.h>

#include <arm_acle.h>

#define HT_ZeroTable(t) memset((t), 0, sizeof((t)))
#define HT_GetValue(k, type, t) *(type *)HT_GetValuePtr((k), (t), (sizeof((t)) / sizeof(*(t))))
#define HT_SetValue(k, type, v, t) HT_SetValuePtr((k), (void *)(v), sizeof(type), (t), (sizeof((t)) / sizeof(*(t))))

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

unsigned
HT_ProbeForBucket(const HT_HashTable *t, unsigned hash, unsigned i, size_t len) {
	unsigned s = i ? i - 1 : len - 1;

	while (t[i].h != hash) {
		if (++i == len) i = 0;
		if (i == s) return -1;
	}

	return i;
}

unsigned HT_ProbeForFreeBucket(const HT_HashTable *t, unsigned i, size_t len) {
	unsigned s = i ? i - 1 : len - 1;

	while (t[i].h) {
		if (++i == len) i = 0;
		if (i == s) return -1;
	}

	return i;
}

void *
HT_GetValuePtr(const char *key, const HT_HashTable *t, size_t len) {
	unsigned hash = HT_GetHash(key);
	unsigned i = (int)(hash % len);

	if (t[i].h == hash) return (void *)&t[i].d;

	i = HT_ProbeForBucket(t, hash, i, len);

	return (signed)i == -1 ? NULL : (void *)&t[i].d;
}

unsigned
HT_SetValuePtr(const char *key, void *val, size_t nbytes, HT_HashTable *t, size_t len) {
	unsigned hash = HT_GetHash(key);
	unsigned i = hash % len;

	if (nbytes > sizeof(void *)) return -1;

	if (t[i].h && t[i].h != hash) i = HT_ProbeForFreeBucket(t, i, len);
	if ((signed)i == -1) return i;

	t[i].h = hash;
	t[i].d = val;

	return hash;
}

int
main(void) {
	HT_HashTable t[16];
	char *test_string = "Hello, world!";

	HT_ZeroTable(t);
	HT_SetValue("Value 1", int, 16, t);
	HT_SetValue("Value 2", int, 12, t);
	HT_SetValue("Value 3", int, 243, t);
	HT_SetValue("Value 4", int, 19, t);
	HT_SetValue("Value 5", char *, test_string, t);
	HT_SetValue("Value 6", int, 55682, t);

	HT_GetValue("Value 2", int, t) += 8;

	printf("%d, %s, %d, %d, %d, %d\n",
		HT_GetValue("Value 6", int, t),
		HT_GetValue("Value 5", char *, t),
		HT_GetValue("Value 4", int, t),
		HT_GetValue("Value 3", int, t),
		HT_GetValue("Value 2", int, t),
		HT_GetValue("Value 1", int, t)
	);

	return 0;
}
