#ifndef HELPERS_H
#define HELPERS_H

#define DEBUG false
#if DEBUG
	#define DEBUG_PRINT(a) printf a
#else
	#define DEBUG_PRINT(a) (void)0
#endif

void printHex(const void *buffer, size_t length);

void printAscii(unsigned char *data, size_t length);

int32_t readInt32(unsigned char *bytes);

#endif
