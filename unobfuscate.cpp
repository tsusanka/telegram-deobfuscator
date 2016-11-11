#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <openssl/aes.h>
#include "decrypt.h"
#include "helpers.h"


int main(int argc, char **argv)
{
	const int keyLength = 32;
	const int ivLength = 16;

	unsigned char key[32];
	unsigned char iv[16];
	unsigned char dummy[8];
	unsigned char temp[64];
	unsigned char dataLength[1];
	unsigned char data[300];
	memset(key, 0, keyLength);
	memset(iv, 0, ivLength);
	AES_KEY encryptKey;
	uint32_t encryptNum = 0;
	uint8_t encryptCount[16];
	memset(encryptCount, 0, 16);
	uint8_t realLength;


	FILE *file = fopen(argv[1], "rb");
	if (!file) {
		printf("Unable to open file");
		return 1;
	}
	fread(&dummy, 8, 1, file); // first 8 bytes are unused
	fread(&key, keyLength, 1, file); // key
	fread(&iv, ivLength, 1, file); // iv
	fread(&dummy, 8, 1, file); // unused?
	fread(&dataLength, 1, 1, file); // length

	if (AES_set_encrypt_key(key, 256, &encryptKey) < 0) { // bits
		printf("unable to set encryptKey");
		return 2;
	}

	AES_ctr128_encrypt(temp, temp, 64, &encryptKey, iv, encryptCount, &encryptNum); // ? check?

	AES_ctr128_encrypt(dataLength, dataLength, 1, &encryptKey, iv, encryptCount, &encryptNum);
	realLength = (uint8_t) (*dataLength * 4);
	printf("data len: %d\n", realLength);

	fread(&data, realLength, 1, file); // data

	AES_ctr128_encrypt(data, data, realLength, &encryptKey, iv, encryptCount, &encryptNum);
	printf("ctr decrypted: ");
	printHex(data, realLength);


	decrypt(data, (uint32_t) realLength);

	return 0;
}
