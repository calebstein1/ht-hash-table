#include <stdio.h>

#include <arm_acle.h>

unsigned int
get_hash(const char *data) {
	char *d = (char *)data;
	unsigned int crc = 0xffffffff;

	while(*d) crc = __crc32cb(crc, *d++);

	return crc ^ 0xffffffff;
}

int
main(void) {
	printf("%u\n", get_hash("Hello, world!"));
	return 0;
}
