#include "ht_hash.h"

#define OFFSET 0x811c9dc5
#define PRIME 0x01000193
#define INIT_TBL_SIZE 8
#define MAX_KEY_LEN 32

struct hash_entry_t {
	uintptr_t d;
	unsigned h, u;
	char k[MAX_KEY_LEN];
};

struct hash_table_t {
	int s, c;
	struct hash_entry_t *d;
};

struct hash_table_t *HT_InitTable(struct hash_table_t **tbl) {
	if (*tbl) return *tbl;
	if (!(*tbl = malloc(sizeof(struct hash_table_t)))) return NULL;
	if (!((*tbl)->d = calloc(INIT_TBL_SIZE, sizeof(struct hash_entry_t)))) {
		free(*tbl);
		return NULL;
	}
	(*tbl)->c = 0;
	(*tbl)->s = INIT_TBL_SIZE;

	return *tbl;
}

struct hash_table_t *HT_IncreaseSizeRehash(struct hash_table_t *tbl) {
	struct hash_entry_t *ntbl = NULL, *otbl;
	int i;

	if (!(ntbl = calloc(tbl->s << 1, sizeof(struct hash_entry_t)))) return NULL;
	otbl = tbl->d, tbl->d = ntbl;
	for (i = 0, tbl->s <<= 1; i < tbl->s >> 1; i++) if (otbl[i].u) HT_SetValue(otbl[i].k, otbl[i].d, tbl);
	free(otbl);

	return tbl;
}

unsigned HT_GetHash(const char *data) {
	unsigned hash = OFFSET;
	char *d = (char *)data;

	if (!d || !*d) return 0;
	while (*d) {
		hash ^= *d++;
		hash *= PRIME;
	}
	return hash;
}

int HT_ProbeForBucket(const struct hash_table_t *t, unsigned hash, int i, int set) {
	int len = t->s, s = i ? i - 1 : len;

	while (t->d[i].h != hash) {
		if (set && !t->d[i].u) break;
		if (i == s) return -1;
		if (++i == len) i = 0;
	}
	return i;
}

uintptr_t HT_GetValue(const char *key, const struct hash_table_t *t) {
	unsigned hash = HT_GetHash(key);
	int i = (int)(hash & (t->s - 1));

	if (t->d[i].h == hash && strcmp(t->d[i].k, key) == 0) return t->d[i].d;
	i = HT_ProbeForBucket(t, hash, i, 0);
	return i == -1 ? 0 : t->d[i].d;
}

unsigned HT_SetValue(const char *key, uintptr_t val, struct hash_table_t *t) {
	unsigned hash = HT_GetHash(key);
	int i;

	if (t->c >= (t->s * 7) / 10) HT_IncreaseSizeRehash(t);
	i = (int)(hash & (t->s - 1));
	if (t->d[i].h && (t->d[i].h != hash || strcmp(t->d[i].k, key) != 0)) i = HT_ProbeForBucket(t, hash, i, 1);
	if (i == -1) return 0;
	t->d[i].d = val, t->d[i].h = hash, t->d[i].u = 1, t->c++;
	strlcpy(t->d[i].k, key, MAX_KEY_LEN - 1);
	return hash;
}

void HT_FreeTable(struct hash_table_t **tbl) {
	if (!tbl) return;
	free((*tbl)->d);
	free(*tbl);
	*tbl = NULL;
}
