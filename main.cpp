#include "Unobfuscator.h"

int main(int argc, char **argv)
{
	Unobfuscator *unobfuscator = new Unobfuscator(
			"/home/tomas/MEGA/Skola/diplomka/sniffs/2016-11-18/all-outgoing.dat",
			"/home/tomas/MEGA/Skola/diplomka/sniffs/2016-11-18/all-incoming.dat",
			"/home/tomas/MEGA/Skola/diplomka/sniffs/key.dat"
	);

	unobfuscator->unobfuscate();

	return 0;
}
