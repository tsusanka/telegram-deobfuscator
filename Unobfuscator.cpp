#include <stdio.h>
#include <stdint.h>
#include <stdexcept>
#include <openssl/aes.h>
#include "decrypt.h"
#include "Unobfuscator.h"
#include "helpers.h"

#define KEY_LENGTH 32
#define IV_LENGTH 16

Unobfuscator::Unobfuscator()
{
}

void Unobfuscator::unobfuscate(std::string path)
{
	FILE *file = openFile(path);
	setKeyFromFile(file);

	while(readData(file)); // read until content
}

void Unobfuscator::setDecryptKey(unsigned char *encKeyBytes)
{
	unsigned char decKeyBytes[KEY_LENGTH];

	for (int a = 0; a < IV_LENGTH; a++) {
		decKeyBytes[a] = encIv[IV_LENGTH - 1 - a];
	}
	for (int a = 0; a < IV_LENGTH; a++) {
		decKeyBytes[a + IV_LENGTH] = encKeyBytes[KEY_LENGTH - 1 - a];
	}
	for (int a = 0; a < IV_LENGTH; a++) {
		decIv[a] = encKeyBytes[IV_LENGTH - 1 - a];
	}

	if (AES_set_encrypt_key(decKeyBytes, KEY_LENGTH * 8, &decryptKey) < 0) {
		throw std::invalid_argument("unable to set decryptKey");
	}
}

void Unobfuscator::setEncryptKey(unsigned char *encKeyBytes)
{
	if (AES_set_encrypt_key(encKeyBytes, KEY_LENGTH * 8, &encryptKey) < 0) { // bits
		throw std::invalid_argument("unable to set encryptKey");
	}

}

void Unobfuscator::setKeyFromFile(FILE *fileWithKey)
{
	unsigned char encKeyBytes[KEY_LENGTH];
	unsigned char dummy[8];
	unsigned char temp[64];
	encryptNum = decryptNum = 0;
	memset(encryptCount, 0, 16);
	memset(decryptCount, 0, 16);

	fread(&dummy, 8, 1, fileWithKey); // first 8 bytes are unused
	fread(&encKeyBytes, KEY_LENGTH, 1, fileWithKey); // key
	fread(&encIv, IV_LENGTH, 1, fileWithKey); // iv
	fread(&dummy, 8, 1, fileWithKey); // unused?

	setDecryptKey(encKeyBytes);
	setEncryptKey(encKeyBytes);

	AES_ctr128_encrypt(temp, temp, 64, &encryptKey, encIv, encryptCount, &encryptNum); // ? check?
}

FILE *Unobfuscator::openFile(std::string filename)
{
	FILE *file = fopen(filename.c_str(), "rb");
	if (!file) {
		throw std::invalid_argument("Unable to open file");
	}
	return file;
}

bool Unobfuscator::readData(FILE *file)
{
	unsigned char dataLength[1];
	unsigned char data[300];
	uint8_t realLength;

	size_t success = fread(&dataLength, 1, 1, file); // length
	if (success == 0) {
		return false;
	}

	AES_ctr128_encrypt(dataLength, dataLength, 1, &encryptKey, encIv, encryptCount, &encryptNum);
	realLength = (uint8_t) (*dataLength * 4);
	DEBUG_PRINT(("data len: %d\n", realLength));

	fread(&data, realLength, 1, file); // data

	DEBUG_PRINT(("ctr encrypted: "));
	if (DEBUG) printHex(data, realLength);
	AES_ctr128_encrypt(data, data, realLength, &encryptKey, encIv, encryptCount, &encryptNum);
	DEBUG_PRINT(("ctr decrypted: "));
	if (DEBUG) printHex(data, realLength);

	decrypt(data, (uint32_t) realLength);

	return true;
}
