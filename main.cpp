#include "Deobfuscator.h"

int main(int argc, char **argv)
{
	Deobfuscator *deobfuscator = new Deobfuscator(
			"/home/tomas/MEGA/Skola/diplomka/sniffs/2016-11-18/all-outgoing.dat",
			"/home/tomas/MEGA/Skola/diplomka/sniffs/2016-11-18/all-incoming.dat",
			"/home/tomas/MEGA/Skola/diplomka/sniffs/key.dat"
	);

	deobfuscator->run();

	return 0;
}
