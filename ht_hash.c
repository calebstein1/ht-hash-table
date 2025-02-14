#include "ht_hash.h"

#define OFFSET 0x811c9dc5
#define PRIME 0x01000193
#define INIT_TBL_SIZE 8

enum hash_status_e {
	HT_USED = 1,
	HT_DELETED
};

struct hash_entry_t {
	intptr_t d;
	unsigned h;
	enum hash_status_e s;
	char k[MAX_KEY_LEN];
};

struct hash_table_t {
	int s, c;
	struct hash_entry_t *d;
};

HT_HashTable
HT_NewTable(void) {
	HT_HashTable tbl;

	if (!(tbl = malloc(sizeof(struct hash_table_t)))) return NULL;
	if (!((tbl)->d = calloc(INIT_TBL_SIZE, sizeof(struct hash_entry_t)))) {
		free(tbl);
		return NULL;
	}
	tbl->c = 0;
	tbl->s = INIT_TBL_SIZE;

	return tbl;
}

struct hash_table_t *
HT_IncreaseSizeRehash(struct hash_table_t *tbl) {
	struct hash_entry_t *ntbl = NULL, *otbl;
	int i;

	if (!(ntbl = calloc(tbl->s << 1, sizeof(struct hash_entry_t)))) return NULL;
	tbl->c = 0;
	otbl = tbl->d, tbl->d = ntbl;
	for (i = 0, tbl->s <<= 1; i < tbl->s >> 1; i++) if (otbl[i].s) HT_SetValue(otbl[i].k, otbl[i].d, tbl);
	free(otbl);

	return tbl;
}

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
HT_ProbeForBucket(const struct hash_table_t *t, unsigned hash, int i, int set) {
	int len = t->s, s = i ? i - 1 : len, p = 0;

	while (t->d[i].s && t->d[i].h != hash) {
		if (set && t->d[i].s == HT_DELETED) p = i;
		if (i == s) return -1;
		if (++i == len) i = 0;
	}
	return set && !t->d[i].s ? p : i;
}

intptr_t
HT_GetValue(const char *key, HT_HashTable t) {
	unsigned hash = HT_GetHash(key);
	int i = (int)(hash & (t->s - 1));

	if (t->d[i].h == hash && strcmp(t->d[i].k, key) == 0) return t->d[i].d;
	i = HT_ProbeForBucket(t, hash, i, 0);
	return i == -1 ? 0 : t->d[i].d;
}

unsigned
HT_SetValue(const char *key, intptr_t val, HT_HashTable t) {
	unsigned hash = HT_GetHash(key);
	int i;

	if (t->c >= (t->s * 7) / 10) HT_IncreaseSizeRehash(t);
	i = (int)(hash & (t->s - 1));
	if (t->d[i].h && (t->d[i].h != hash || strcmp(t->d[i].k, key) != 0)) i = HT_ProbeForBucket(t, hash, i, 1);
	if (i == -1) return 0;
	if (t->d[i].s != HT_USED) t->c++;
	t->d[i].d = val, t->d[i].h = hash, t->d[i].s = HT_USED;
	strlcpy(t->d[i].k, key, MAX_KEY_LEN - 1);
	return hash;
}

void
HT_DeleteKey(const char *key, HT_HashTable t) {
	unsigned hash = HT_GetHash(key);
	int i = (int)(hash & (t->s - 1));

	if (t->d[i].h != hash || strcmp(t->d[i].k, key) != 0)
		if ((i = HT_ProbeForBucket(t, hash, i, 0)) == -1) return;
	t->d[i].s = HT_DELETED;
	t->c--;
}

void
HT_FreeTable(HT_HashTable tbl) {
	free(tbl->d);
	free(tbl);
}
