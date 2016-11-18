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

void Unobfuscator::unobfuscate(std::string outgoingPath, std::string incomingPath)
{
	FILE *outgoingFile = openFile(outgoingPath);
	setKeyFromFile(outgoingFile);

	FILE *incomingFile = openFile(incomingPath);

	while(readData(outgoingFile, false)); // read until content
	printf("\n\n---------------------- INCOMING ----------------------\n\n");
	while(readData(incomingFile, true)); // read until content
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

bool Unobfuscator::readData(FILE *file, bool incoming)
{
	unsigned char data[10000]; // todo dynamic
	uint32_t realLength;

	realLength = readRealLength(file, incoming);
	if (realLength == 0) {
		return false;
	}
	DEBUG_PRINT(("real len: %d\n", realLength));

	fread(&data, realLength, 1, file); // data

	DEBUG_PRINT(("ctr encrypted: "));
	if (DEBUG) printHex(data, realLength);

	ctrDecipher(data, data, realLength, incoming);
	DEBUG_PRINT(("ctr decrypted: "));
	if (DEBUG) printHex(data, realLength);

	decrypt(data, (uint32_t) realLength, incoming);

	return true;
}

void Unobfuscator::ctrDecipher(unsigned char *in, unsigned char *out, size_t length, bool incoming)
{
	if (incoming) {
		AES_ctr128_encrypt(in, out, length, &decryptKey, decIv, decryptCount, &decryptNum);
	} else {
		AES_ctr128_encrypt(in, out, length, &encryptKey, encIv, encryptCount, &encryptNum);
	}
}

uint32_t Unobfuscator::readRealLength(FILE *file, bool incoming)
{
	unsigned char length[4];

	size_t success = fread(&length, 1, 1, file);
	if (success == 0) {
		return 0;
	}

	ctrDecipher(length, length, 1, incoming);

	if (length[0] != 0x7f) {
		return (uint32_t) (length[0] * 4);
	} else {
		unsigned char extra[3];
		fread(&extra, 3, 1, file);
		ctrDecipher(extra, extra, 3, incoming);
		length[1] = extra[0];
		length[2] = extra[1];
		length[3] = extra[2];

		return ((uint32_t) readInt32(length) >> 8) * 4;
	}
}
