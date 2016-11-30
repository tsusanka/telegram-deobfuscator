#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include "Decrypter.h"
#include "Helpers.h"
#include "TLConstructors.h"

Decrypter::Decrypter(std::string keyPath)
{
	readKey(keyPath);
}

void Decrypter::readKey(std::string keyPath)
{
	FILE *file = fopen(keyPath.c_str(), "rb");
	if (!file) {
		throw std::invalid_argument("Unable to open file '" + keyPath + "'");
	}

	fseek(file, 0L, SEEK_END);
	if (ftell(file) != 256) {
		throw std::invalid_argument("Invalid size of the key file, should be 256 bytes");
	}
	rewind(file);

	fread(&authKey, 256, 1, file);
}

/**
 * Decrypts the data and prints what is going on
 * @param data
 * @param length  length of data (with authKeyId and msgKey)
 */
void Decrypter::run(unsigned char *data, uint32_t length, bool incoming)
{
	static uint8_t key[64];
	generateMessageKey(authKey, data + 8, key, incoming);
	aesIgeEncryption(data + 24, key, key + 32, false, false, length - 24);

	printf("Message content (IGE decrypted): ");
	Helpers::printHex(data + 24, length - 24);

	printf("\tSalt: ");
	Helpers::printHex(data + 24, 8);
	printf("\tSession: ");
	Helpers::printHex(data + 24 + 8, 8);
	printf("\tID: ");
	Helpers::printHex(data + 24 + 16, 8);
	printf("\tSequence number: ");
	Helpers::printHex(data + 24 + 24, 4);
	printf("\tLength: ");
	Helpers::printHex(data + 24 + 28, 4);

	int32_t constructor = Helpers::readInt32(data + 24 + 32);
	printf("\tConstructor: %02X %s\n", constructor, TLconstructors.at((uint32_t) constructor));

	printf("\tContent: ");
	Helpers::printHex(data + 24 + 32 + 4, length - 24 - 32 - 4);
	printf("\tContent in ASCII: ");
	Helpers::printAscii(data + 24 + 32 + 4, length - 24 - 32 - 4);

	printf("\n\n");
}

void Decrypter::generateMessageKey(uint8_t *authKey, uint8_t *messageKey, uint8_t *result, bool incoming)
{
	uint32_t x = incoming ? 8 : 0;

	static uint8_t sha[68];

	memcpy(sha + 20, messageKey, 16);
	memcpy(sha + 20 + 16, authKey + x, 32);
	SHA1(sha + 20, 48, sha);
	memcpy(result, sha, 8);
	memcpy(result + 32, sha + 8, 12);

	memcpy(sha + 20, authKey + 32 + x, 16);
	memcpy(sha + 20 + 16, messageKey, 16);
	memcpy(sha + 20 + 16 + 16, authKey + 48 + x, 16);
	SHA1(sha + 20, 48, sha);
	memcpy(result + 8, sha + 8, 12);
	memcpy(result + 32 + 12, sha, 8);

	memcpy(sha + 20, authKey + 64 + x, 32);
	memcpy(sha + 20 + 32, messageKey, 16);
	SHA1(sha + 20, 48, sha);
	memcpy(result + 8 + 12, sha + 4, 12);
	memcpy(result + 32 + 12 + 8, sha + 16, 4);

	memcpy(sha + 20, messageKey, 16);
	memcpy(sha + 20 + 16, authKey + 96 + x, 32);
	SHA1(sha + 20, 48, sha);
	memcpy(result + 32 + 12 + 8 + 4, sha, 8);
}

void Decrypter::aesIgeEncryption(uint8_t *buffer, uint8_t *key, uint8_t *iv, bool encrypt, bool changeIv, uint32_t length)
{
	uint8_t *ivBytes = iv;
	if (!changeIv) {
		ivBytes = new uint8_t[32];
		memcpy(ivBytes, iv, 32);
	}
	AES_KEY akey;
	if (!encrypt) {
		AES_set_decrypt_key(key, 32 * 8, &akey);
		AES_ige_encrypt(buffer, buffer, length, &akey, ivBytes, AES_DECRYPT);
	} else {
		AES_set_encrypt_key(key, 32 * 8, &akey);
		AES_ige_encrypt(buffer, buffer, length, &akey, ivBytes, AES_ENCRYPT);
	}
	if (!changeIv) {
		delete[] ivBytes;
	}
}
