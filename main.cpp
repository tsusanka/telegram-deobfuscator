#include "Unobfuscator.h"


int main(int argc, char **argv)
{
	Unobfuscator *unobfuscator = new Unobfuscator();
	unobfuscator->unobfuscate("/home/tomas/MEGA/Skola/diplomka/sniffs/2016-11-16/1-key.dat",
							  "/home/tomas/MEGA/Skola/diplomka/sniffs/2016-11-16/2.dat");

	return 0;
}
