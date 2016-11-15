#include <vector>
#include "Unobfuscator.h"

int main(int argc, char **argv)
{
	Unobfuscator *unobfuscator = new Unobfuscator();

	std::vector<std::string> dataFiles = {
			"/home/tomas/MEGA/Skola/diplomka/sniffs/2016-11-16/2.dat"
	};

	unobfuscator->unobfuscate("/home/tomas/MEGA/Skola/diplomka/sniffs/2016-11-16/1-key.dat",
							  dataFiles);

	return 0;
}
