#include "Unobfuscator.h"

int main(int argc, char **argv)
{
	Unobfuscator *unobfuscator = new Unobfuscator();

	unobfuscator->unobfuscate("/home/tomas/MEGA/Skola/diplomka/sniffs/2016-11-18/all-outgoing.dat",
							  "/home/tomas/MEGA/Skola/diplomka/sniffs/2016-11-18/all-incoming.dat");

	return 0;
}
