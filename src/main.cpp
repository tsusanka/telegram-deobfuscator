#include "Deobfuscator.h"

void printUsage()
{
	printf("Telegram Deobfuscator\n");
	printf("Usage: <outgoing stream> <incoming stream> [<key>]");
}

int main(int argc, char **argv)
{
	std::string key = "";

	if (argc != 3 && argc != 4) {
		printUsage();
		return 1;
	}
	if (argc == 4) {
		key = argv[3];
	}

	Deobfuscator *deobfuscator = new Deobfuscator(argv[1], argv[2], key);

	deobfuscator->run();

	return 0;
}
