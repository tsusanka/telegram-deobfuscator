#include <stdio.h>
#include <stdint.h>
#include <string>
#include <string.h>
#include <openssl/aes.h>

#ifndef UNOBFUSCATOR_H
#define UNOBFUSCATOR_H

class Unobfuscator
{
public:
	Unobfuscator();

	void unobfuscate(std::string fileWithKey, std::vector<std::string> fileWithData);

private:

	FILE *openFile(std::string filename);

	void readData(FILE *file);

	void setKeyFromFile(FILE *fileWithKey);

	AES_KEY encryptKey;
	unsigned char iv[16];
	uint32_t encryptNum;
	uint8_t encryptCount[16];
};

#endif
