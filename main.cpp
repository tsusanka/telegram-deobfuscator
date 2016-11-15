#include "Unobfuscator.h"


int main(int argc, char **argv)
{
	Unobfuscator *unobfuscator = new Unobfuscator();
	unobfuscator->unobfuscate("/home/tomas/MEGA/Skola/diplomka/sniffs/2016-11-15/40-key.dat",
							  "/home/tomas/MEGA/Skola/diplomka/sniffs/2016-11-15/41.dat");

	return 0;
}
