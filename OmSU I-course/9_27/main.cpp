#include <iostream>
#include "WordAnalysis.h"

int main(){
	std::string line;
	getline(std::cin, line);
	std::vector<std::string> args = split(line);
	AnagramDictionary dictionary;
	for(std::string arg:args)
		dictionary.place(arg);
	std::cout << dictionary.toString(true) << std::endl;
	return 0;
}
