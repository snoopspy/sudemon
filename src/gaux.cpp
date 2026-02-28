#include "gaux.h"

std::string GAux::getDir(std::string argv) {
	ssize_t i = argv.length() - 1;
	while (i >= 0) {
		char& ch = argv.at(i);
		if (ch == '/' || ch == '\\') {
			std::string res = argv.substr(0, i + 1);
			return res;
		}
		i--;
	}
	return "/";
}


const char* GAux::getVersion() {
	const char* version =
#include "../../version.txt"
	;

	return version;
}
