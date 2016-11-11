#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include "decrypt.h"
#include "helpers.h"

/**
 * Copied from Telegram
 */
void aesIgeEncryption(uint8_t *buffer, uint8_t *key, uint8_t *iv, bool encrypt, bool changeIv, uint32_t length)
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

//s KDF
inline void generateMessageKey(uint8_t *authKey, uint8_t *messageKey, uint8_t *result, bool incoming)
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

/**
 * Decrypts encrypted data
 * @param data
 * @param length  length of data (with authKeyId and msgKey)
 */
void decrypt(unsigned char *data, uint32_t length)
{
	uint8_t authKey[] = {
	};

	static uint8_t key[64];
	generateMessageKey(authKey, data + 8, key, false);
	aesIgeEncryption(data + 24, key, key + 32, false, false, length - 24);

	printf("ige decrypted:\n");
    printHex(data + 24, length - 24);

	printf("\tsalt:\n");
    printHex(data + 24, 8);
	printf("\tsession:\n");
    printHex(data + 24 + 8, 8);
	printf("\tid:\n");
    printHex(data + 24 + 16, 8);
	printf("\tmsg_seq:\n");
    printHex(data + 24 + 24, 4);
	printf("\tmsg_size:\n");
    printHex(data + 24 + 28, 4);
	printf("\tmsg:\n");
    printHex(data + 24 + 32, length - 24 - 32);
	printAscii(data + 24 + 32, length - 24 - 32);

	// last 8 are random id?
}
