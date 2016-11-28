#ifndef DECRYPTOR_H
#define DECRYPTOR_H


class Decryptor
{
public:
	Decryptor();

	void decrypt(unsigned char *data, uint32_t length, bool incoming);

private:

	void generateMessageKey(uint8_t *authKey, uint8_t *messageKey, uint8_t *result, bool incoming);

	void aesIgeEncryption(uint8_t *buffer, uint8_t *key, uint8_t *iv, bool encrypt, bool changeIv, uint32_t length);

};

#endif
