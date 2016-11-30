#include <stdio.h>
#include <stdint.h>
#include <string>
#include <openssl/aes.h>
#include "Decrypter.h"

#ifndef DEOBFUSCATOR_H
#define DEOBFUSCATOR_H

class Deobfuscator
{
public:
	Deobfuscator(std::string outgoingPath, std::string incomingPath, std::string keyPath);

	void deobfuscate();

private:

	FILE *openFile(std::string filename);

	bool readData(FILE *file, bool incoming);

	void setObfuscationKeyFromFile(FILE *fileWithKey);

	AES_KEY encryptKey;
	AES_KEY decryptKey;
	unsigned char encIv[16];
	unsigned char decIv[16];
	uint32_t encryptNum;
	uint32_t decryptNum;
	uint8_t encryptCount[16];
	uint8_t decryptCount[16];
	FILE *outgoingFile;
	FILE *incomingFile;
	Decrypter *decrypter;

	void setDecryptKey(unsigned char *encKeyBytes);

	void setEncryptKey(unsigned char encKeyBytes[32]);

	void ctrDecipher(unsigned char *in, unsigned char *out, size_t length, bool incoming);

	uint32_t readRealLength(FILE *file, bool incoming);
};

#endif
