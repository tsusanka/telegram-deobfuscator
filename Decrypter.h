#include <stdio.h>
#include <string>

#ifndef DECRYPTER_H
#define DECRYPTER_H

class Decrypter
{
public:
	Decrypter(std::string keyPath);

	void decrypt(unsigned char *data, uint32_t length, bool incoming);

private:

	FILE *keyFile;
	uint8_t authKey[256];

	void readKey(std::string keyPath);

	void generateMessageKey(uint8_t *authKey, uint8_t *messageKey, uint8_t *result, bool incoming);

	void aesIgeEncryption(uint8_t *buffer, uint8_t *key, uint8_t *iv, bool encrypt, bool changeIv, uint32_t length);

};

#endif
