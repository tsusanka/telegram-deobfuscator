#include <stdio.h>
#include <stdint.h>
#include <openssl/aes.h>

#ifndef UNOBFUSCATOR_H
#define UNOBFUSCATOR_H

class Unobfuscator
{
public:
	Unobfuscator();
	void unobfuscate(char const *fileWithKey, char const *fileWithData);

private:

	FILE *openFile(char const *filename);

	void readData(FILE *file);

	void setKeyFromFile(FILE *fileWithKey);

	AES_KEY encryptKey;
	unsigned char iv[16];
	uint32_t encryptNum;
	uint8_t encryptCount[16];
};

#endif
