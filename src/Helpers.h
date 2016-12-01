#ifndef HELPERS_H
#define HELPERS_H

class Helpers
{

public:
	static void printHex(const void *buffer, size_t length);

	static void printAscii(unsigned char *data, size_t length);

	static int32_t readInt32(unsigned char *bytes);

};

#endif
