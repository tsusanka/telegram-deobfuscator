#include <stdio.h>
#include <stdint.h>
#include <stdexcept>
#include <string.h>
#include <openssl/aes.h>
#include "decrypt.h"
#include "Unobfuscator.h"
#include "helpers.h"

#define KEY_LENGTH 32
#define IV_LENGTH 16

Unobfuscator::Unobfuscator()
{
}

void Unobfuscator::unobfuscate(char const *fileWithKeyPath, char const *fileWithDataPath)
{
	FILE *fileWithKey = openFile(fileWithKeyPath);
	setKeyFromFile(fileWithKey);
	readData(fileWithKey);

	FILE *file = openFile(fileWithDataPath);
	readData(file);
}

void Unobfuscator::setKeyFromFile(FILE *fileWithKey)
{
	unsigned char keyBytes[KEY_LENGTH];
	unsigned char dummy[8];
	unsigned char temp[64];
	encryptNum = 0;
	memset(keyBytes, 0, KEY_LENGTH);
	memset(iv, 0, IV_LENGTH);
	memset(encryptCount, 0, 16);

	fread(&dummy, 8, 1, fileWithKey); // first 8 bytes are unused
	fread(&keyBytes, KEY_LENGTH, 1, fileWithKey); // key
	fread(&iv, IV_LENGTH, 1, fileWithKey); // iv
	fread(&dummy, 8, 1, fileWithKey); // unused?

	if (AES_set_encrypt_key(keyBytes, KEY_LENGTH * 8, &encryptKey) < 0) { // bits
		throw std::invalid_argument("unable to set encryptKey");
	}

	AES_ctr128_encrypt(temp, temp, 64, &encryptKey, iv, encryptCount, &encryptNum); // ? check?
}

FILE *Unobfuscator::openFile(char const *filename)
{
	FILE *file = fopen(filename, "rb");
	if (!file) {
		throw std::invalid_argument("Unable to open file");
	}
	return file;
}

void Unobfuscator::readData(FILE *file)
{
	unsigned char dataLength[1];
	unsigned char data[300];
	uint8_t realLength;

	fread(&dataLength, 1, 1, file); // length

	AES_ctr128_encrypt(dataLength, dataLength, 1, &encryptKey, iv, encryptCount, &encryptNum);
	realLength = (uint8_t) (*dataLength * 4);
	printf("data len: %d\n", realLength);

	fread(&data, realLength, 1, file); // data

	printf("ctr encrypted: ");
	printHex(data, realLength);
	AES_ctr128_encrypt(data, data, realLength, &encryptKey, iv, encryptCount, &encryptNum);
	printf("ctr decrypted: ");
	printHex(data, realLength);

	decrypt(data, (uint32_t) realLength);
}
