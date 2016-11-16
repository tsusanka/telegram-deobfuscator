#include "Unobfuscator.h"

int main(int argc, char **argv)
{
	Unobfuscator *unobfuscator = new Unobfuscator();

	unobfuscator->unobfuscate("/home/tomas/MEGA/Skola/diplomka/sniffs/2016-11-16/all-follow3.dat");

	return 0;
}
