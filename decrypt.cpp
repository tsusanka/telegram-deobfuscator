#include <stdio.h>
#include <stdint.h>
#include "decrypt.h"
#include "helpers.h"
#include "tgnet/Datacenter.h"

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
	Datacenter::generateMessageKey(authKey, data + 8, key, false);
	Datacenter::aesIgeEncryption(data + 24, key, key + 32, false, false, length - 24);

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
