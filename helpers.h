#ifndef HELPERS_H
#define HELPERS_H

void printHex(const void *buffer, size_t length);

void printAscii(unsigned char *data, size_t length);

int32_t readInt32(unsigned char *bytes);

#endif
