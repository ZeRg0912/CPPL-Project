#include <iostream>
#include "INI-Parser.hpp"

int main() {
	setlocale(LC_ALL, "RUSSIAN");
	
	INIParser parser("test-file");
	try {
		auto value = parser.GetValue<int>("Section2.var3");
		std::cout << value << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	std::cout << "\nEnd of process\n";
	//system("pause");
	return 0;
}